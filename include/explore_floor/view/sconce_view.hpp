#pragma once
#include "../../graphics_render/model.hpp"
#include "../../graphics_render/pipeline.hpp"
#include "../model/sconce_model.hpp"

/**
 * SCONCE VIEW
 * Purpose: Renders the 3D sconce mesh and sets the glowing "emissive" state.
 */
struct SconceView {
    void draw(const SconceModel& model, Model& mesh, Pipeline& pipeline) {
        pipeline.set_emissive(true);
        mesh.transform = model.transform;
        mesh.draw();
        pipeline.set_emissive(false);
    }
};
