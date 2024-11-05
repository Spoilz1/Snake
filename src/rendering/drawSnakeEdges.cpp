#include "../core/Game.hpp"

void Game::drawSnakeConnector(int i) {
    Direction currentDir = snakeBody[i].direction;
    Direction prevDir = snakeBody[i-1].direction;
    
    int curX = snakeBody[i].xPos;
    int curY = snakeBody[i].yPos;
    int prevX = snakeBody[i-1].xPos;
    int prevY = snakeBody[i-1].yPos;

    float gradientPosition = static_cast<float>(i) / (snakeLength * gradientSteps);
    
    int greenIntensity = 255 - static_cast<int>(gradientPosition * 155);
    greenIntensity = (greenIntensity < 100) ? 100 : greenIntensity;
    
    int blueIntensity = static_cast<int>(gradientPosition * 50);
    blueIntensity = (blueIntensity > 50) ? 50 : blueIntensity;
    
    // Enable blending for anti-aliasing
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Main color
    SDL_SetRenderDrawColor(renderer, 0, greenIntensity, blueIntensity, 255);
    
    // Helper function to draw anti-aliased connector
    auto drawConnector = [&](int x, int y, int w, int h) {
        // Draw main connector
        SDL_Rect mainRect = {x, y, w, h};
        SDL_RenderFillRect(renderer, &mainRect);
        
        // Draw anti-aliased edges
        SDL_SetRenderDrawColor(renderer, 0, greenIntensity, blueIntensity, 128);
        
        // Outer edge
        SDL_Rect edgeRect = {x-1, y-1, w+2, h+2};
        SDL_RenderDrawRect(renderer, &edgeRect);
        
        // Reset color for next draw
        SDL_SetRenderDrawColor(renderer, 0, greenIntensity, blueIntensity, 255);
    };
    
    if (prevDir == Direction::DOWN) {
        if (currentDir == Direction::LEFT) {
            if (curY != prevY) {
                drawConnector(prevX, curY, abs(curX - prevX), abs(curY - prevY));
            }
        } else if (currentDir == Direction::RIGHT) {
            if (curY != prevY) {
                drawConnector(curX + GRIDSIZE, curY, abs(curX - prevX), abs(curY - prevY));
            }   
        }
    }
    else if (prevDir == Direction::UP) {
        if (currentDir == Direction::LEFT) {
            if (curY != prevY) {
                drawConnector(prevX, prevY + GRIDSIZE, abs(curX-prevX), abs(curY-prevY));
            }
        } else if (currentDir == Direction::RIGHT) {
            if (curY != prevY) {
                drawConnector(curX + GRIDSIZE, prevY + GRIDSIZE, 
                          abs((curX + GRIDSIZE)-(prevX + GRIDSIZE)), 
                          abs((curY + GRIDSIZE)-(prevY + GRIDSIZE)));
            }
        }
    }
    else if (prevDir == Direction::LEFT) {
        if (currentDir == Direction::UP) {
            if (curX != prevX) {
                drawConnector(prevX + GRIDSIZE, prevY, 
                          abs((curX+GRIDSIZE)-(prevX+GRIDSIZE)), 
                          abs(curY-prevY));
            }
        } else if (currentDir == Direction::DOWN) {
            if (curX != prevX) {
                drawConnector(prevX + GRIDSIZE, curY + GRIDSIZE, 
                          abs((curX + GRIDSIZE)-(prevX + GRIDSIZE)), 
                          abs((curY + GRIDSIZE) - (prevY + GRIDSIZE)));
            }
        }
    }
    else if (prevDir == Direction::RIGHT) {
        if (currentDir == Direction::UP) {
            if (curX != prevX) { 
                drawConnector(curX, prevY, abs(curX - prevX), abs(curY - prevY));
            }
        } else if (currentDir == Direction::DOWN) {
            if (curX != prevX) {
                drawConnector(curX, curY + GRIDSIZE, abs(curX-prevX), 
                          abs((curY + GRIDSIZE)- (prevY+GRIDSIZE)));
            }
        }
    }
    
    // Reset blend mode after drawing
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}