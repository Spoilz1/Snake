#include "../core/Game.hpp"

// Game Initialization
bool Game::Initialize(const char* title, int xpos, int ypos, bool fullscreen) {
    int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    // Initialize SDL with both video and audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set up OpenGL attributes for anti-aliasing
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Button initialization
    playAgainButton.rect = {screen_width/2 - 200, screen_height/2 + 100, 180, 60};
    playAgainButton.text = "PLAY";
    playAgainButton.isHovered = false;

    quitButton.rect = {screen_width/2 + 20, screen_height/2 + 100, 180, 60};
    quitButton.text = "NO";
    quitButton.isHovered = false;

    // Create window
    window = SDL_CreateWindow(title, xpos, ypos, screen_width, screen_height, flags);
    if (!window) {
        std::cout << "Window creation failed. SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create OpenGL context
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cout << "OpenGL context creation failed. SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cout << "Renderer creation failed. SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize audio
    SDL_AudioSpec wanted;
    wanted.freq = 44100;
    wanted.format = AUDIO_S16LSB;
    wanted.channels = 2;
    wanted.samples = 4096;
    wanted.callback = nullptr;  

    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &wanted, nullptr, 0);
    if (audioDevice == 0) {
        std::cout << "Failed to open audio device: " << SDL_GetError() << std::endl;
    }

    // Start audio playback
    SDL_PauseAudioDevice(audioDevice, 0);

    // Enable anti-aliasing
    glEnable(GL_MULTISAMPLE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    isRunning = true;
    return true;
}


void Game::setupAntiAliasing() {
    glViewport(0, 0, screen_width, screen_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screen_width, screen_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Game State Management
bool Game::IsRunning() { return isRunning; }

bool Game::isPaused() { 
    return paused; 
}

void Game::togglePause() { 
    paused = !paused; 
}

void Game::resetGame() {
    paused = false;
    score = 0;
    snakeLength = 1;
    gameOver = false;
    
    // Initialize head with default position and RIGHT direction
    snake head;
    head.xPos = 100;
    head.yPos = 100;
    head.last_x = 100;
    head.last_y = 100;
    head.direction = Direction::RIGHT;
    head.queue_direction = Direction::RIGHT;
    snakeBody[0] = head;

    // Initialize rest of the snake body with default values
    for (int i = 1; i < 400; i++) {
        snake segment;
        segment.xPos = 0;
        segment.yPos = 0;
        segment.last_x = 0;
        segment.last_y = 0;
        segment.direction = Direction::RIGHT;  // Default direction
        segment.queue_direction = Direction::RIGHT;
        snakeBody[i] = segment;
    }
    
    updateWindowTitle();
}

void Game::endGame() { 
    togglePause();
    playWavInThread("../assets/lose.wav"); 
    gameOver = true;
}