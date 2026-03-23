#pragma once
#include "graphics_render/time.hpp"
#include "graphics_render/input.hpp"
#include "graphics_render/window.hpp"
#include "graphics_render/camera.hpp"
#include "graphics_render/pipeline.hpp"
#include "graphics_render/audio.hpp"
#include "graphics_render/texture.hpp"
#include "game_site_manager.hpp"

/**
 * ENGINE
 * Purpose: The beating heart of the program that ties everything together.
 * 
 * Flow:
 * - Owns all the core systems (Window, Time, Input, Camera, Graphics Pipeline)
 * - Owns the specific running Game (SnakeGame)
 * - handle_sdl_event(): Listens to the window and passes keys/clicks into the Input system
 * - handle_sdl_frame(): Runs 60+ times a second to update the time, update the game logic,
 *   clear the screen, and ask the game to draw itself using the camera.
 */
struct Engine {
    // Core systems
    Time time;
    Window window;
    Camera camera;
    Pipeline pipeline;
    
    // The game manager
    GameSiteManager game_manager;
    
    Engine() {
        time.init();
        window.init(1080, 1080);
        pipeline.init("default.vert", "default.frag");
        game_manager.init();

        // Default camera for 2D Snake Game
        camera._position = glm::vec3(0, 0, 10);
        camera._rotation = glm::vec3(0, 0, 0);
        camera._fov = 50.0f;

        pipeline.bind();
        pipeline.use_texture(false);
    }
    
    ~Engine() {
        game_manager.destroy();
        pipeline.destroy();
        window.destroy();
    }

    auto handle_sdl_event(SDL_Event& event) -> SDL_AppResult {
        if (event.type == SDL_EventType::SDL_EVENT_QUIT) {
            return SDL_AppResult::SDL_APP_SUCCESS;
        }
        Input::register_event(event);
        return SDL_AppResult::SDL_APP_CONTINUE;
    }
    
    auto handle_sdl_frame() -> SDL_AppResult {
        time.update();
        
        // Clear screen
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Game logic
        game_manager.handle_input(window, camera);
        game_manager.update(time._delta, camera);
        
        // Render
        game_manager.draw(pipeline, camera);
        
        // Present
        SDL_GL_SwapWindow(window._window_p);
        Input::flush();
        
        return SDL_AppResult::SDL_APP_CONTINUE;
    }
};