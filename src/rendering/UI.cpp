#include "../core/Game.hpp"

// Renders a button with specific colors for normal and hover states
void Game::renderButton(const Button& button, 
                        Uint8 normalR, Uint8 normalG, Uint8 normalB, 
                        Uint8 hoverR, Uint8 hoverG, Uint8 hoverB) {
    // Set button color based on hover state
    if (button.isHovered) {
        SDL_SetRenderDrawColor(renderer, hoverR, hoverG, hoverB, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, normalR, normalG, normalB, 255);
    }
    SDL_RenderFillRect(renderer, &button.rect);
    
    // Draw button border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &button.rect);
    
    // Calculate text position
    int letterSize = 40;  // Size of each letter
    int spacing = letterSize / 4;  // Space between letters
    int totalWidth = button.text.length() * (letterSize + spacing) - spacing;
    int textX = button.rect.x + (button.rect.w - totalWidth) / 2;
    int textY = button.rect.y + (button.rect.h - letterSize) / 2;

    // Draw text on the button
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (size_t i = 0; i < button.text.length(); i++) {
        char c = button.text[i];
        if (c >= '0' && c <= '9') {
            drawDigit(c - '0', textX, textY, letterSize);
        } else if (c >= 'A' && c <= 'Z') {
            drawLetter(c, textX, textY, letterSize);
        }
        textX += letterSize + spacing;
    }
}

// Displays the lose screen with score and buttons
void Game::displayLoseScreen() {
    const int digitSize = 120;
    const int spacing = digitSize + 20;
    
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int centerX = screen_width / 2;
    int centerY = screen_height / 2 - digitSize;

    // Calculate number of digits in the score
    int numDigits = (score == 0) ? 1 : 0;
    int tempScore = score;
    while (tempScore > 0) {
        numDigits++;
        tempScore /= 10;
    }

    // Calculate starting x position for score digits
    int startX = centerX - ((numDigits * spacing) / 2);
        
    // Set color for score digits
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    // Draw score digits
    if (score == 0) {
        drawDigit(0, startX, centerY, digitSize);
    } else {
        int x = startX + (numDigits - 1) * spacing;
        tempScore = score;
        while (tempScore > 0) {
            int digit = tempScore % 10;
            drawDigit(digit, x, centerY, digitSize);
            tempScore /= 10;
            x -= spacing;
        }
    }
    
    // Render buttons
    renderButton(playAgainButton, 0, 150, 0, 0, 200, 0);  // Dark green normally, lighter green when hovered
    renderButton(quitButton, 150, 0, 0, 200, 0, 0);  // Dark red normally, lighter red when hovered

    // Present the rendered frame
    SDL_RenderPresent(renderer);
}