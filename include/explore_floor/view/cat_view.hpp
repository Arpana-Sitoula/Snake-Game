#pragma once
#include "../model/cat_model.hpp"
#include "../../graphics_render/pipeline.hpp"
#include "../../graphics_render/model.hpp"
#include "../../graphics_render/texture.hpp"

struct CatView {
    void draw(CatModel& model, Pipeline& pipeline, Model& cat_obj, Texture& cat_tex) {
        pipeline.use_texture(true);
        cat_tex.bind();
        
        cat_obj.set_position(model.position);
        cat_obj.transform._rotation = model.rotation;
        cat_obj.set_scale(model.scale.x, model.scale.y, model.scale.z);
        
        cat_obj.draw();
        
        pipeline.use_texture(false);
    }
};
