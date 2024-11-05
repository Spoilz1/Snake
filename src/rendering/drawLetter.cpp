#include "../core/Game.hpp"
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
