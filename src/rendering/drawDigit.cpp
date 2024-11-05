#include "../core/Game.hpp"

void Game::drawDigit(int digit, int x, int y, int size) {
    const bool segments[10][7] = {
        {1,1,1,0,1,1,1}, // 0
        {0,0,1,0,0,1,0}, // 1
        {1,0,1,1,1,0,1}, // 2
        {1,0,1,1,0,1,1}, // 3
        {0,1,1,1,0,1,0}, // 4
        {1,1,0,1,0,1,1}, // 5
        {1,1,0,1,1,1,1}, // 6
        {1,0,1,0,0,1,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };

    int thickness = size / 6;  // Segment thickness
    int gap = thickness;   // Gap between segments
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color for segments
    
    SDL_Rect segment;
    
    if (segments[digit][0]) { // Top horizontal
        segment = {x + gap, y, size - 2*gap, thickness};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][1]) { // Top left vertical
        segment = {x, y + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][2]) { // Top right vertical
        segment = {x + size - thickness, y + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][3]) { // Middle horizontal
        segment = {x + gap, y + (size/2) - thickness/2, size - 2*gap, thickness};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][4]) { // Bottom left vertical
        segment = {x, y + (size/2) + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][5]) { // Bottom right vertical
        segment = {x + size - thickness, y + (size/2) + gap, thickness, (size/2) - gap};
        SDL_RenderFillRect(renderer, &segment);
    }
    
    if (segments[digit][6]) { // Bottom horizontal
        segment = {x + gap, y + size - thickness, size - 2*gap, thickness};
        SDL_RenderFillRect(renderer, &segment);
    }
}