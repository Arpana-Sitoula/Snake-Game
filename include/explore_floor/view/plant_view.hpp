#pragma once
#include "../model/plant_model.hpp"
#include "../../graphics_render/pipeline.hpp"
#include "../../graphics_render/mesh.hpp"
#include "../../graphics_render/model.hpp"

struct PlantView {
    void draw(PlantModel& model, Pipeline& pipeline, 
              Model& leaves, Texture& leaf_tex,
              Model& pot, Texture& pot_tex,
              Model& soil, Texture& soil_tex,
              Model& root, Texture& root_tex) {
        
        auto draw_part = [&](Model& m, Texture& t) {
            pipeline.use_texture(true);
            t.bind();
            m.set_position(model.position);
            m.set_scale(model.scale.x, model.scale.y, model.scale.z);
            m.draw();
        };

        pipeline.set_color(glm::vec4(1.0f));
        draw_part(leaves, leaf_tex);
        draw_part(pot, pot_tex);
        draw_part(soil, soil_tex);
        draw_part(root, root_tex);

        pipeline.use_texture(false);
    }
};
