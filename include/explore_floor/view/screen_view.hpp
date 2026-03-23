#pragma once
#include "../model/screen_model.hpp"
#include "../../graphics_render/pipeline.hpp"
#include "../../graphics_render/model.hpp"

struct ScreenView {
    void draw(ScreenModel& model, Pipeline& pipeline, Model& test_box) {
        pipeline.use_texture(false);
        pipeline.set_color(model.screen.color);

        test_box.set_position(model.screen.position);
        test_box.transform._rotation = glm::vec3(0, glm::radians(90.0f), 0);
        test_box.set_scale(model.screen.size.z, model.screen.size.y, 1);
        test_box.draw();
    }
};
