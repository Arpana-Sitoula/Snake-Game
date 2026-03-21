#pragma once
#include "explore_floor/model/base_floor_model.hpp"
#include "explore_floor/model/sofa_model.hpp"
#include "explore_floor/model/wall_model.hpp"
#include "explore_floor/model/carpet_model.hpp"
#include "explore_floor/view/floor_view.hpp"
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

    // View
    FloorView view;

    // Controller
    FloorController controller;

    // Assets
    Model test_box; // Internal model for drawing boxes

    void init() {
        base_floor.init();
        sofa_model.init();
        wall_model.init();
        carpet_model.init();
        test_box.init();
    }

    void destroy() {
        test_box.destroy();
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
        
        // Draw Environment
        view.draw(base_floor, sofa_model, wall_model, carpet_model, pipeline, test_box);
    }
};
