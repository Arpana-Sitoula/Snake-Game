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
    
    // Background
    Pipeline _bg_pipeline;
    Texture _bg_texture;
    Mesh _bg_mesh;
    
    // Kitty Decoration
    Pipeline _kitty_pipeline;
    Texture _kitty_texture;
    Model _kitty_model;

    // Audio
    Audio _wow, _faaah;
    SDL_AudioStream *_wow_stream = nullptr, *_faaah_stream = nullptr;

    // The game manager
    GameSiteManager game_manager;
    
    Engine() {
        time.init();
        window.init(1080, 1080);
        pipeline.init("default.vert", "vertcols.frag");
        game_manager.init();
        
        // Background Init
        _bg_pipeline.init("background.vert", "background.frag");
        _bg_texture.init("image.png");
        _bg_mesh.init();

        // Kitty Init
        _kitty_pipeline.init("default.vert", "background.frag");
        _kitty_texture.init("kitty.png");
        _kitty_model.init();
        _kitty_model.set_position(-2.5f, 3.55f);
        _kitty_model.transform._rotation = glm::vec3(0, 0, glm::radians(180.0f));
        _kitty_model.set_scale(1.8f);

        // Init Audio Subsystem
        SDL_InitSubSystem(SDL_INIT_AUDIO);
        _wow.init("assets/audio/woww.wav");
        _faaah.init("assets/audio/faaah.wav");

        // Open streams for playback
        _wow_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr, nullptr, nullptr);
        _faaah_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr, nullptr, nullptr);

        SDL_AudioSpec device_format;
        SDL_GetAudioDeviceFormat(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &device_format, nullptr);

        if (_wow_stream) {
            SDL_SetAudioStreamFormat(_wow_stream, &_wow.spec, &device_format);
            SDL_ResumeAudioStreamDevice(_wow_stream);
        }
        if (_faaah_stream) {
            SDL_SetAudioStreamFormat(_faaah_stream, &_faaah.spec, &device_format);
            SDL_ResumeAudioStreamDevice(_faaah_stream);
        }

        // Fixed camera looking at the board
        camera._position = glm::vec3(0, 0, 10);
        camera._rotation = glm::vec3(0, 0, 0);
    }
    
    ~Engine() {
        if (_wow_stream) SDL_DestroyAudioStream(_wow_stream);
        if (_faaah_stream) SDL_DestroyAudioStream(_faaah_stream);
        _wow.destroy();
        _faaah.destroy();

        _bg_mesh.destroy();
        _bg_texture.destroy();
        _bg_pipeline.destroy();

        _kitty_model.destroy();
        _kitty_texture.destroy();
        _kitty_pipeline.destroy();
        
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
        
        bool is_snake_game = (game_manager.current_state == GameSiteManager::State::SNAKE_GAME);

        if (is_snake_game) {
            // Draw Background
            _bg_pipeline.bind();
            _bg_texture.bind();
            _bg_mesh.bind();
            _bg_mesh.draw();

            // Draw Kitty Decoration 
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            _kitty_pipeline.bind();
            camera.bind(); 
            _kitty_texture.bind();
            _kitty_model.draw();
            glDisable(GL_BLEND);
        }

        // Game logic
        game_manager.handle_input();
        game_manager.update(time._delta, _wow, _wow_stream, _faaah, _faaah_stream);
        
        // Render
        pipeline.bind();
        camera.bind();
        game_manager.draw(pipeline);
        
        // Present
        SDL_GL_SwapWindow(window._window_p);
        Input::flush();
        
        return SDL_AppResult::SDL_APP_CONTINUE;
    }
};