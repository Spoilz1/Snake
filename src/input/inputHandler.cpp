#include "../core/Game.hpp"

Direction Game::checkInput() { 
    const Uint8* state = SDL_GetKeyboardState(NULL);
    Direction current_direction = snakeBody[0].direction;
    Direction new_direction = current_direction;

    // Check input keys
    bool up = (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]);
    bool down = (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]);
    bool left = (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]);
    bool right = (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]);
    
    // If no input, return current direction
    if (!up && !down && !left && !right) {
        return current_direction;
    }

    // Directional movement checks
    bool moving_vertical = (current_direction == Direction::UP || current_direction == Direction::DOWN);
    bool moving_horizontal = (current_direction == Direction::LEFT || current_direction == Direction::RIGHT);
    
    // Edge checks
    bool at_right_edge = (snakeBody[0].xPos >= screen_width - GRIDSIZE);
    bool at_left_edge = (snakeBody[0].xPos < GRIDSIZE);
    bool at_top_edge = (snakeBody[0].yPos < GRIDSIZE);
    bool at_bottom_edge = (snakeBody[0].yPos >= screen_height - GRIDSIZE);
    bool near_edge = at_right_edge || at_left_edge || at_top_edge || at_bottom_edge;

    if (!near_edge) {  // Use priority system when not near edge
        if (moving_vertical) {
            if (left && current_direction != Direction::RIGHT) {
                new_direction = Direction::LEFT;
            } else if (right && current_direction != Direction::LEFT) {
                new_direction = Direction::RIGHT;
            } else if (up && current_direction != Direction::DOWN) {
                new_direction = Direction::UP;
            } else if (down && current_direction != Direction::UP) {
                new_direction = Direction::DOWN;
            }
        } else if (moving_horizontal) {
            if (up && current_direction != Direction::DOWN) {
                new_direction = Direction::UP;
            } else if (down && current_direction != Direction::UP) {
                new_direction = Direction::DOWN;
            } else if (left && current_direction != Direction::RIGHT) {
                new_direction = Direction::LEFT;
            } else if (right && current_direction != Direction::LEFT) {
                new_direction = Direction::RIGHT;
            }
        }
    } else {  // Handle edge cases
        if (at_right_edge) {
            if (up && current_direction != Direction::DOWN) {
                new_direction = Direction::UP;
            } else if (down && current_direction != Direction::UP) {
                new_direction = Direction::DOWN;
            } else if (left && current_direction != Direction::RIGHT) {
                new_direction = Direction::LEFT;
            } else if (right && current_direction != Direction::LEFT) {
                new_direction = Direction::RIGHT;
            }
        } else if (at_left_edge) {
            if (up && current_direction != Direction::DOWN) {
                new_direction = Direction::UP;
            } else if (down && current_direction != Direction::UP) {
                new_direction = Direction::DOWN;
            } else if (right && current_direction != Direction::LEFT) {
                new_direction = Direction::RIGHT;
            } else if (left && current_direction != Direction::RIGHT) {
                new_direction = Direction::LEFT;
            }
        } else if (at_top_edge) {
            if (left && current_direction != Direction::RIGHT) {
                new_direction = Direction::LEFT;
            } else if (right && current_direction != Direction::LEFT) {
                new_direction = Direction::RIGHT;
            } else if (down && current_direction != Direction::UP) {
                new_direction = Direction::DOWN;
            } else if (up && current_direction != Direction::DOWN) {
                new_direction = Direction::UP;
            }
        } else if (at_bottom_edge) {
            if (left && current_direction != Direction::RIGHT) {
                new_direction = Direction::LEFT;
            } else if (right && current_direction != Direction::LEFT) {
                new_direction = Direction::RIGHT;
            } else if (up && current_direction != Direction::DOWN) {
                new_direction = Direction::UP;
            } else if (down && current_direction != Direction::UP) {
                new_direction = Direction::DOWN;
            }
        }
    }

    return new_direction;
}