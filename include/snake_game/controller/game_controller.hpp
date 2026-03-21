#pragma once
#include "../model/snake.hpp"
#include "../model/board.hpp"
#include "../model/food.hpp"
#include "graphics_render/input.hpp"

/**
 * GAME CONTROLLER
 * Purpose: Manages high-level game states (start/restart) and UI interactions.
 */
struct GameController {
    void handle_input(bool& waiting_to_start, Snake& snake, const Board& board, Food& food) {
        // Restart with 'R'
        if (Keys::pressed(SDLK_R)) {
            snake.init(board.width / 4, board.height / 4);
            food.spawn(board, snake);
            waiting_to_start = false;
        }

        // Button logic
        if (!snake.alive || waiting_to_start) {
            auto [mx, my] = Mouse::position();
            bool is_hovering = (mx >= 401 && mx <= 680 && my >= 941 && my <= 1033);

            if (is_hovering) {
                SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
                SDL_SetCursor(cursor);
            } else {
                SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
                SDL_SetCursor(cursor);
            }

            if (is_hovering && Mouse::pressed(Mouse::ids::left)) {
                SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
                SDL_SetCursor(cursor);
                
                if (waiting_to_start) {
                    waiting_to_start = false;
                } else if (!snake.alive) {
                    snake.init(board.width / 4, board.height / 4);
                    food.spawn(board, snake);
                }
            }
        }
    }
};
