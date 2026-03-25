#pragma once
#include "../model/snake.hpp"
#include "../model/board.hpp"
#include "graphics_render/pipeline.hpp"
#include "graphics_render/model.hpp"

/**
 * SNAKE VIEW
 * Purpose: Renders the snake segments.
 */
struct SnakeView {
    void draw(const Snake& snake, const Board& board, Pipeline& pipeline, Model& cell_model) {
        int i = 0;
        for (const auto& seg : snake.body) {
            glm::vec3 pos = board.grid_to_world(seg.x, seg.y);
            cell_model.set_position(pos.x, pos.y);
            cell_model.set_scale(board.cell_size);
            
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
    }
};
