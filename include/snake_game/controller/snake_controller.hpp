#pragma once
#include "../model/snake.hpp"
#include "graphics_render/input.hpp"

/**
 * SNAKE CONTROLLER
 * Purpose: Maps keyboard input to snake direction changes.
 */
struct SnakeController {
    void handle_input(Snake& snake) {
        if (Keys::pressed(SDLK_W) || Keys::pressed(SDLK_UP))    snake.set_direction(Snake::Direction::UP);
        if (Keys::pressed(SDLK_S) || Keys::pressed(SDLK_DOWN))  snake.set_direction(Snake::Direction::DOWN);
        if (Keys::pressed(SDLK_A) || Keys::pressed(SDLK_LEFT))  snake.set_direction(Snake::Direction::LEFT);
        if (Keys::pressed(SDLK_D) || Keys::pressed(SDLK_RIGHT)) snake.set_direction(Snake::Direction::RIGHT);
    }
};
