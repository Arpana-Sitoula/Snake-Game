#pragma once
#include "../model/board.hpp"
#include "graphics_render/pipeline.hpp"
#include "graphics_render/model.hpp"

/**
 * BOARD VIEW
 * Purpose: Renders the game board grid and borders.
 */
struct BoardView {
    void draw(const Board& board, Pipeline& pipeline, Model& cell_model) {
        float cell_scale = board.cell_size;
        
        // Draw grid
        for (int x = 0; x < board.width; x++) {
            for (int y = 0; y < board.height; y++) {
                glm::vec3 pos = board.grid_to_world(x, y);
                cell_model.set_position(pos.x, pos.y);
                cell_model.set_scale(cell_scale);
                
                if ((x + y) % 2 == 0) {
                    pipeline.set_color(glm::vec4(0.988f, 0.835f, 0.808f, 1.0f)); 
                } else {
                    pipeline.set_color(glm::vec4(0.980f, 0.882f, 0.867f, 1.0f)); 
                }
                
                cell_model.draw();
            }
        }
        
        // Draw borders
        float total_w = board.width * board.cell_size;
        float total_h = board.height * board.cell_size;
        float border_thickness = 0.34f;
        
        pipeline.set_color(glm::vec4(1.0f, 0.42f, 0.42f, 1.0f)); 
        
        // Top
        cell_model.transform._position = glm::vec3(0, total_h / 2.0f + border_thickness / 2.0f, 0);
        cell_model.transform._scale = glm::vec3(total_w + border_thickness * 2, border_thickness, 1);
        cell_model.draw();
        // Bottom
        cell_model.transform._position = glm::vec3(0, -total_h / 2.0f - border_thickness / 2.0f, 0);
        cell_model.transform._scale = glm::vec3(total_w + border_thickness * 2, border_thickness, 1);
        cell_model.draw();
        // Left
        cell_model.transform._position = glm::vec3(-total_w / 2.0f - border_thickness / 2.0f, 0, 0);
        cell_model.transform._scale = glm::vec3(border_thickness, total_h, 1);
        cell_model.draw();
        // Right
        cell_model.transform._position = glm::vec3(total_w / 2.0f + border_thickness / 2.0f, 0, 0);
        cell_model.transform._scale = glm::vec3(border_thickness, total_h, 1);
        cell_model.draw();
    }
};
