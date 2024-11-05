#include "../core/Game.hpp"
#include <random>

void Game::Clean() {
    cleanupAudio();
    cleanupOpenGL();

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
    isRunning = false;
}

void Game::cleanupOpenGL() {
    if (glContext) {
        SDL_GL_DeleteContext(glContext);
        glContext = nullptr;
    }
}

void Game::sleep(int ms) {
    SDL_Delay(ms);
}

bool Game::isPointInRect(int x, int y, const SDL_Rect& rect) {
    return (x >= rect.x && x < rect.x + rect.w &&
            y >= rect.y && y < rect.y + rect.h);
}

food Game::findPos() {
    std::random_device rd;
    std::mt19937 gen(rd());

    int grid_cols = (screen_width - GRIDSIZE) / GRIDSIZE;
    int grid_rows = (screen_height - GRIDSIZE) / GRIDSIZE;

    std::uniform_int_distribution<> width_dist(0, grid_cols - 1);
    std::uniform_int_distribution<> height_dist(0, grid_rows - 1);

    food newPos;
    bool validPosition = false;
    const float tolerance = (GRIDSIZE / 5) * 1.0f;

    while (!validPosition) {
        newPos.xPos = width_dist(gen) * GRIDSIZE;
        newPos.yPos = height_dist(gen) * GRIDSIZE;

        validPosition = true;

        for (int i = 0; i < snakeLength; i++) {
            if (newPos.xPos >= snakeBody[i].xPos - tolerance &&
                newPos.xPos <= snakeBody[i].xPos + tolerance &&
                newPos.yPos >= snakeBody[i].yPos - tolerance &&
                newPos.yPos <= snakeBody[i].yPos + tolerance) {
                validPosition = false;
                break;
            }
        }

        if (validPosition) {
            newPos.apple.x = newPos.xPos;
            newPos.apple.y = newPos.yPos;
            newPos.apple.w = GRIDSIZE;
            newPos.apple.h = GRIDSIZE;
        }
    }

    return newPos;
}

void Game::updateWindowTitle() {
    std::string title = "Score: " + std::to_string(score);
    SDL_SetWindowTitle(window, title.c_str());
}