#pragma once
#include "../model/food.hpp"
#include "../model/board.hpp"
#include "graphics_render/pipeline.hpp"
#include "graphics_render/model.hpp"

/**
 * FOOD VIEW
 * Purpose: Renders the food collectible.
 */
struct FoodView {
    void draw(const Food& food, const Board& board, Pipeline& pipeline, Model& cell_model) {
        glm::vec3 food_pos = board.grid_to_world(food.position.x, food.position.y);
        cell_model.set_position(food_pos.x, food_pos.y);
        cell_model.set_scale(board.cell_size * 0.8f);
        pipeline.set_color(glm::vec4(0.996f, 0.365f, 0.624f, 1.0f));
        cell_model.draw();
    }
};
