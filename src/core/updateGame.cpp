#include "../core/Game.hpp"


Game::Game() : 
    window(nullptr), 
    renderer(nullptr), 
    isRunning(false), 
    gameOver(false),
    currentTime(0), 
    lastTime(0), 
    deltaTime(0) {}


Game::~Game() {
    cleanupTextures();
    Clean();
}

// Main update function
void Game::Update() {
    Direction new_direction = checkInput();
    updateSnakeDirection(new_direction);
    
    float frameSpeed = calculateFrameSpeed();
    bool headReachedGrid = updateSnakeHead(frameSpeed);
    
    if (headReachedGrid) {
        alignSnakeToGrid();
        updateBodyDirections();
    }
    
    updateSnakeBody(frameSpeed);
    
    if (collisions()) {
        endGame();
    }
}

// Snake direction update
void Game::updateSnakeDirection(Direction new_direction) {

    if (snakeBody[0].direction != new_direction) {
        snakeBody[0].queue_direction = new_direction;

        if (abs(snakeBody[1].xPos - snakeBody[1].last_x) >= GRIDSIZE/1.5 || abs(snakeBody[1].yPos - snakeBody[1].last_y) >= GRIDSIZE/1.5) {
            switch (new_direction) {
                case Direction::DOWN:
                    snakeBody[0].xPos = snakeBody[0].last_x;
                    snakeBody[0].yPos = snakeBody[1].yPos + GRIDSIZE;
                    snakeBody[0].direction = new_direction;
                    break;
                
                case Direction::UP:
                    snakeBody[0].xPos = snakeBody[0].last_x;
                    snakeBody[0].yPos = snakeBody[1].yPos - GRIDSIZE;
                    snakeBody[0].direction = new_direction;
                    break;
                
                case Direction::LEFT:
                    snakeBody[0].yPos = snakeBody[0].last_y;
                    snakeBody[0].xPos = snakeBody[1].xPos - GRIDSIZE;
                    snakeBody[0].direction = new_direction;
                    break;
                
                case Direction::RIGHT:
                    snakeBody[0].yPos = snakeBody[0].last_y;
                    snakeBody[0].xPos = snakeBody[1].xPos + GRIDSIZE;
                    snakeBody[0].direction = new_direction;
                    break;
            }
        }

    } 
}

// Frame speed calculation
float Game::calculateFrameSpeed() {
    currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    
    // Limit delta time to prevent large jumps
    if (deltaTime > 0.1f) {
        deltaTime = 0.1f;
    }
    
    return BASE_MOVE_SPEED * deltaTime;
}

// Update snake head position
bool Game::updateSnakeHead(float frameSpeed) {
    bool headReachedGrid = false;
    snake& head = snakeBody[0];
    
    switch (head.direction) {
        case Direction::UP:
            head.yPos -= frameSpeed;
            if (head.yPos <= head.last_y - GRIDSIZE) {
                head.yPos = head.last_y - GRIDSIZE;
                headReachedGrid = true;
            }
            break;
            
        case Direction::DOWN:
            head.yPos += frameSpeed;
            if (head.yPos >= head.last_y + GRIDSIZE) {
                head.yPos = head.last_y + GRIDSIZE;
                headReachedGrid = true;
            }
            break;
            
        case Direction::LEFT:
            head.xPos -= frameSpeed;
            if (head.xPos <= head.last_x - GRIDSIZE) {
                head.xPos = head.last_x - GRIDSIZE;
                headReachedGrid = true;
            }
            break;
            
        case Direction::RIGHT:
            head.xPos += frameSpeed;
            if (head.xPos >= head.last_x + GRIDSIZE) {
                head.xPos = head.last_x + GRIDSIZE;
                headReachedGrid = true;
            }
            break;
    }
    
    return headReachedGrid;
}

// Align the snake to the grid
void Game::alignSnakeToGrid() {
    snake& head = snakeBody[0];
    head.direction = head.queue_direction;
    snapToGrid(head);
    head.last_x = head.xPos;
    head.last_y = head.yPos;
}

// Snap segment to grid
void Game::snapToGrid(snake& segment) {
    segment.xPos = ((segment.xPos + GRIDSIZE / 2) / GRIDSIZE) * GRIDSIZE;
    segment.yPos = ((segment.yPos + GRIDSIZE / 2) / GRIDSIZE) * GRIDSIZE;
}

// Update directions of the snake body
void Game::updateBodyDirections() {
    for (int i = 1; i < snakeLength; i++) {
        snapToGrid(snakeBody[i]);
        
        int current_x = snakeBody[i].xPos;
        int current_y = snakeBody[i].yPos;
        
        int target_x = snakeBody[i - 1].last_x;
        int target_y = snakeBody[i - 1].last_y;
        
        updateSegmentDirection(snakeBody[i], current_x, current_y, target_x, target_y);
        
        snakeBody[i].last_x = current_x;
        snakeBody[i].last_y = current_y;
        snakeBody[i].queue_direction = snakeBody[i].direction;
    }
}

// Update individual segment direction
void Game::updateSegmentDirection(snake& segment, int current_x, int current_y, int target_x, int target_y) {
    if (target_x < current_x) 
        segment.direction = Direction::LEFT;
    else if (target_x > current_x) 
        segment.direction = Direction::RIGHT;
    else if (target_y < current_y) 
        segment.direction = Direction::UP;
    else if (target_y > current_y) 
        segment.direction = Direction::DOWN;
}

// Update the snake body positions
void Game::updateSnakeBody(float frameSpeed) {
    for (int i = 1; i < snakeLength; i++) {
        float targetX = snakeBody[i - 1].last_x;
        float targetY = snakeBody[i - 1].last_y;
        
        updateSegmentPosition(snakeBody[i], targetX, targetY, frameSpeed);
    }
}

// Update individual segment position
void Game::updateSegmentPosition(snake& segment, float targetX, float targetY, float frameSpeed) {
    switch (segment.direction) {
        case Direction::UP:
            segment.yPos = std::max(targetY, segment.yPos - frameSpeed);
            segment.xPos = ((segment.xPos + GRIDSIZE / 2) / GRIDSIZE) * GRIDSIZE;
            break;
            
        case Direction::DOWN:
            segment.yPos = std::min(targetY, segment.yPos + frameSpeed);
            segment.xPos = ((segment.xPos + GRIDSIZE / 2) / GRIDSIZE) * GRIDSIZE;
            break;
            
        case Direction::LEFT:
            segment.xPos = std::max(targetX, segment.xPos - frameSpeed);
            segment.yPos = ((segment.yPos + GRIDSIZE / 2) / GRIDSIZE) * GRIDSIZE;
            break;
            
        case Direction::RIGHT:
            segment.xPos = std::min(targetX, segment.xPos + frameSpeed);
            segment.yPos = ((segment.yPos + GRIDSIZE / 2) / GRIDSIZE) * GRIDSIZE;
            break;
    }
}
