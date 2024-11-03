#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <iostream> 
#include <thread>

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

// Game object structures
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
    
private:  
    // Audio handling
    static SDL_AudioDeviceID audioDevice;
    
    void playWav(const char* filename);

    void playWavInThread(const char* filename);
    
    // Drawing functions
    void drawSquare(int x, int y, int width, int height);

    // Time management
    Uint32 currentTime;
    Uint32 lastTime;
    float deltaTime;
    const float TARGET_DELTA_TIME = 1.0f / 60.0f;  // Target 60 FPS
    const float BASE_MOVE_SPEED = 400.0f;          // Pixels per second
    Uint32 lastTurnTime = 0;
    float dt;
    float LastTime;

    // Game display functions
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

    // Game state
    bool isRunning;
    bool gameOver;
    int score = 0;
    int snakeLength = 1;
    float gradientSteps = 1.0f;
    int frames_count = 0;

    // Game objects
    Button playAgainButton;
    Button quitButton;
    food apple;
    SDL_Rect squares[400];
    
    // Window dimensions
    int screen_width = 1200;
    int screen_height = 800;
    const int GRIDSIZE = 50;
    int total_grids = (screen_width / GRIDSIZE) * (screen_height / GRIDSIZE);

    // Snake structure and data
    struct snake { 
        int xPos; 
        int yPos; 
        int last_x; 
        int last_y; 
        std::string direction;
        std::string queue_direction = "RIGHT";
    };
    snake snakeBody[400] = {100, 100, 100, 100, "RIGHT", "RIGHT"};

    // Game logic functions
    bool isPointInRect(int x, int y, const SDL_Rect& rect);
    void resetGame();
    bool collisions();
    void endGame();
    void addSnake();
    void addApple();
    food findPos();
    std::string checkInput();
    
    // SDL components
    SDL_Window *window;
    SDL_Renderer *renderer;
};