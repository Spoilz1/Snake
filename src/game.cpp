#include "../include/game.hpp"
#include <iostream>
#include <random>


// Lifecycle/Engine
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), gameOver(false),
    currentTime(0), lastTime(0), deltaTime(0) {}

Game::~Game() {
    Clean();
}

bool Game::Initialize(const char* title, int xpos, int ypos, bool fullscreen) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Could not initialize audio: " << SDL_GetError() << std::endl;
        return false;
    }
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Could not initialize audio: " << SDL_GetError() << std::endl;
        return false;
    }

    // Configure the desired audio specs and open the device once here.
    SDL_AudioSpec wavSpec;
    wavSpec.freq = 44100; // Sample rate
    wavSpec.format = AUDIO_S16SYS; // Format
    wavSpec.channels = 2; // Stereo
    wavSpec.samples = 4096; // Buffer size
    wavSpec.callback = nullptr; // No callback
    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);

    if (audioDevice == 0) {
        std::cerr << "Could not open audio device: " << SDL_GetError() << std::endl;
        return false;
    }
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL initialized..." << std::endl;
        
        std::string initial_title = "Score: 0";
        window = SDL_CreateWindow(initial_title.c_str(), xpos, ypos, screen_width, screen_height, flags);
        if (window) {
            std::cout << "Window created..." << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created..." << std::endl;
        }

        isRunning = true;
    } else {
        isRunning = false;
    }

    playAgainButton.rect = {screen_width/2 - 200, screen_height/2 + 100, 180, 60};
    playAgainButton.text = "PLAY";
    playAgainButton.isHovered = false;
    
    quitButton.rect = {screen_width/2 + 20, screen_height/2 + 100, 180, 60};
    quitButton.text = "NO";
    quitButton.isHovered = false;
    
    lastTime = SDL_GetTicks();

    return isRunning;
}

bool Game::IsRunning() { return isRunning; }

void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            
            case SDL_MOUSEMOTION:
                if (gameOver) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;
                    
                    playAgainButton.isHovered = isPointInRect(mouseX, mouseY, playAgainButton.rect);
                    quitButton.isHovered = isPointInRect(mouseX, mouseY, quitButton.rect);
                }
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                if (gameOver && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    
                    if (isPointInRect(mouseX, mouseY, playAgainButton.rect)) {
                        resetGame();
                    }
                    else if (isPointInRect(mouseX, mouseY, quitButton.rect)) {
                        isRunning = false;
                    }
                }
                break;
            
            case SDL_KEYDOWN:
                if (gameOver) {
                    switch (event.key.keysym.sym) {
                        case SDLK_RETURN:  // Enter key to play again
                            resetGame();
                            break;
                        case SDLK_ESCAPE:  // Escape key to quit
                            isRunning = false;
                            break;
                    }
                }
                break;
        }
    }
}

void Game::Update() {
    
    std::string new_direction = checkInput();


    if (snakeBody[0].direction != new_direction) { 
        snakeBody[0].queue_direction = new_direction; 
    }

    
    
    // Calculate delta time
    currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0f; // Convert to seconds
    lastTime = currentTime;

    // Limit delta time to prevent large jumps
    if (deltaTime > 0.1f) {
        deltaTime = 0.1f;
    }

    // Calculate movement based on delta time
    float frameSpeed = BASE_MOVE_SPEED * deltaTime;

    bool headReachedGrid = false;
    
    if (snakeBody[0].direction == "UP") {
        snakeBody[0].yPos -= frameSpeed;
        if (snakeBody[0].yPos <= snakeBody[0].last_y - GRIDSIZE) {
            snakeBody[0].yPos = snakeBody[0].last_y - GRIDSIZE;
            headReachedGrid = true;
        }
    }
    else if (snakeBody[0].direction == "DOWN") {
        snakeBody[0].yPos += frameSpeed;
        if (snakeBody[0].yPos >= snakeBody[0].last_y + GRIDSIZE) {
            snakeBody[0].yPos = snakeBody[0].last_y + GRIDSIZE;
            headReachedGrid = true;
        }
    }
    else if (snakeBody[0].direction == "LEFT") {
        snakeBody[0].xPos -= frameSpeed;
        if (snakeBody[0].xPos <= snakeBody[0].last_x - GRIDSIZE) {
            snakeBody[0].xPos = snakeBody[0].last_x - GRIDSIZE;
            headReachedGrid = true;
        }
    }
    else if (snakeBody[0].direction == "RIGHT") {
        snakeBody[0].xPos += frameSpeed;
        if (snakeBody[0].xPos >= snakeBody[0].last_x + GRIDSIZE) {
            snakeBody[0].xPos = snakeBody[0].last_x + GRIDSIZE;
            headReachedGrid = true;
        }
    }

    if (headReachedGrid) {
        snakeBody[0].direction = snakeBody[0].queue_direction;
        snakeBody[0].xPos = ((snakeBody[0].xPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
        snakeBody[0].yPos = ((snakeBody[0].yPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
        
        snakeBody[0].last_x = snakeBody[0].xPos;
        snakeBody[0].last_y = snakeBody[0].yPos;
        
        for (int i = 1; i < snakeLength; i++) {
            snakeBody[i].xPos = ((snakeBody[i].xPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
            snakeBody[i].yPos = ((snakeBody[i].yPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
            
            int current_x = snakeBody[i].xPos;
            int current_y = snakeBody[i].yPos;
            
            int target_x = snakeBody[i-1].last_x;
            int target_y = snakeBody[i-1].last_y;
            
            if (target_x < current_x) snakeBody[i].direction = "LEFT";
            else if (target_x > current_x) snakeBody[i].direction = "RIGHT";
            else if (target_y < current_y) snakeBody[i].direction = "UP";
            else if (target_y > current_y) snakeBody[i].direction = "DOWN";
            
            snakeBody[i].last_x = current_x;
            snakeBody[i].last_y = current_y;
            snakeBody[i].queue_direction = snakeBody[i].direction;
        }
    }

    for (int i = 1; i < snakeLength; i++) {
        float targetX = snakeBody[i-1].last_x;
        float targetY = snakeBody[i-1].last_y;
        
        if (snakeBody[i].direction == "UP") {
            snakeBody[i].yPos = std::max(targetY, snakeBody[i].yPos - frameSpeed);
            snakeBody[i].xPos = ((snakeBody[i].xPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
        }
        else if (snakeBody[i].direction == "DOWN") {
            snakeBody[i].yPos = std::min(targetY, snakeBody[i].yPos + frameSpeed);
            snakeBody[i].xPos = ((snakeBody[i].xPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
        }
        else if (snakeBody[i].direction == "LEFT") {
            snakeBody[i].xPos = std::max(targetX, snakeBody[i].xPos - frameSpeed);
            snakeBody[i].yPos = ((snakeBody[i].yPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
        }
        else if (snakeBody[i].direction == "RIGHT") {
            snakeBody[i].xPos = std::min(targetX, snakeBody[i].xPos + frameSpeed);
            snakeBody[i].yPos = ((snakeBody[i].yPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
        }
    }
    if (collisions()) {
        endGame();
    }
}

void Game::Render() {
    if (gameOver) {
        displayLoseScreen();
        return;
    }

    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    if (snakeLength == 1) { 
        addApple();
        addSnake();
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &apple.apple);
    
   
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    
    for(int i = 0; i < snakeLength; i++) { 
        float gradientPosition = static_cast<float>(i) / (snakeLength * gradientSteps);
        int greenIntensity = 255 - static_cast<int>(gradientPosition * 155);
        greenIntensity = (greenIntensity < 100) ? 100 : greenIntensity;
        int blueIntensity = static_cast<int>(gradientPosition * 50);
        blueIntensity = (blueIntensity > 50) ? 50 : blueIntensity;
        
        SDL_SetRenderDrawColor(renderer, 0, greenIntensity, blueIntensity, 255);
        
        squares[i].x = snakeBody[i].xPos;
        squares[i].y = snakeBody[i].yPos;
        squares[i].w = GRIDSIZE;
        squares[i].h = GRIDSIZE;
        
        SDL_RenderFillRect(renderer, &squares[i]);
    }
    
    for(int i = snakeLength-1; i > 0; i--) {
        drawSnakeConnector(i);
    }

    updateWindowTitle();
    SDL_RenderPresent(renderer);
}

void Game::Clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned..." << std::endl;
}

// Game State Management
void Game::resetGame() {
    score = 0;
    snakeLength = 1;
    gameOver = false;
    
    snakeBody[0] = {100, 100, 100, 100, "RIGHT", "RIGHT"};
    

    for (int i = 1; i < 400; i++) {
        snakeBody[i] = {0, 0, 0, 0, "", ""};
    }
    
    updateWindowTitle();
}

void Game::endGame() { 
    gameOver = true;
}

void Game::sleep(int ms) { 
    SDL_Delay(ms);
}


// Input
 std::string Game::checkInput() { 
    const Uint8* state = SDL_GetKeyboardState(NULL);
    std::string current_direction = snakeBody[0].direction;
    std::string new_direction = current_direction;
    
    bool up = (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]);
    bool down = (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]);
    bool left = (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]);
    bool right = (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]);
    
    // If no input, return current direction
    if (!up && !down && !left && !right) {
        return current_direction;
    }

    bool moving_vertical = (current_direction == "UP" || current_direction == "DOWN");
    bool moving_horizontal = (current_direction == "LEFT" || current_direction == "RIGHT");
    
    bool directionChanged = false;
    bool at_right_edge = (snakeBody[0].xPos >= screen_width - GRIDSIZE);
    bool at_left_edge = (snakeBody[0].xPos < GRIDSIZE);
    bool at_top_edge = (snakeBody[0].yPos < GRIDSIZE);
    bool at_bottom_edge = (snakeBody[0].yPos >= screen_height - GRIDSIZE);
    bool near_edge = at_right_edge || at_left_edge || at_top_edge || at_bottom_edge;

    std::cout << "Near edge: " << near_edge << std::endl;
    std::cout << "Position: " << snakeBody[0].xPos << ", " << snakeBody[0].yPos << std::endl;
    
    if (!near_edge) {  // Use priority system when not near edge
        if (moving_vertical) {
            if (left && current_direction != "RIGHT") {
                new_direction = "LEFT";
                directionChanged = true;
            }
            else if (right && current_direction != "LEFT") {
                new_direction = "RIGHT";
                directionChanged = true;
            }
            else if (up && current_direction != "DOWN") {
                new_direction = "UP";
                directionChanged = true;
            }
            else if (down && current_direction != "UP") {
                new_direction = "DOWN";
                directionChanged = true;
            }
        }
        else if (moving_horizontal) {
            if (up && current_direction != "DOWN") {
                new_direction = "UP";
                directionChanged = true;
            }
            else if (down && current_direction != "UP") {
                new_direction = "DOWN";
                directionChanged = true;
            }
            else if (left && current_direction != "RIGHT") {
                new_direction = "LEFT";
                directionChanged = true;
            }
            else if (right && current_direction != "LEFT") {
                new_direction = "RIGHT";
                directionChanged = true;
            }
        }
    } 
    else {  // Handle edge cases
        if (at_right_edge) {
            // At right edge, prioritize UP/DOWN/LEFT, but allow RIGHT
            if (up && current_direction != "DOWN") {
                new_direction = "UP";
            }
            else if (down && current_direction != "UP") {
                new_direction = "DOWN";
            }
            else if (left && current_direction != "RIGHT") {
                new_direction = "LEFT";
            }
            else if (right && current_direction != "LEFT") {
                new_direction = "RIGHT";
            }
        }
        else if (at_left_edge) {
            // At left edge, prioritize UP/DOWN/RIGHT, but allow LEFT
            if (up && current_direction != "DOWN") {
                new_direction = "UP";
            }
            else if (down && current_direction != "UP") {
                new_direction = "DOWN";
            }
            else if (right && current_direction != "LEFT") {
                new_direction = "RIGHT";
            }
            else if (left && current_direction != "RIGHT") {
                new_direction = "LEFT";
            }
        }
        else if (at_top_edge) {
            // At top edge, prioritize LEFT/RIGHT/DOWN, but allow UP
            if (left && current_direction != "RIGHT") {
                new_direction = "LEFT";
            }
            else if (right && current_direction != "LEFT") {
                new_direction = "RIGHT";
            }
            else if (down && current_direction != "UP") {
                new_direction = "DOWN";
            }
            else if (up && current_direction != "DOWN") {
                new_direction = "UP";
            }
        }
        else if (at_bottom_edge) {
            // At bottom edge, prioritize LEFT/RIGHT/UP, but allow DOWN
            if (left && current_direction != "RIGHT") {
                new_direction = "LEFT";
            }
            else if (right && current_direction != "LEFT") {
                new_direction = "RIGHT";
            }
            else if (up && current_direction != "DOWN") {
                new_direction = "UP";
            }
            else if (down && current_direction != "UP") {
                new_direction = "DOWN";
            }
        }
    }
    
    return new_direction;
}


// Collision & Positioning
bool Game::collisions() { 
    const float tolerance = (GRIDSIZE/5) * 1.0f;
    for (int i = 1; i < snakeLength; i++) {
        if (snakeBody[i].xPos < 0 || snakeBody[i].xPos >= screen_width || snakeBody[i].yPos < 0 || snakeBody[i].yPos >= screen_height) {
            return true; // Collision with wall
        }
    }
    for (int i = 1; i < snakeLength; i++) {
        
        if (snakeBody[0].xPos >= snakeBody[i].xPos - tolerance && 
            snakeBody[0].xPos <= snakeBody[i].xPos + tolerance &&
            snakeBody[0].yPos >= snakeBody[i].yPos - tolerance && 
            snakeBody[0].yPos <= snakeBody[i].yPos + tolerance) {
            return true; // Collision with self
        }
    }
    if (snakeBody[0].xPos >= apple.xPos - tolerance && 
            snakeBody[0].xPos <= apple.xPos + tolerance &&
            snakeBody[0].yPos >= apple.yPos - tolerance && 
            snakeBody[0].yPos <= apple.yPos + tolerance) {
            score++; 
            playWavInThread("../include/turn.wav"); 
            addApple(); 
            addSnake();
            
        }
    if (snakeLength >= total_grids){
        endGame(); 
    }
    
    

    return false;
}

bool Game::isPointInRect(int x, int y, const SDL_Rect& rect) {
    return (x >= rect.x && x < rect.x + rect.w &&
            y >= rect.y && y < rect.y + rect.h);
}

food Game::findPos() { 
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int grid_cols = screen_width / GRIDSIZE;
    int grid_rows = screen_height / GRIDSIZE;
    
    std::uniform_int_distribution<> width_dist(0, grid_cols - 1);
    std::uniform_int_distribution<> height_dist(0, grid_rows - 1);
    
    food newPos;
    bool validPosition = false;
    const float tolerance = (GRIDSIZE/5) * 1.0f;
    
    while (!validPosition) {
        newPos.xPos = width_dist(gen) * GRIDSIZE;
        newPos.yPos = height_dist(gen) * GRIDSIZE;
        
        validPosition = true;
        
        for (int i = 0; i < snakeLength; i++) {
            if (newPos.xPos >= snakeBody[i].xPos - tolerance &&
                newPos.xPos <= snakeBody[i].xPos + tolerance &&
                newPos.yPos >= snakeBody[i].yPos - tolerance &&
                newPos.yPos <= snakeBody[i].yPos + tolerance) {
                validPosition = false;
                break;
            }
        }
        
        if (validPosition) {
            newPos.apple.x = newPos.xPos;
            newPos.apple.y = newPos.yPos;
            newPos.apple.w = GRIDSIZE;
            newPos.apple.h = GRIDSIZE;
        }
    }
    
    return newPos;
}

void Game::addSnake() {
    
    SDL_Rect square;
    squares[snakeLength] = square;
    
    int baseX = ((snakeBody[snakeLength-1].xPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
    int baseY = ((snakeBody[snakeLength-1].yPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
    
    if (snakeBody[snakeLength-1].direction == "UP") {
        snakeBody[snakeLength].yPos = baseY + GRIDSIZE;
        snakeBody[snakeLength].xPos = baseX;
        snakeBody[snakeLength].last_x = baseX;
        snakeBody[snakeLength].last_y = baseY + GRIDSIZE;
    }
    else if (snakeBody[snakeLength-1].direction == "DOWN") {
        snakeBody[snakeLength].yPos = baseY - GRIDSIZE;
        snakeBody[snakeLength].xPos = baseX;
        snakeBody[snakeLength].last_x = baseX;
        snakeBody[snakeLength].last_y = baseY - GRIDSIZE;
    }
    else if (snakeBody[snakeLength-1].direction == "LEFT") {
        snakeBody[snakeLength].yPos = baseY;
        snakeBody[snakeLength].xPos = baseX + GRIDSIZE;
        snakeBody[snakeLength].last_x = baseX + GRIDSIZE;
        snakeBody[snakeLength].last_y = baseY;
    }
    else if (snakeBody[snakeLength-1].direction == "RIGHT") {
        snakeBody[snakeLength].yPos = baseY;
        snakeBody[snakeLength].xPos = baseX - GRIDSIZE;
        snakeBody[snakeLength].last_x = baseX - GRIDSIZE;
        snakeBody[snakeLength].last_y = baseY;
    }
    
    snakeBody[snakeLength].direction = snakeBody[snakeLength-1].direction;
    snakeBody[snakeLength].queue_direction = snakeBody[snakeLength-1].direction;
    
    snakeLength++;
}

void Game::addApple() { 
    food newApple = findPos();
    apple = newApple;
}


// Rendering 
void Game::drawSquare(int x, int y, int width, int height) {
        SDL_Rect square;
        square.x = x;
        square.y = y;
        square.w = width;
        square.h = height;
        
        SDL_RenderFillRect(renderer, &square);
        SDL_RenderDrawRect(renderer, &square);
    }

void Game::drawLine(int x1, int y1, int x2, int y2) { 
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Game::drawDigit(int digit, int x, int y, int size) {
    const bool segments[10][7] = {
        {1,1,1,0,1,1,1}, // 0
        {0,0,1,0,0,1,0}, // 1
        {1,0,1,1,1,0,1}, // 2
        {1,0,1,1,0,1,1}, // 3
        {0,1,1,1,0,1,0}, // 4
        {1,1,0,1,0,1,1}, // 5
        {1,1,0,1,1,1,1}, // 6
        {1,0,1,0,0,1,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };

    int thickness = size / 6;  // Segment thickness
    int gap = thickness;   // Gap between segments
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color for segments
    
    SDL_Rect segment;
    
    if (segments[digit][0]) { // Top horizontal
        segment = {x + gap, y, size - 2*gap, thickness};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][1]) { // Top left vertical
        segment = {x, y + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][2]) { // Top right vertical
        segment = {x + size - thickness, y + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][3]) { // Middle horizontal
        segment = {x + gap, y + (size/2) - thickness/2, size - 2*gap, thickness};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][4]) { // Bottom left vertical
        segment = {x, y + (size/2) + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][5]) { // Bottom right vertical
        segment = {x + size - thickness, y + (size/2) + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][6]) { // Bottom horizontal
        segment = {x + gap, y + size - thickness, size - 2*gap, thickness};
        SDL_RenderFillRect(renderer, &segment);
    }
}

void Game::drawLetter(char letter, int x, int y, int size) {
    if (letter < 'A' || letter > 'Z') return;
    
    int index = letter - 'A';
    int thickness = size / 6;
    int gap = thickness;
    
    SDL_Rect segment;
    
    if (letterSegments[index][0]) { // Top horizontal
        segment = {x + gap, y, size - 2*gap, thickness};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (letterSegments[index][1]) { // Top left vertical
        segment = {x, y + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (letterSegments[index][2]) { // Top right vertical
        segment = {x + size - thickness, y + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (letterSegments[index][3]) { // Middle horizontal
        segment = {x + gap, y + (size/2) - thickness/2, size - 2*gap, thickness};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (letterSegments[index][4]) { // Bottom left vertical
        segment = {x, y + (size/2) + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (letterSegments[index][5]) { // Bottom right vertical
        segment = {x + size - thickness, y + (size/2) + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (letterSegments[index][6]) { // Bottom horizontal
        segment = {x + gap, y + size - thickness, size - 2*gap, thickness};
        SDL_RenderFillRect(renderer, &segment);
    }
}

void Game::drawSnakeConnector(int i) {
    
    std::string currentDir = snakeBody[i].direction;
    std::string prevDir = snakeBody[i-1].direction;
    
    
    int curX = snakeBody[i].xPos;
    int curY = snakeBody[i].yPos;
    int prevX = snakeBody[i-1].xPos;
    int prevY = snakeBody[i-1].yPos;


    float gradientPosition = static_cast<float>(i) / (snakeLength * gradientSteps);
        
    
    int greenIntensity = 255 - static_cast<int>(gradientPosition * 155);
    greenIntensity = (greenIntensity < 100) ? 100 : greenIntensity;
    
    int blueIntensity = static_cast<int>(gradientPosition * 50);
    blueIntensity = (blueIntensity > 50) ? 50 : blueIntensity;
    
    SDL_SetRenderDrawColor(renderer, 0, greenIntensity, blueIntensity, 255);
    
    if (currentDir == prevDir) {
        if (currentDir == "UP" || currentDir == "DOWN") {
            if (curX != prevX) {
                drawLine(curX, curY, prevX, prevY);  // Left edge
                drawLine(curX + GRIDSIZE, curY, prevX + GRIDSIZE, prevY);  // Right edge
            }
        }
        else if (currentDir == "LEFT" || currentDir == "RIGHT") {
            if (curY != prevY) {
                drawLine(curX, curY, prevX, prevY);  // Top edge
                drawLine(curX, curY + GRIDSIZE, prevX, prevY + GRIDSIZE);  // Bottom edge
            }
        }
        return;
    }
    
    if (prevDir == "DOWN") {
        if (currentDir == "LEFT") {
            if (curY != prevY) {
                
                drawSquare(prevX, curY , abs(curX - prevX), abs(curY - prevY));
            }
        } else if (currentDir == "RIGHT") {
            if (curY != prevY) {
                
                drawSquare(curX + GRIDSIZE, curY, abs(curX - prevX), abs(curY - prevY));
            }   
        }
    }
    else if (prevDir == "UP") {
        if (currentDir == "LEFT") {
            if (curY != prevY) {
                
                drawSquare(prevX, prevY+ GRIDSIZE, abs(curX-prevX), abs(curY-prevY));
            }
        } else if (currentDir == "RIGHT") {
            if (curY != prevY) {
                
                drawSquare(curX + GRIDSIZE, prevY + GRIDSIZE, abs((curX+ GRIDSIZE)-(prevX + GRIDSIZE)), abs((curY + GRIDSIZE)-(prevY + GRIDSIZE)));

            }
        }
    }
    else if (prevDir == "LEFT") {
        if (currentDir == "UP") {
            if (curX != prevX) {
                
                drawSquare(prevX + GRIDSIZE, prevY,abs((curX+GRIDSIZE)-(prevX+GRIDSIZE)), abs(curY-prevY));
            }
        } else if (currentDir == "DOWN") {
            if (curX != prevX) {
                
                drawSquare(prevX + GRIDSIZE, curY + GRIDSIZE, abs((curX + GRIDSIZE)-(prevX + GRIDSIZE)), abs((curY + GRIDSIZE) - (prevY + GRIDSIZE)));
           }
        }
    }
    else if (prevDir == "RIGHT") {
        if (currentDir == "UP") {
            if (curX != prevX) { 
                
                drawSquare(curX, prevY, abs(curX - prevX), abs(curY - prevY));
            }
        } else if (currentDir == "DOWN") {
            if (curX != prevX) {
                
                drawSquare(curX, curY + GRIDSIZE, abs(curX-prevX), abs((curY + GRIDSIZE)- (prevY+GRIDSIZE)));
            }
        }
    }
}


// UI and Display
void Game::renderButton(const Button& button, Uint8 normalR, Uint8 normalG, Uint8 normalB, 
                       Uint8 hoverR, Uint8 hoverG, Uint8 hoverB) {
    if (button.isHovered) {
        SDL_SetRenderDrawColor(renderer, hoverR, hoverG, hoverB, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, normalR, normalG, normalB, 255);
    }
    SDL_RenderFillRect(renderer, &button.rect);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &button.rect);
    
    int letterSize = 40;  // Size of each letter
    int spacing = letterSize / 4;  // Space between letters
    int totalWidth = button.text.length() * (letterSize + spacing) - spacing;
    int textX = button.rect.x + (button.rect.w - totalWidth) / 2;
    int textY = button.rect.y + (button.rect.h - letterSize) / 2;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (size_t i = 0; i < button.text.length(); i++) {
        char c = button.text[i];
        if (c >= '0' && c <= '9') {
            drawDigit(c - '0', textX, textY, letterSize);
        } else if (c >= 'A' && c <= 'Z') {
            drawLetter(c, textX, textY, letterSize);
        }
        textX += letterSize + spacing;
    }
}
void Game::displayLoseScreen() {
    const int digitSize = 120;
    const int spacing = digitSize + 20;
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int centerX = screen_width / 2;
    int centerY = screen_height / 2 - digitSize;
    
    
    int numDigits = (score == 0) ? 1 : 0;
    int tempScore = score;
    while (tempScore > 0) {
        numDigits++;
        tempScore /= 10;
    }

    int startX = centerX - ((numDigits * spacing) / 2);

        
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    if (score == 0) {
        drawDigit(0, startX, centerY, digitSize);
    } else {
        int x = startX + (numDigits - 1) * spacing;
        tempScore = score;
        while (tempScore > 0) {
            int digit = tempScore % 10;
            drawDigit(digit, x, centerY, digitSize);
            tempScore /= 10;
            x -= spacing;
        }
    }
    
    
    renderButton(playAgainButton, 0, 150, 0, 0, 200, 0);  // Dark green normally, lighter green when hovered

    renderButton(quitButton, 150, 0, 0, 200, 0, 0);  // Dark red normally, lighter red when hovered


    SDL_RenderPresent(renderer);
}

void Game::updateWindowTitle() {
    std::string title = "Score: " + std::to_string(score);
    SDL_SetWindowTitle(window, title.c_str());
}



// Audio 
SDL_AudioDeviceID Game::audioDevice = 0;
void Game::playWav(const char* filename) {
        // Load WAV file
        SDL_AudioSpec wavSpec;
        Uint32 wavLength;
        Uint8 *wavBuffer;

        if (SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength) == nullptr) {
            std::cerr << "Could not load WAV: " << SDL_GetError() << std::endl;
            return;
        }

        // Clear any existing audio in the queue
        SDL_ClearQueuedAudio(audioDevice);

        // Queue and play the new audio
        SDL_QueueAudio(audioDevice, wavBuffer, wavLength);
        SDL_PauseAudioDevice(audioDevice, 0);

        // Calculate duration in milliseconds
        float duration = (float)wavLength / (wavSpec.freq * wavSpec.channels * (SDL_AUDIO_BITSIZE(wavSpec.format) / 8));
        int delayMs = (int)(duration * 1000);

        // Wait for just the duration of the sound
        SDL_Delay(delayMs);

        // Stop playback and clear the queue
        SDL_PauseAudioDevice(audioDevice, 1);
        SDL_ClearQueuedAudio(audioDevice);

        // Free the WAV buffer
        SDL_FreeWAV(wavBuffer);
    }
void Game::playWavInThread(const char* filename) {
        std::thread audioThread(&Game::playWav, this, filename);
        audioThread.detach();
    }
