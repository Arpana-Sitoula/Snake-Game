#pragma once
#include "graphics_render/model.hpp"
#include "graphics_render/pipeline.hpp"

/**
 * DIGIT RENDERER / UI (Graphics Render)
 * Purpose: Draws classic 7-segment digital numbers for the UI (like a digital clock).
 * 
 * Flow:
 * - has_segment(): Uses bitwise math to know which of the 7 bars make up a specific number (0-9)
 * - draw(): Moves, squishes, and stretches a single square Model over and over to draw 
 *   each necessary bar for a digit at a specific location
 */
// Draw a simple 7-segment display digit using the cell model
struct DigitRenderer {
    // Segments are numbered:
    //  000
    // 5   1
    // 5   1
    //  666
    // 4   2
    // 4   2
    //  333
    // Returns true if segment 'seg' is active for digit 'val'
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

    // draw one digit at position (x,y) with a given scale and thickness
    static void draw(int val, float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale;          // horizontal segment length
        float v_len = scale;          // vertical segment length
        float t = thickness;          // thickness

        // 0: top
        if (has_segment(val, 0)) {
            model.transform._position = glm::vec3(x, y + v_len, 0);
            model.transform._scale = glm::vec3(h_len, t, 1);
            model.draw();
        }
        // 1: top-right
        if (has_segment(val, 1)) {
            model.transform._position = glm::vec3(x + h_len / 2.0f, y + v_len / 2.0f, 0);
            model.transform._scale = glm::vec3(t, v_len, 1);
            model.draw();
        }
        // 2: bottom-right
        if (has_segment(val, 2)) {
            model.transform._position = glm::vec3(x + h_len / 2.0f, y - v_len / 2.0f, 0);
            model.transform._scale = glm::vec3(t, v_len, 1);
            model.draw();
        }
        // 3: bottom
        if (has_segment(val, 3)) {
            model.transform._position = glm::vec3(x, y - v_len, 0);
            model.transform._scale = glm::vec3(h_len, t, 1);
            model.draw();
        }
        // 4: bottom-left
        if (has_segment(val, 4)) {
            model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0);
            model.transform._scale = glm::vec3(t, v_len, 1);
            model.draw();
        }
        // 5: top-left
        if (has_segment(val, 5)) {
            model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0);
            model.transform._scale = glm::vec3(t, v_len, 1);
            model.draw();
        }
        // 6: middle
        if (has_segment(val, 6)) {
            model.transform._position = glm::vec3(x, y, 0);
            model.transform._scale = glm::vec3(h_len, t, 1);
            model.draw();
        }
    }

    // For spelling "RESTART" without a font library
    static void draw_r(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale;
        float v_len = scale;
        float t = thickness;

        // Left vertical (top and bottom)
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0);
        model.transform._scale = glm::vec3(t, v_len, 1);
        model.draw();
        
        model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0);
        model.transform._scale = glm::vec3(t, v_len, 1);
        model.draw();

        // Top horizontal
        model.transform._position = glm::vec3(x, y + v_len, 0);
        model.transform._scale = glm::vec3(h_len, t, 1);
        model.draw();

        // Top right vertical
        model.transform._position = glm::vec3(x + h_len / 2.0f, y + v_len / 2.0f, 0);
        model.transform._scale = glm::vec3(t, v_len, 1);
        model.draw();

        // Middle horizontal
        model.transform._position = glm::vec3(x, y, 0);
        model.transform._scale = glm::vec3(h_len, t, 1);
        model.transform._rotation = glm::vec3(0, 0, 0);
        model.draw();

        // Bottom right diagonal (rotated leg)
        model.transform._position = glm::vec3(x + h_len / 4.0f, y - v_len / 2.0f, 0);
        model.transform._scale = glm::vec3(t, v_len * 1.2f, 1);
        model.transform._rotation = glm::vec3(0, 0, 0.4f); // Angle it outward
        model.draw();
        
        // Reset rotation so it doesn't affect subsequent segments
        model.transform._rotation = glm::vec3(0, 0, 0);
    }

    static void draw_e(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        // left vertical
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        // top, mid, bottom horizontal
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y - v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
    }
    static void draw_s(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        // top left vert, bottom right vert
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        // horizontals
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y - v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
    }
    static void draw_t(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        // top horiz
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        // center vert
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(t, v_len * 2, 1); model.draw();
    }
    static void draw_a(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        // left vert, right vert
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        // top horiz, mid horiz
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
    }
    static void draw_p(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        // left vertical (top and bottom)
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        // top, mid horizontal
        model.transform._position = glm::vec3(x, y + v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        // top right vertical
        model.transform._position = glm::vec3(x + h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
    }
    static void draw_l(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        // left vertical
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x - h_len / 2.0f, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        // bottom horizontal
        model.transform._position = glm::vec3(x, y - v_len, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
    }
    static void draw_y(float x, float y, float scale, float thickness, Model& model) {
        float h_len = scale; float v_len = scale; float t = thickness;
        // top left vert, top right vert
        model.transform._position = glm::vec3(x - h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        model.transform._position = glm::vec3(x + h_len / 2.0f, y + v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
        // mid horizontal, bottom horiz
        model.transform._position = glm::vec3(x, y, 0); model.transform._scale = glm::vec3(h_len, t, 1); model.draw();
        // bottom center vert
        model.transform._position = glm::vec3(x, y - v_len / 2.0f, 0); model.transform._scale = glm::vec3(t, v_len, 1); model.draw();
    }
};
