#pragma once
#include "explore_floor/model/floor_model.hpp"
#include "explore_floor/view/floor_view.hpp"
#include "explore_floor/controller/floor_controller.hpp"
#include "graphics_render/pipeline.hpp"
#include "graphics_render/camera.hpp"
#include "graphics_render/model.hpp"

/**
 * EXPLORE FLOOR (MVC Container)
 * Purpose: Connects Floor Models, Views, and Controllers.
 */
struct ExploreFloor {
    FloorModel model;
    FloorView view;
    FloorController controller;
    Model test_box; // Internal model for drawing boxes

    void init() {
        model.init();
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
        view.draw(model, pipeline, test_box);
    }
};
