#include "Game.hpp"


int main(int argc, char* argv[]) {
    Game game;
    
    game.Initialize("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, false);
    
    while (game.IsRunning()) {
        game.HandleEvents();
        if(!game.isPaused()) { 
            game.Update();
        }
        game.Render();
        game.sleep(8);
    }
    
    game.Clean();
    return 0;
}
