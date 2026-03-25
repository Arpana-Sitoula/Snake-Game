#pragma once
#include "../model/base_floor_model.hpp"
#include "../../graphics_render/pipeline.hpp"
#include "../../graphics_render/model.hpp"

/**
 * BASE FLOOR VIEW
 * Purpose: Renders the base floor.
 */
struct BaseFloorView {
    void draw(BaseFloorModel& model, Pipeline& pipeline, Model& test_box, Texture& texture) {
        pipeline.use_texture(true);
        texture.bind();
        draw_box(pipeline, test_box, model.position, model.size, model.color);
        pipeline.use_texture(false);
    }

private:
    void draw_box(Pipeline& pipeline, Model& test_box, glm::vec3 pos, glm::vec3 size, glm::vec4 color) {
        pipeline.set_color(color);
        
        // Front
        test_box.set_position(pos.x, pos.y, pos.z + size.z/2);
        test_box.transform._rotation = glm::vec3(0, 0, 0);
        test_box.set_scale(size.x, size.y, 1);
        test_box.draw();
        
        // Back
        test_box.set_position(pos.x, pos.y, pos.z - size.z/2);
        test_box.transform._rotation = glm::vec3(0, glm::radians(180.0f), 0);
        test_box.set_scale(size.x, size.y, 1);
        test_box.draw();
        
        // Top
        test_box.set_position(pos.x, pos.y + size.y/2, pos.z);
        test_box.transform._rotation = glm::vec3(glm::radians(-90.0f), 0, 0);
        test_box.set_scale(size.x, size.z, 1);
        test_box.draw();
        
        // Bottom
        test_box.set_position(pos.x, pos.y - size.y/2, pos.z);
        test_box.transform._rotation = glm::vec3(glm::radians(90.0f), 0, 0);
        test_box.set_scale(size.x, size.z, 1);
        test_box.draw();
        
        // Left
        test_box.set_position(pos.x - size.x/2, pos.y, pos.z);
        test_box.transform._rotation = glm::vec3(0, glm::radians(-90.0f), 0);
        test_box.set_scale(size.z, size.y, 1);
        test_box.draw();
        
        // Right
        test_box.set_position(pos.x + size.x/2, pos.y, pos.z);
        test_box.transform._rotation = glm::vec3(0, glm::radians(90.0f), 0);
        test_box.set_scale(size.z, size.y, 1);
        test_box.draw();
    }
};
