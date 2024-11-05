#include "../core/Game.hpp"
void Game::addApple() { 
    food newApple = findPos();
    apple = newApple;
}
