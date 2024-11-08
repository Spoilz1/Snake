#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef __APPLE__
    #include <TargetConditionals.h>
    #include <SDL2/SDL_opengl.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <string>
#include <iostream>
#include <thread>



// Direction enum to replace string comparisons
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct AppleGradient {
    float colorVariation;
    float highlightX;
    float highlightY;
};



// Digital display segments for letters A-Z
const bool letterSegments[26][7] = {
    {1,1,1,1,1,1,0}, // A
    {1,1,1,1,1,1,1}, // B
    {1,1,0,0,1,1,1}, // C
    {1,0,1,1,1,1,1}, // D
    {1,1,0,1,1,1,1}, // E
    {1,1,0,1,1,1,0}, // F
    {1,1,0,0,1,1,1}, // G
    {0,1,1,1,1,1,0}, // H
    {0,0,1,0,0,1,0}, // I
    {0,0,1,0,1,1,1}, // J
    {0,1,1,1,1,1,0}, // K
    {0,1,0,0,1,1,1}, // L
    {1,1,1,0,1,1,0}, // M
    {1,1,1,0,1,1,0}, // N
    {1,1,1,0,1,1,1}, // O
    {1,1,1,1,1,0,0}, // P
    {1,1,1,1,0,1,0}, // Q
    {1,1,1,1,1,1,0}, // R
    {1,1,0,1,0,1,1}, // S
    {1,0,0,0,0,0,0}, // T
    {0,1,1,0,1,1,1}, // U
    {0,1,1,0,1,1,1}, // V
    {0,1,1,0,1,1,1}, // W
    {0,1,1,1,1,1,0}, // X
    {0,1,1,1,0,1,1}, // Y
    {1,0,1,1,1,0,1}  // Z
};

// Structs for game objects
struct food { 
    float xPos; 
    float yPos; 
    SDL_Rect apple;
};

struct Button {
    SDL_Rect rect;
    std::string text;
    bool isHovered;
    SDL_Color color;  
};

struct Point {
    float x;
    float y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

struct Polygon {
    static const int MAX_VERTICES = 16;  
    Point vertices[MAX_VERTICES];
    int vertexCount;  
    int greenIntensity;
    int blueIntensity;
    
    Polygon() : vertexCount(0), greenIntensity(0), blueIntensity(0) {}
};

struct snake { 
    int xPos; 
    int yPos; 
    int last_x; 
    int last_y; 
    Direction direction;
    Direction queue_direction;
    
    snake(int x = 100, int y = 100) : 
        xPos(x), 
        yPos(y), 
        last_x(x), 
        last_y(y), 
        direction(Direction::RIGHT), 
        queue_direction(Direction::RIGHT) {}
};

// Main Game Class
class Game {
public:
    Game();
    ~Game();
    bool Initialize(const char* title, int xpos, int ypos, bool fullscreen);
    void HandleEvents();
    void Render();
    void Clean();
    bool IsRunning();
    void Update();
    void sleep(int);
    bool isPaused(); 
    
private:  

    void drawSnakeEyes(const SDL_Rect& headRect);

    SDL_Texture* appleTexture = nullptr;
    SDL_Texture* stemAndLeafTexture = nullptr;
    bool texturesInitialized = false;
    
    void initializeTextures();
    void createAppleTexture();
    void createStemAndLeafTexture();
    void cleanupTextures();

    void initNoisePattern();
    void renderBackground();
    float smoothNoise(float x, float y);
    // SDL components
    SDL_Window *window;
    SDL_Renderer *renderer;

    // Anti-aliasing related members
    SDL_GLContext glContext;
    bool useOpenGL;
    void setupAntiAliasing();
    void cleanupOpenGL();

    // Audio handling
    static SDL_AudioDeviceID audioDevice;
    void playWav(const char* filename);
    void playWavInThread(const char* filename);
    void cleanupAudio();
    void initializeAudio();
    
    // Drawing functions
    void drawSquare(int x, int y, int width, int height);
    void drawLetter(char letter, int x, int y, int size);
    void drawDigit(int digit, int x, int y, int size);
    void displayLoseScreen();
    void updateWindowTitle();
    void displayScore();
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawLine(int, int, int, int);
    void drawSnakeConnector(int);
    void renderButton(const Button& button, Uint8 normalR, Uint8 normalG, Uint8 normalB, 
                     Uint8 hoverR, Uint8 hoverG, Uint8 hoverB);

    // Time management
    Uint32 currentTime;
    Uint32 lastTime;
    float deltaTime;
    const float TARGET_DELTA_TIME = 1.0f / 60.0f;  
    const float BASE_MOVE_SPEED = 400.0f;          
    

    // Game state
    void togglePause();
    bool paused = false;
    bool isRunning;
    bool gameOver;
    int score = 0;
    int snakeLength = 1;
    float gradientSteps = 1.0f;
    int frames_count = 0;

    // Movement and update functions
    Direction checkInput();
    void updateSnakeDirection(Direction new_direction);
    bool updateSnakeHead(float frameSpeed);
    void updateBodyDirections();
    void updateSnakeBody(float frameSpeed);
    void updateSegmentPosition(snake& segment, float targetX, float targetY, float frameSpeed);
    void updateSegmentDirection(snake& segment, int current_x, int current_y, int target_x, int target_y);
    void snapToGrid(snake& segment);
    void alignSnakeToGrid();
    float calculateFrameSpeed();

    // Game objects
    Button playAgainButton;
    Button quitButton;
    food apple;
    SDL_Rect squares[400];
    snake snakeBody[400];
    AppleGradient currentGradient;
    
    // Window dimensions
    int screen_width = 1200;
    int screen_height = 800;
    const int GRIDSIZE = 50;
    int total_grids = (screen_width / GRIDSIZE) * (screen_height / GRIDSIZE);
    
    // Game logic functions
    bool isPointInRect(int x, int y, const SDL_Rect& rect);
    void resetGame();
    bool collisions();
    void endGame();
    void addSnake();
    void addApple();
    food findPos();
};
