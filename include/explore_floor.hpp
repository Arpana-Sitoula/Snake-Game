#pragma once
#include "explore_floor/model/base_floor_model.hpp"
#include "explore_floor/model/sofa_model.hpp"
#include "explore_floor/model/wall_model.hpp"
#include "explore_floor/model/carpet_model.hpp"
#include "explore_floor/model/screen_model.hpp"
#include "explore_floor/model/plant_model.hpp"
#include "explore_floor/view/floor_view.hpp"
#include "explore_floor/view/screen_view.hpp"
#include "explore_floor/view/plant_view.hpp"
#include "explore_floor/view/cat_view.hpp"
#include "explore_floor/controller/floor_controller.hpp"
#include "explore_floor/model/cat_model.hpp"
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
    PlantModel plant_model;
    CatModel cat_model;

    // Views
    FloorView view;
    ScreenView screen_view;
    PlantView plant_view;
    CatView cat_view;

    // Controller
    FloorController controller;

    // Common Resources
    Model test_box; // Internal model for drawing boxes
    
    // Plant Sub-Models
    Model plant_leaves_obj, plant_pot_obj, plant_soil_obj, plant_root_obj;
    Mesh  plant_leaves_mesh, plant_pot_mesh, plant_soil_mesh, plant_root_mesh;

    Model cat_obj;
    Mesh  cat_mesh;

    Texture carpet_tex;
    Texture floor_tex;
    Texture wall_tex;
    Texture sofa_tex;
    Texture screen_tex;
    Texture prompt_tex;
    Texture cat_tex;
    Texture plant_leaf_tex, plant_pot_tex, plant_soil_tex, plant_root_tex;

    void init() {
        base_floor.init();
        sofa_model.init();
        wall_model.init();
        carpet_model.init();
        screen_model.init();
        plant_model.init();
        cat_model.init();
        test_box.init();

        // Load Plant Parts by Material (Split into 4 components for correct texturing)
        const std::string plant_path = "../assets/models/indoor plant_02.obj";
        plant_leaves_mesh.load_obj(plant_path, "indoor_plant_02", "IDP_leaves");
        plant_pot_mesh.load_obj(plant_path, "indoor_plant_02", "IDP_Pot");
        plant_soil_mesh.load_obj(plant_path, "indoor_plant_02", "IDP_ground");
        plant_root_mesh.load_obj(plant_path, "indoor_plant_02", "IDP_root");

        // Load Cat
        cat_mesh.load_obj("../assets/models/12221_Cat_v1_l3.obj");
        cat_obj.init(&cat_mesh);

        plant_leaves_obj.init(&plant_leaves_mesh);
        plant_pot_obj.init(&plant_pot_mesh);
        plant_soil_obj.init(&plant_soil_mesh);
        plant_root_obj.init(&plant_root_mesh);
        
        // Load 3D Textures
        carpet_tex.init("carpet.jpg");
        floor_tex.init("wood_floor.jpg");
        wall_tex.init("plastered_wall.jpg");
        sofa_tex.init("floral_jacquard.jpg");

        // Use the COL texture for all 4 parts as it contains the layout for each
        plant_leaf_tex.init("indoor plant_2_COL.jpg");
        plant_pot_tex.init("indoor plant_2_COL.jpg");
        plant_soil_tex.init("indoor plant_2_COL.jpg");
        plant_root_tex.init("indoor plant_2_COL.jpg");

        // Screen 
        screen_tex.init("Game with Hello Kitty.png"); 

        // Interaction Prompt Text
        prompt_tex.init("prompt tex.png");

        // Cat Texture
        cat_tex.init("Cat_diffuse.jpg");
    }

    void destroy() {
        test_box.destroy();
        carpet_tex.destroy();
        floor_tex.destroy();
        wall_tex.destroy();
        sofa_tex.destroy();
        screen_tex.destroy();
        prompt_tex.destroy();
        plant_leaf_tex.destroy();
        plant_pot_tex.destroy();
        plant_soil_tex.destroy();
        plant_root_tex.destroy();
        cat_tex.destroy();
        cat_mesh.destroy();
    }

    void handle_input(Camera& camera, float delta) {
        controller.handle_input(camera, delta);
    }

    void update(float delta) {
        cat_model.update(delta);
    }

    void draw(Pipeline& pipeline, Camera& camera, bool show_prompt = false) {
        pipeline.bind();
        camera.bind();
        
        // Draw Environment with Textures
        view.draw(base_floor, sofa_model, wall_model, carpet_model, pipeline, test_box, carpet_tex, floor_tex, wall_tex, sofa_tex);
        
        // Draw Screen
        screen_view.draw(screen_model, pipeline, test_box, screen_tex, prompt_tex, show_prompt);

        // Draw Plant 
        plant_view.draw(plant_model, pipeline, 
                        plant_leaves_obj, plant_leaf_tex,
                        plant_pot_obj,    plant_pot_tex,
                        plant_soil_obj,   plant_soil_tex,
                        plant_root_obj,   plant_root_tex);
        
        // Draw Cat
        cat_view.draw(cat_model, pipeline, cat_obj, cat_tex);
    }
};
