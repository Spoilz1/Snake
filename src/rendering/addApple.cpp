#include "../core/Game.hpp"
#include <random>

void Game::addApple() { 
    food newApple = findPos();
    apple = newApple;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> color_var(0.8, 1.0);
    static std::uniform_real_distribution<> highlight_pos(0.2, 0.4);
    
    currentGradient.colorVariation = color_var(gen);
    currentGradient.highlightX = highlight_pos(gen);
    currentGradient.highlightY = highlight_pos(gen);
    
    addSnake();
}

