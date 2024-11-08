#include "Game.hpp"
#include <cmath>  

bool Game::collisions() { 
    const float tolerance = GRIDSIZE / 1.0f;  // Allowable margin for collision detection
    const auto& snakeHead = snakeBody[0];  // Reference to the snake's head position

    // Check collision with walls (screen boundaries)
    if (snakeHead.xPos < -GRIDSIZE || snakeHead.xPos >= screen_width || 
        snakeHead.yPos < -GRIDSIZE || snakeHead.yPos >= screen_height) {
        return true;  // Collision detected
    }

    // Check collision with self (snake body)
    for (int i = 2; i < snakeLength; ++i) {
        if (std::abs(snakeBody[0].xPos - snakeBody[i].xPos) <= tolerance &&
            std::abs(snakeBody[0].yPos - snakeBody[i].yPos) <= tolerance) {
            return true;  // Collision detected
        }
    }

    // Check collision with apple (food)
    if (std::abs(snakeBody[1].xPos - apple.xPos) <= tolerance &&
        std::abs(snakeBody[1].yPos - apple.yPos) <= tolerance) {
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
