#include "../include/game.hpp"

int main(int argc, char* argv[]) {
    Game game;
    
    game.Initialize("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, false);
    
    while (game.IsRunning()) {
        game.HandleEvents();
        game.Update();
        game.Render();
        game.sleep(16);
    }
    
    game.Clean();
    return 0;
}