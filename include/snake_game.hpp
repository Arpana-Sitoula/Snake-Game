#pragma once
#include "game_logic/board.hpp"
#include "game_logic/snake.hpp"
#include "game_logic/food.hpp"
#include "graphics_render/model.hpp"
#include "graphics_render/input.hpp"
#include "graphics_render/score_board.hpp"

// Forward declare Pipeline (we only use it as a reference parameter)
struct Pipeline;

#include <iostream>

/**
 * SNAKE GAME
 * Purpose: Connects the raw game logic to the graphics engine to make a playable game.
 * 
 * Flow:
 * - Owns the logic objects: Board, Snake, Food.
 * - Owns the visual objects: A reusable `cell_model` to draw the grid squares.
 * - init(): Spawns the snake and the first food.
 * - handle_input(): Listens to W/A/S/D or Arrows to change the snake's direction.
 * - update(): Moves the snake, checks if food was eaten, handles growing/score.
 * - draw(): Iterates through the board, snake body, and food, drawing each piece.
 */
struct SnakeGame {
    Board board;
    Snake snake;
    Food food;
    Model cell_model;  // reused to draw all cells
    
    void init() {
        cell_model.init();
        snake.init(board.width / 4, board.height / 4);
        food.spawn(board, snake);
    }
    
    void destroy() {
        cell_model.destroy();
    }
    
    // Handle keyboard input
    void handle_input() {
        if (Keys::pressed(SDLK_W) || Keys::pressed(SDLK_UP))    snake.set_direction(Snake::Direction::UP);
        if (Keys::pressed(SDLK_S) || Keys::pressed(SDLK_DOWN))  snake.set_direction(Snake::Direction::DOWN);
        if (Keys::pressed(SDLK_A) || Keys::pressed(SDLK_LEFT))  snake.set_direction(Snake::Direction::LEFT);
        if (Keys::pressed(SDLK_D) || Keys::pressed(SDLK_RIGHT)) snake.set_direction(Snake::Direction::RIGHT);
        
        if (Keys::pressed(SDLK_R)) {
            snake.init(board.width / 4, board.height / 4);
            food.spawn(board, snake);
        }

        // Handle Mouse Click and Hover on Restart Button
        if (!snake.alive) {
            auto [mx, my] = Mouse::position();

            // Based on user prints, the button bounds in Screen Space are roughly:
            // X: [-40, 370]
            // Y: [-130, -40] (in SDL, Y goes down, but since we had negatives let's just use raw values)
            bool is_hovering = (mx >= -45 && mx <= 375 && my >= -135 && my <= -35);

            if (is_hovering) {
                SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
                SDL_SetCursor(cursor);
            } else {
                SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
                SDL_SetCursor(cursor);
            }

            if (is_hovering && Mouse::pressed(Mouse::ids::left)) {
                // Reset cursor before restarting
                SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
                SDL_SetCursor(cursor);
                
                snake.init(board.width / 4, board.height / 4);
                food.spawn(board, snake);
            }
        }
    }
    
    // Update game state
    void update(float delta) {
        snake.update(delta, board);
        
        if (food.check_eaten(snake)) {
            snake.grow();
            food.spawn(board, snake);
        }
    }
    
    // Draw everything 
    void draw(Pipeline& pipeline) {
        float cell_scale = board.cell_size;
        
        // 1. Draw board

        for (int x = 0; x < board.width; x++) {
            for (int y = 0; y < board.height; y++) {
                glm::vec3 pos = board.grid_to_world(x, y);
                cell_model.set_position(pos.x, pos.y);
                cell_model.set_scale(cell_scale);
                pipeline.set_color(glm::vec4(0.988f, 0.835f, 0.808f, 1.0f));
                cell_model.draw();
            }
        }
        
        // 1.5 Draw border 
        float total_w = board.width * board.cell_size;
        float total_h = board.height * board.cell_size;
        float border_thickness = 0.34f; // thin border
        
        pipeline.set_color(glm::vec4(0.906f, 0.561f, 0.557f, 1.0f)); 
        
        // Top edge
        cell_model.transform._position = glm::vec3(0, total_h / 2.0f + border_thickness / 2.0f, 0);
        cell_model.transform._scale = glm::vec3(total_w + border_thickness * 2, border_thickness, 1);
        cell_model.draw();
        
        // Bottom edge
        cell_model.transform._position = glm::vec3(0, -total_h / 2.0f - border_thickness / 2.0f, 0);
        cell_model.transform._scale = glm::vec3(total_w + border_thickness * 2, border_thickness, 1);
        cell_model.draw();
        
        // Left edge
        cell_model.transform._position = glm::vec3(-total_w / 2.0f - border_thickness / 2.0f, 0, 0);
        cell_model.transform._scale = glm::vec3(border_thickness, total_h, 1);
        cell_model.draw();

        // Right edge
        cell_model.transform._position = glm::vec3(total_w / 2.0f + border_thickness / 2.0f, 0, 0);
        cell_model.transform._scale = glm::vec3(border_thickness, total_h, 1);
        cell_model.draw();
        
        // 2. Draw snake
        int i = 0;
        for (const auto& seg : snake.body) {
            glm::vec3 pos = board.grid_to_world(seg.x, seg.y);
            cell_model.set_position(pos.x, pos.y);
            cell_model.set_scale(cell_scale);
            
            if (i == 0) {
                pipeline.set_color(snake.alive ? glm::vec4(0.349f, 0.349f, 1.0f, 1.0f) 
                                               : glm::vec4(0.400f, 0.400f, 0.980f, 1.0f));
            } else {
                pipeline.set_color(snake.alive ? glm::vec4(0.576f, 0.506f, 1.0f, 1.0f) 
                                               : glm::vec4(0.722f, 0.722f, 1.0f, 1.0f));
            }
            cell_model.draw();
            i++;
        }
        
        // 3. Draw food
        glm::vec3 food_pos = board.grid_to_world(food.position.x, food.position.y);
        cell_model.set_position(food_pos.x, food_pos.y);
        cell_model.set_scale(cell_scale * 0.8f);
        pipeline.set_color(glm::vec4(0.996f, 0.365f, 0.624f, 1.0f));
        cell_model.draw();

        // 4. Draw scoreboard background
        float board_top = total_h / 2.0f;
        float score_bg_width = 1.6f;
        float score_bg_height = 0.6f;
        float score_bg_y = board_top + border_thickness + score_bg_height / 2.0f + 0.1f;

        cell_model.transform._position = glm::vec3(0, score_bg_y, 0);
        cell_model.transform._scale = glm::vec3(score_bg_width, score_bg_height, 1);
        pipeline.set_color(glm::vec4(0.996f, 0.784f, 0.604f, 1.0f));
        cell_model.draw();

        // 5. Draw score digits
        int s = snake.score;
        int d1 = s / 100;
        int d2 = (s / 10) % 10;
        int d3 = s % 10;

        float digit_scale = 0.2f;
        float digit_thick = 0.05f;
        float spacing = 0.4f;
        pipeline.set_color(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)); // Dark text color

        DigitRenderer::draw(d1, -spacing, score_bg_y, digit_scale, digit_thick, cell_model);
        DigitRenderer::draw(d2, 0.0f,     score_bg_y, digit_scale, digit_thick, cell_model);
        DigitRenderer::draw(d3, spacing,  score_bg_y, digit_scale, digit_thick, cell_model);

        // 6. Draw Restart Button (Only if dead)
        if (!snake.alive) {
            float restart_width = 3.6f;
            float restart_height = 0.8f;
            float restart_y = -board_top - border_thickness - restart_height / 2.0f - 0.2f;

            // Background
            cell_model.transform._position = glm::vec3(0, restart_y, 0);
            cell_model.transform._scale = glm::vec3(restart_width, restart_height, 1);
            pipeline.set_color(glm::vec4(1.0f, 0.42f, 0.42f, 1.0f));
            cell_model.draw();

            // Word 'RESTART'
            pipeline.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // White text
            float let_scale = 0.15f;
            float let_thick = 0.04f;
            float let_space = 0.4f;
            float start_x = -let_space * 3.0f;

            DigitRenderer::draw_r(start_x, restart_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_e(start_x + let_space * 1, restart_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_s(start_x + let_space * 2, restart_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_t(start_x + let_space * 3, restart_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_a(start_x + let_space * 4, restart_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_r(start_x + let_space * 5, restart_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_t(start_x + let_space * 6, restart_y, let_scale, let_thick, cell_model);
        }
    }
};
