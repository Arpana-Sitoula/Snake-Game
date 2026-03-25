#pragma once
#include "board_view.hpp"
#include "snake_view.hpp"
#include "food_view.hpp"
#include "score_view.hpp"
#include "button_view.hpp"
#include "background_view.hpp"
#include "kitty_view.hpp"
#include "graphics_render/camera.hpp"

/**
 * GAME VIEW (Composite)
 * Purpose: Orchestrates the drawing of all game components.
 */
struct GameView {
    BackgroundView background_view;
    KittyView kitty_view;
    BoardView board_view;
    SnakeView snake_view;
    FoodView food_view;
    ScoreView score_view;
    ButtonView button_view;

    void draw(const Board& board, const Snake& snake, const Food& food, bool waiting_to_start, Pipeline& pipeline, Model& cell_model,
              Pipeline& bg_pipeline, Texture& bg_texture, Mesh& bg_mesh,
              Pipeline& kitty_pipeline, Texture& kitty_texture, Model& kitty_model, Camera& camera) {
        
        // 1. Draw Background
        background_view.draw(bg_pipeline, bg_texture, bg_mesh);

        // 2. Draw Kitty
        kitty_view.draw(kitty_pipeline, kitty_texture, kitty_model, camera);

        // 3. Draw Game Elements
        pipeline.bind();
        camera.bind();
        
        board_view.draw(board, pipeline, cell_model);
        snake_view.draw(snake, board, pipeline, cell_model);
        food_view.draw(food, board, pipeline, cell_model);
        
        float border_thickness = 0.34f;
        float board_top = (board.height * board.cell_size) / 2.0f;
        
        score_view.draw(snake.score, board_top, border_thickness, pipeline, cell_model);
        
        if (!snake.alive || waiting_to_start) {
            button_view.draw(waiting_to_start, board_top, border_thickness, pipeline, cell_model);
        }
    }
};
