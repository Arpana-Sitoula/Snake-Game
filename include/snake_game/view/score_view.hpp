#pragma once
#include "digit_renderer.hpp"
#include "graphics_render/pipeline.hpp"

/**
 * SCORE VIEW
 * Purpose: Renders the scoreboard.
 */
struct ScoreView {
    void draw(int score, float board_top, float border_thickness, Pipeline& pipeline, Model& cell_model) {
        float score_bg_width = 1.6f;
        float score_bg_height = 0.6f;
        float score_bg_y = board_top + border_thickness + score_bg_height / 2.0f + 0.1f;

        cell_model.transform._position = glm::vec3(0, score_bg_y, 0);
        cell_model.transform._scale = glm::vec3(score_bg_width, score_bg_height, 1);
        pipeline.set_color(glm::vec4(0.5922f, 0.8078f, 0.9882f, 1.0f));
        cell_model.draw();

        int d1 = score / 100;
        int d2 = (score / 10) % 10;
        int d3 = score % 10;

        float digit_scale = 0.2f;
        float digit_thick = 0.05f;
        float spacing = 0.4f;
        pipeline.set_color(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)); 

        DigitRenderer::draw(d1, -spacing, score_bg_y, digit_scale, digit_thick, cell_model);
        DigitRenderer::draw(d2, 0.0f,     score_bg_y, digit_scale, digit_thick, cell_model);
        DigitRenderer::draw(d3, spacing,  score_bg_y, digit_scale, digit_thick, cell_model);
    }
};
