#include "Game.hpp"
#include <cmath>  

bool Game::collisions() { 
    const float tolerance = GRIDSIZE / 5.0f;  // Allowable margin for collision detection
    const auto& snakeHead = snakeBody[0];  // Reference to the snake's head position

    // Check collision with walls (screen boundaries)
    if (snakeHead.xPos < 0 || snakeHead.xPos >= screen_width || 
        snakeHead.yPos < 0 || snakeHead.yPos >= screen_height) {
        return true;  // Collision detected
    }

    // Check collision with self (snake body)
    for (int i = 1; i < snakeLength; ++i) {
        if (std::abs(snakeHead.xPos - snakeBody[i].xPos) <= tolerance &&
            std::abs(snakeHead.yPos - snakeBody[i].yPos) <= tolerance) {
            return true;  // Collision detected
        }
    }

    // Check collision with apple (food)
    if (std::abs(snakeHead.xPos - apple.xPos) <= tolerance &&
        std::abs(snakeHead.yPos - apple.yPos) <= tolerance) {
        score++;  // Increase score upon eating an apple
        playWavInThread("../assets/eat.wav");  // Play eating sound
        addApple();  // Spawn a new apple
    }

    // Check win condition (if snake length reaches total grids)
    if (snakeLength >= total_grids) {
        endGame();  // End the game if the win condition is met
    }

    return false;  // No collision detected
}