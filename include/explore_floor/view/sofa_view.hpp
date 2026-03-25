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
            draw_sofa(pipeline, test_box, sofa);
        }

        pipeline.use_texture(false);
    }

private:
    void draw_box(Pipeline& pipeline, Model& test_box, glm::vec3 center, glm::vec3 size, glm::vec4 color, float base_rotation) {
        pipeline.set_color(color);
        
        auto rot_y = [&](glm::vec3 v) {
            return glm::rotate(glm::mat4(1.0f), base_rotation, glm::vec3(0, 1, 0)) * glm::vec4(v, 1.0f);
        };

        // Front
        test_box.set_position(center + glm::vec3(rot_y(glm::vec3(0, 0, size.z/2))));
        test_box.transform._rotation = glm::vec3(0, base_rotation, 0);
        test_box.set_scale(size.x, size.y, 1);
        test_box.draw();
        
        // Back
        test_box.set_position(center + glm::vec3(rot_y(glm::vec3(0, 0, -size.z/2))));
        test_box.transform._rotation = glm::vec3(0, base_rotation + glm::radians(180.0f), 0);
        test_box.set_scale(size.x, size.y, 1);
        test_box.draw();
        
        // Top
        test_box.set_position(center + glm::vec3(rot_y(glm::vec3(0, size.y/2, 0))));
        test_box.transform._rotation = glm::vec3(glm::radians(-90.0f), base_rotation, 0);
        test_box.set_scale(size.x, size.z, 1);
        test_box.draw();
        
        // Bottom
        test_box.set_position(center + glm::vec3(rot_y(glm::vec3(0, -size.y/2, 0))));
        test_box.transform._rotation = glm::vec3(glm::radians(90.0f), base_rotation, 0);
        test_box.set_scale(size.x, size.z, 1);
        test_box.draw();
        
        // Left
        test_box.set_position(center + glm::vec3(rot_y(glm::vec3(-size.x/2, 0, 0))));
        test_box.transform._rotation = glm::vec3(0, base_rotation + glm::radians(-90.0f), 0);
        test_box.set_scale(size.z, size.y, 1);
        test_box.draw();
        
        // Right
        test_box.set_position(center + glm::vec3(rot_y(glm::vec3(size.x/2, 0, 0))));
        test_box.transform._rotation = glm::vec3(0, base_rotation + glm::radians(90.0f), 0);
        test_box.set_scale(size.z, size.y, 1);
        test_box.draw();
    }

    void draw_sofa(Pipeline& pipeline, Model& test_box, const Sofa& sofa) {
        auto rot_y = [&](glm::vec3 v) {
            return glm::rotate(glm::mat4(1.0f), sofa.rotation, glm::vec3(0, 1, 0)) * glm::vec4(v, 1.0f);
        };

        float w = sofa.width;
        glm::vec3 pos = sofa.position;
        glm::vec4 color = sofa.color;

        // Base (width 'w', 0.9 high, 1.5 deep)
        draw_box(pipeline, test_box, pos + glm::vec3(rot_y(glm::vec3(0, 0.45f, 0))), glm::vec3(w, 0.9f, 1.5f), color, sofa.rotation);
        
        // Backrest (width 'w', 1.5 high, 0.3 deep)
        draw_box(pipeline, test_box, pos + glm::vec3(rot_y(glm::vec3(0, 1.2f, -0.6f))), glm::vec3(w, 1.5f, 0.3f), color * 0.8f, sofa.rotation);

        // Arms (0.3 wide, 0.9 high, 1.5 deep)
        float arm_x_offset = (w / 2.0f) - 0.15f;
        if (sofa.has_left_arm) {
            draw_box(pipeline, test_box, pos + glm::vec3(rot_y(glm::vec3(-arm_x_offset, 0.9f, 0))), glm::vec3(0.3f, 0.9f, 1.5f), color * 0.9f, sofa.rotation);
        }
        if (sofa.has_right_arm) {
            draw_box(pipeline, test_box, pos + glm::vec3(rot_y(glm::vec3(arm_x_offset, 0.9f, 0))), glm::vec3(0.3f, 0.9f, 1.5f), color * 0.9f, sofa.rotation);
        }
    }
};
