#pragma once
#include "../model/screen_model.hpp"
#include "../../graphics_render/pipeline.hpp"
#include "../../graphics_render/model.hpp"
#include "../../graphics_render/texture.hpp"

struct ScreenView {
    void draw(ScreenModel& model, Pipeline& pipeline, Model& test_box, Texture& screen_tex) {
        // --- TV bezel (dark border, slightly larger than the screen) ---
        pipeline.use_texture(false);
        pipeline.set_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        test_box.set_position(model.screen.position);
        test_box.transform._rotation = glm::vec3(0, glm::radians(90.0f), 0);
        test_box.set_scale(model.screen.size.z + 0.5f, model.screen.size.y + 0.4f, 1);
        test_box.draw();

        // Screen  
        pipeline.use_texture(true);
        screen_tex.bind();
        test_box.set_position(model.screen.position + glm::vec3(0.15f, 0.0f, 0.0f));
        test_box.transform._rotation = glm::vec3(0, glm::radians(90.0f), 0);
        test_box.set_scale(model.screen.size.z, model.screen.size.y, 1);
        test_box.draw();

        pipeline.use_texture(false);
    }
};
