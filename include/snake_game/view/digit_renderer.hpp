#pragma once
#include "graphics_render/model.hpp"

/**
 * DIGIT RENDERER / UI (Snake Game View Utility)
 * Purpose: Draws classic 7-segment digital numbers and basic letters for the UI.
 */
struct DigitRenderer {
    static bool has_segment(int val, int seg) {
        static const int segments[10] = {
            0b0111111, // 0
            0b0000110, // 1
            0b1011011, // 2
            0b1001111, // 3
            0b1100110, // 4
            0b1101101, // 5
            0b1111101, // 6
            0b0000111, // 7
            0b1111111, // 8
            0b1101111  // 9
        };
        if (val < 0 || val > 9) return false;
        return (segments[val] & (1 << seg)) != 0;
    }

    static void draw(int val, float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        if (has_segment(val, 0)) { model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw(); }
        if (has_segment(val, 1)) { model.transform._position = glm::vec3(x + h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw(); }
        if (has_segment(val, 2)) { model.transform._position = glm::vec3(x + h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw(); }
        if (has_segment(val, 3)) { model.transform._position = glm::vec3(x, y - v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw(); }
        if (has_segment(val, 4)) { model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw(); }
        if (has_segment(val, 5)) { model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw(); }
        if (has_segment(val, 6)) { model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw(); }
    }

    static void draw_r(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 4.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len * 1.2f, 1); model.transform._rotation = glm::vec3(0, 0, 0.4f); model.draw();
        model.transform._rotation = glm::vec3(0, 0, 0);
    }
    static void draw_e(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y - v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
    }
    static void draw_s(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y - v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
    }
    static void draw_t(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(t, v_len * 2, 1); model.draw();
    }
    static void draw_a(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
    }
    static void draw_p(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
    }
    static void draw_l(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x, y - v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
    }
    static void draw_y(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
    }
};
