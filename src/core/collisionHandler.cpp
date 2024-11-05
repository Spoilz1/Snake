#include "Game.hpp"
#include <cmath>  

bool Game::collisions() { 
    const float tolerance = GRIDSIZE / 5.0f;
    const auto& snakeHead = snakeBody[0];

    // Check collision with wall
    if (snakeHead.xPos < 0 || snakeHead.xPos >= screen_width || 
        snakeHead.yPos < 0 || snakeHead.yPos >= screen_height) {
        return true;
    }

    // Check collision with self
    for (int i = 1; i < snakeLength; ++i) {
        if (std::abs(snakeHead.xPos - snakeBody[i].xPos) <= tolerance &&
            std::abs(snakeHead.yPos - snakeBody[i].yPos) <= tolerance) {
            return true;
        }
    }

    // Check collision with apple
    if (std::abs(snakeHead.xPos - apple.xPos) <= tolerance &&
        std::abs(snakeHead.yPos - apple.yPos) <= tolerance) {
        score++;
        playWavInThread("../assets/eat.wav");
        addApple();
        addSnake();
    }

    // Check win condition
    if (snakeLength >= total_grids) {
        endGame();
    }

    return false;
}