#pragma once
#include "../model/floor_model.hpp"
#include "../../graphics_render/pipeline.hpp"
#include "../../graphics_render/model.hpp"
#include <glm/ext/matrix_transform.hpp>

/**
 * FLOOR VIEW
 * Purpose: Renders the floor and the sofas.
 */
struct FloorView {
    void draw(FloorModel& model, Pipeline& pipeline, Model& test_box) {
        // Draw each sofa
        for (const auto& sofa : model.sofas) {
            draw_sofa(pipeline, test_box, sofa.position, sofa.rotation, sofa.color);
        }

        // Draw the floor
        draw_box(pipeline, test_box, model.floor_pos, model.floor_size, model.floor_color);
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

    void draw_sofa(Pipeline& pipeline, Model& test_box, glm::vec3 pos, float rotation, glm::vec4 color) {
        auto rotate_vec = [&](glm::vec3 offset) {
            return glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0)) * glm::vec4(offset, 1.0f);
        };

        // Sofa components with offsets and custom colors
        // Base
        draw_box(pipeline, test_box, pos + glm::vec3(rotate_vec(glm::vec3(0, 0.3f, 0))), glm::vec3(2.0f, 0.6f, 1.0f), color);
        // Backrest (offset in Z)
        draw_box(pipeline, test_box, pos + glm::vec3(rotate_vec(glm::vec3(0, 0.8f, -0.4f))), glm::vec3(2.0f, 1.0f, 0.2f), color * 0.8f);
        // Arms
        draw_box(pipeline, test_box, pos + glm::vec3(rotate_vec(glm::vec3(-0.9f, 0.6f, 0))), glm::vec3(0.2f, 0.6f, 1.0f), color * 0.9f);
        draw_box(pipeline, test_box, pos + glm::vec3(rotate_vec(glm::vec3(0.9f, 0.6f, 0))), glm::vec3(0.2f, 0.6f, 1.0f), color * 0.9f);
    }
};
