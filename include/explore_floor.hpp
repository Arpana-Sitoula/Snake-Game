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
#include "explore_floor/model/sconce_model.hpp"
#include "explore_floor/view/sconce_view.hpp"
#include "graphics_render/pipeline.hpp"
#include "graphics_render/camera.hpp"
#include "graphics_render/model.hpp"
#include "graphics_render/texture.hpp"
#include "graphics_render/audio.hpp"
#include <array>

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
    SconceView sconce_view;

    // Controller
    FloorController controller;

    // Common Resources
    Model test_box; // Internal model for drawing boxes
    
    // Plant Sub-Models
    Model plant_leaves_obj, plant_pot_obj, plant_soil_obj, plant_root_obj;
    Mesh  plant_leaves_mesh, plant_pot_mesh, plant_soil_mesh, plant_root_mesh;

    Model cat_obj;
    Mesh  cat_mesh;
    Model sconce_obj;
    Mesh  sconce_mesh;

    Texture carpet_tex;
    Texture floor_tex;
    Texture wall_tex;
    Texture sofa_tex;
    Texture screen_tex;
    Texture prompt_tex;
    Texture cat_tex;
    Texture plant_leaf_tex, plant_pot_tex, plant_soil_tex, plant_root_tex;

    // Lights
    std::array<SconceModel, 4> sconce_models;
    glm::vec3 light_color = glm::vec3(1.0f, 0.7f, 0.3f); 
    // Audio
    Audio cat_meow;
    Audio walk_sound;
    SDL_AudioStream* meow_stream = nullptr;
    SDL_AudioStream* walk_stream = nullptr;
    float meow_cooldown = 0;
    float step_timer = 0;

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

        // Cat
        cat_mesh.load_obj("assets/models/12221_Cat_v1_l3.obj");
        cat_obj.init(&cat_mesh);

        // Light bulb
        sconce_mesh.load_obj("assets/models/eb_sconce_light_01.obj");
        sconce_obj.init(&sconce_mesh);
        
        
        sconce_models[0].init(glm::vec3(-5.0f, 3.5f, -9.8f),   0.0f, 0.025f);
        sconce_models[1].init(glm::vec3( 5.0f, 3.5f, -9.8f),   0.0f, 0.025f);
        sconce_models[2].init(glm::vec3(-5.0f, 3.5f,  9.8f), 180.0f, 0.025f);
        sconce_models[3].init(glm::vec3( 5.0f, 3.5f,  9.8f), 180.0f, 0.025f);

        // Plant
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

        // Audio Init
        cat_meow.init("assets/audio/meow.wav");
        walk_sound.init("assets/audio/walk.wav");

        // Meow Stream
        meow_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr, nullptr, nullptr);
        if (meow_stream) {
            SDL_AudioSpec device_format;
            SDL_GetAudioDeviceFormat(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &device_format, nullptr);
            SDL_SetAudioStreamFormat(meow_stream, &cat_meow.spec, &device_format);
            SDL_ResumeAudioStreamDevice(meow_stream);
        }

        // Walk Stream
        walk_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr, nullptr, nullptr);
        if (walk_stream) {
            SDL_AudioSpec device_format;
            SDL_GetAudioDeviceFormat(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &device_format, nullptr);
            SDL_SetAudioStreamFormat(walk_stream, &walk_sound.spec, &device_format);
            SDL_ResumeAudioStreamDevice(walk_stream);
        }
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
        sconce_mesh.destroy();

        if (meow_stream) SDL_DestroyAudioStream(meow_stream);
        if (walk_stream) SDL_DestroyAudioStream(walk_stream);
        cat_meow.destroy();
        walk_sound.destroy();
    }

    void handle_input(Camera& camera, float delta) {
        controller.handle_input(camera, delta);
    }

    void update(float delta, Camera& camera) {
        cat_model.update(delta);

        float dist = glm::length(glm::vec3(camera._position) - cat_model.position);

        // 1. When cat is nearby player
        if (meow_cooldown > 0) {
            meow_cooldown -= delta;
        } else {
            if (dist < 3.5f) {
                cat_meow.play(meow_stream);
                meow_cooldown = 6.0f; 
            }
        }

        // 2. When player collide with cat
        static float bump_cooldown = 0;
        if (bump_cooldown > 0) bump_cooldown -= delta;
        if (dist < 1.0f && bump_cooldown <= 0) {
            cat_meow.play(meow_stream);
            bump_cooldown = 1.0f; 
        }

        // 3. Walking Sound
        if (controller.is_active()) {
            step_timer -= delta;
            if (step_timer <= 0) {
                walk_sound.play(walk_stream);
                step_timer = 0.8f; // Reduced overlap
            }
        } else {
            step_timer = 0; // Reset when standing still
        }
    }

    void draw(Pipeline& pipeline, Camera& camera, bool show_prompt = false) {
        pipeline.bind();
        pipeline.use_lighting(true);
        
        // 1. Setup Lights
        for (int i = 0; i < 4; i++) {
            pipeline.set_light(i, sconce_models[i].transform._position, sconce_models[i].light_color);
        }

        camera.bind();

        // 2. Draw Environment
        pipeline.clear_color_override();
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

        // 4. Draw Sconces (Glow)
        for (int i = 0; i < 4; i++) {
            sconce_view.draw(sconce_models[i], sconce_obj, pipeline);
        }
    }
};
