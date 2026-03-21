#pragma once
#include "digit_renderer.hpp"
#include "graphics_render/pipeline.hpp"

/**
 * BUTTON VIEW
 * Purpose: Renders the Play/Restart button overlays.
 */
struct ButtonView {
    void draw(bool waiting_to_start, float board_top, float border_thickness, Pipeline& pipeline, Model& cell_model) {
        float btn_width = waiting_to_start ? 2.4f : 3.6f;
        float btn_height = 0.8f;
        float btn_y = -board_top - border_thickness - btn_height / 2.0f - 0.2f;

        // Background
        cell_model.transform._position = glm::vec3(0, btn_y, 0);
        cell_model.transform._scale = glm::vec3(btn_width, btn_height, 1);
        pipeline.set_color(glm::vec4(1.0f, 0.42f, 0.42f, 1.0f));
        cell_model.draw();

        // White text
        pipeline.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); 
        float let_scale = 0.15f;
        float let_thick = 0.04f;
        float let_space = 0.4f;

        if (waiting_to_start) {
            float start_x = -let_space * 1.5f;
            DigitRenderer::draw_p(start_x, btn_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_l(start_x + let_space * 1, btn_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_a(start_x + let_space * 2, btn_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_y(start_x + let_space * 3, btn_y, let_scale, let_thick, cell_model);
        } else {
            float start_x = -let_space * 3.0f;
            DigitRenderer::draw_r(start_x, btn_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_e(start_x + let_space * 1, btn_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_s(start_x + let_space * 2, btn_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_t(start_x + let_space * 3, btn_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_a(start_x + let_space * 4, btn_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_r(start_x + let_space * 5, btn_y, let_scale, let_thick, cell_model);
            DigitRenderer::draw_t(start_x + let_space * 6, btn_y, let_scale, let_thick, cell_model);
        }
    }
};
