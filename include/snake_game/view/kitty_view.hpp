#pragma once
#include "graphics_render/pipeline.hpp"
#include "graphics_render/texture.hpp"
#include "graphics_render/model.hpp"
#include "graphics_render/camera.hpp"

/**
 * KITTY VIEW
 * Purpose: Renders the kitty decoration with transparency.
 */
struct KittyView {
    void draw(Pipeline& pipeline, Texture& texture, Model& model, Camera& camera) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        pipeline.bind();
        camera.bind(); 
        texture.bind();
        model.draw();
        
        glDisable(GL_BLEND);
    }
};
