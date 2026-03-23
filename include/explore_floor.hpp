#pragma once
#include "explore_floor/model/base_floor_model.hpp"
#include "explore_floor/model/sofa_model.hpp"
#include "explore_floor/model/wall_model.hpp"
#include "explore_floor/model/carpet_model.hpp"
#include "explore_floor/model/screen_model.hpp"
#include "explore_floor/view/floor_view.hpp"
#include "explore_floor/view/screen_view.hpp"
#include "explore_floor/controller/floor_controller.hpp"
#include "graphics_render/pipeline.hpp"
#include "graphics_render/camera.hpp"
#include "graphics_render/model.hpp"
#include "graphics_render/texture.hpp"

/**
 * EXPLORE FLOOR (MVC Container)
 * Purpose: Connects Floor Models, Views, and Controllers.
 */
struct ExploreFloor {
    // Models
    BaseFloorModel base_floor;
    SofaModel sofa_model;
    WallModel wall_model;
    CarpetModel carpet_model;
    ScreenModel screen_model;

    // Views
    FloorView view;
    ScreenView screen_view;

    // Controller
    FloorController controller;

    // Assets
    Model test_box; // Internal model for drawing boxes
    Texture carpet_tex;
    Texture floor_tex;
    Texture wall_tex;
    Texture sofa_tex;

    void init() {
        base_floor.init();
        sofa_model.init();
        wall_model.init();
        carpet_model.init();
        screen_model.init();
        test_box.init();
        
        // Load 3D Textures
        carpet_tex.init("carpet.jpg");
        floor_tex.init("wood_floor.jpg");
        wall_tex.init("plastered_wall.jpg");
        sofa_tex.init("floral_jacquard.jpg");
    }

    void destroy() {
        test_box.destroy();
        carpet_tex.destroy();
        floor_tex.destroy();
        wall_tex.destroy();
        sofa_tex.destroy();
    }

    void handle_input(Camera& camera, float delta) {
        controller.handle_input(camera, delta);
    }

    void update(float delta) {
        // Any logic for updating the floor models (e.g. animations)
    }

    void draw(Pipeline& pipeline, Camera& camera) {
        pipeline.bind();
        camera.bind();
        
        // Draw Environment with Textures
        view.draw(base_floor, sofa_model, wall_model, carpet_model, pipeline, test_box, carpet_tex, floor_tex, wall_tex, sofa_tex);
        
        // Draw Screen
        screen_view.draw(screen_model, pipeline, test_box);
    }
};
