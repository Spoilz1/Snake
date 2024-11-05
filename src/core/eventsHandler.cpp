#include "../core/Game.hpp"

void Game::HandleEvents() {
    SDL_Event event;

    // Process events while there are events in the queue
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:  // Handle window close event
                isRunning = false;
                break;

            case SDL_MOUSEMOTION:  // Handle mouse movement
                if (gameOver) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;

                    // Check if mouse is hovering over buttons
                    playAgainButton.isHovered = isPointInRect(mouseX, mouseY, playAgainButton.rect);
                    quitButton.isHovered = isPointInRect(mouseX, mouseY, quitButton.rect);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:  // Handle mouse button press
                if (gameOver && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    // Check which button was clicked
                    if (isPointInRect(mouseX, mouseY, playAgainButton.rect)) {
                        resetGame();  // Reset the game
                    } else if (isPointInRect(mouseX, mouseY, quitButton.rect)) {
                        isRunning = false;  // Quit the game
                    }
                }
                break;

            case SDL_KEYDOWN:  // Handle key press events
                if (gameOver) {
                    switch (event.key.keysym.sym) {
                        case SDLK_RETURN:  // Enter key to play again
                            resetGame();
                            break;
                        case SDLK_ESCAPE:  // Escape key to quit
                            isRunning = false;
                            break;
                    }
                }
                break;
        }
    }
}