#include "../core/Game.hpp"

void Game::drawSquare(int x, int y, int width, int height) {
        SDL_Rect square;
        square.x = x;
        square.y = y;
        square.w = width;
        square.h = height;
        
        SDL_RenderFillRect(renderer, &square);
        SDL_RenderDrawRect(renderer, &square);
    }