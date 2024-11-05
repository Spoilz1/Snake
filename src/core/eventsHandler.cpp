#include "../core/Game.hpp"

void Game::HandleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            
            case SDL_MOUSEMOTION:
                if (gameOver) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;

                    playAgainButton.isHovered = isPointInRect(mouseX, mouseY, playAgainButton.rect);
                    quitButton.isHovered = isPointInRect(mouseX, mouseY, quitButton.rect);
                }
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                if (gameOver && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if (isPointInRect(mouseX, mouseY, playAgainButton.rect)) {
                        resetGame();
                    } else if (isPointInRect(mouseX, mouseY, quitButton.rect)) {
                        isRunning = false;
                    }
                }
                break;
            
            case SDL_KEYDOWN:
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