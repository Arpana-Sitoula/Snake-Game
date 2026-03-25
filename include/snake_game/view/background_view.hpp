#pragma once
#include "graphics_render/pipeline.hpp"
#include "graphics_render/texture.hpp"
#include "graphics_render/mesh.hpp"

/**
 * BACKGROUND VIEW
 * Purpose: Renders the static background image.
 */
struct BackgroundView {
    void draw(Pipeline& pipeline, Texture& texture, Mesh& mesh) {
        pipeline.bind();
        texture.bind();
        mesh.bind();
        mesh.draw();
    }
};
