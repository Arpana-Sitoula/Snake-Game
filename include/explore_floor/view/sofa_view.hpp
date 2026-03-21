#pragma once
#include "../model/sofa_model.hpp"
#include "../../graphics_render/pipeline.hpp"
#include "../../graphics_render/model.hpp"
#include <glm/ext/matrix_transform.hpp>

/**
 * SOFA VIEW
 * Purpose: Renders the sofas in the scene.
 */
struct SofaView {
    void draw(SofaModel& model, Pipeline& pipeline, Model& test_box, Texture& texture) {
        pipeline.use_texture(true);
        texture.bind();

        for (const auto& sofa : model.sofas) {
            draw_sofa(pipeline, test_box, sofa.position, sofa.rotation, sofa.color);
        }

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

    void draw_sofa(Pipeline& pipeline, Model& test_box, glm::vec3 pos, float rotation, glm::vec4 color) {
        auto rotate_vec = [&](glm::vec3 offset) {
            return glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0)) * glm::vec4(offset, 1.0f);
        };

        // Base (larger: 3.0 wide, 0.9 high, 1.5 deep)
        draw_box(pipeline, test_box, pos + glm::vec3(rotate_vec(glm::vec3(0, 0.45f, 0))), glm::vec3(3.0f, 0.9f, 1.5f), color);
        // Backrest (3.0 wide, 1.5 high, 0.3 deep)
        draw_box(pipeline, test_box, pos + glm::vec3(rotate_vec(glm::vec3(0, 1.2f, -0.6f))), glm::vec3(3.0f, 1.5f, 0.3f), color * 0.8f);
        // Arms (0.3 wide, 0.9 high, 1.5 deep)
        draw_box(pipeline, test_box, pos + glm::vec3(rotate_vec(glm::vec3(-1.35f, 0.9f, 0))), glm::vec3(0.3f, 0.9f, 1.5f), color * 0.9f);
        draw_box(pipeline, test_box, pos + glm::vec3(rotate_vec(glm::vec3(1.35f, 0.9f, 0))), glm::vec3(0.3f, 0.9f, 1.5f), color * 0.9f);
    }
};
