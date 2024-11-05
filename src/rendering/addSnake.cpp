#include "../core/Game.hpp"

void Game::addSnake() {
    SDL_Rect square;
    squares[snakeLength] = square;
    
    int baseX = ((snakeBody[snakeLength-1].xPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
    int baseY = ((snakeBody[snakeLength-1].yPos + GRIDSIZE/2) / GRIDSIZE) * GRIDSIZE;
    
    switch (snakeBody[snakeLength-1].direction) {
        case Direction::UP:
            snakeBody[snakeLength].yPos = baseY + GRIDSIZE;
            snakeBody[snakeLength].xPos = baseX;
            snakeBody[snakeLength].last_x = baseX;
            snakeBody[snakeLength].last_y = baseY + GRIDSIZE;
            break;
            
        case Direction::DOWN:
            snakeBody[snakeLength].yPos = baseY - GRIDSIZE;
            snakeBody[snakeLength].xPos = baseX;
            snakeBody[snakeLength].last_x = baseX;
            snakeBody[snakeLength].last_y = baseY - GRIDSIZE;
            break;
            
        case Direction::LEFT:
            snakeBody[snakeLength].yPos = baseY;
            snakeBody[snakeLength].xPos = baseX + GRIDSIZE;
            snakeBody[snakeLength].last_x = baseX + GRIDSIZE;
            snakeBody[snakeLength].last_y = baseY;
            break;
            
        case Direction::RIGHT:
            snakeBody[snakeLength].yPos = baseY;
            snakeBody[snakeLength].xPos = baseX - GRIDSIZE;
            snakeBody[snakeLength].last_x = baseX - GRIDSIZE;
            snakeBody[snakeLength].last_y = baseY;
            break;
    }
    
    snakeBody[snakeLength].direction = snakeBody[snakeLength-1].direction;
    snakeBody[snakeLength].queue_direction = snakeBody[snakeLength-1].direction;
    
    snakeLength++;
}