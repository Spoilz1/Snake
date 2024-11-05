#include "../core/Game.hpp"

void Game::Render() {
    if (gameOver) {
        displayLoseScreen();
        return;
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set blend mode for anti-aliasing
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Initialize if needed
    if (snakeLength == 1) { 
        addApple();
        addSnake();
    }

    // Render apple with anti-aliased edges
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    // Draw apple with smoothed edges
    int x = apple.apple.x;
    int y = apple.apple.y;
    int w = apple.apple.w;
    int h = apple.apple.h;
    
    // Draw main apple body
    SDL_RenderFillRect(renderer, &apple.apple);
    
    // Add anti-aliased edges
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);
    SDL_Rect edgeRect = {x-1, y-1, w+2, h+2};
    SDL_RenderDrawRect(renderer, &edgeRect);

    // Render snake body with anti-aliasing
    for (int i = 0; i < snakeLength; i++) {
        float gradientPosition = static_cast<float>(i) / (snakeLength * gradientSteps);
        int greenIntensity = std::max(100, 255 - static_cast<int>(gradientPosition * 155));
        int blueIntensity = std::min(50, static_cast<int>(gradientPosition * 50));

        // Main body
        SDL_SetRenderDrawColor(renderer, 0, greenIntensity, blueIntensity, 255);
        squares[i] = { snakeBody[i].xPos, snakeBody[i].yPos, GRIDSIZE, GRIDSIZE };
        SDL_RenderFillRect(renderer, &squares[i]);

        // Anti-aliased edges
        SDL_SetRenderDrawColor(renderer, 0, greenIntensity, blueIntensity, 128);
        SDL_Rect edgeRect = {
            snakeBody[i].xPos - 1,
            snakeBody[i].yPos - 1,
            GRIDSIZE + 2,
            GRIDSIZE + 2
        };
        SDL_RenderDrawRect(renderer, &edgeRect);
    }

    // Draw connectors with anti-aliasing
    for (int i = snakeLength - 1; i > 0; i--) {
        drawSnakeConnector(i);
    }

    updateWindowTitle();
    SDL_RenderPresent(renderer);
}