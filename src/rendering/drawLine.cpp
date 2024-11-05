#include "../core/Game.hpp"
void Game::drawLine(int x1, int y1, int x2, int y2) { 
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}