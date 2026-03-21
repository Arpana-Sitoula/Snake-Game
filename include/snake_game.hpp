#pragma once
#include "snake_game/model/board.hpp"
#include "snake_game/model/snake.hpp"
#include "snake_game/model/food.hpp"
#include "snake_game/view/game_view.hpp"
#include "snake_game/controller/snake_controller.hpp"
#include "snake_game/controller/game_controller.hpp"
#include "graphics_render/model.hpp"
#include "graphics_render/audio.hpp"

/**
 * SNAKE GAME (MVC Container)
 * Purpose: Connects Models, Views, and Controllers.
 */
struct SnakeGame {
    // Models
    Board board;
    Snake snake;
    Food food;
    bool waiting_to_start = true;

    // View
    GameView view;
    Model cell_model;

    // Background Assets
    Pipeline _bg_pipeline;
    Texture _bg_texture;
    Mesh _bg_mesh;
    
    // Kitty Assets
    Pipeline _kitty_pipeline;
    Texture _kitty_texture;
    Model _kitty_model;

    // Audio Assets
    Audio _wow, _faaah;
    SDL_AudioStream *_wow_stream = nullptr, *_faaah_stream = nullptr;

    // Controllers
    SnakeController snake_controller;
    GameController game_controller;
    
    void init() {
        cell_model.init();
        snake.init(board.width / 4, board.height / 4);
        food.spawn(board, snake);

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

        // Audio Init
        SDL_InitSubSystem(SDL_INIT_AUDIO);
        _wow.init("assets/audio/woww.wav");
        _faaah.init("assets/audio/faaah.wav");

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
    }
    
    void destroy() {
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

        cell_model.destroy();
    }
    
    void handle_input() {
        snake_controller.handle_input(snake);
        game_controller.handle_input(waiting_to_start, snake, board, food);
    }
    
    void update(float delta) {
        if (waiting_to_start || !snake.alive) return;

        snake.update(delta, board);
        
        if (!snake.alive) {
            _faaah.play(_faaah_stream);
        }

        if (food.check_eaten(snake)) {
            _wow.play(_wow_stream);
            snake.grow();
            food.spawn(board, snake);
        }
    }
    
    void draw(Pipeline& pipeline, Camera& camera) {
        view.draw(board, snake, food, waiting_to_start, pipeline, cell_model,
                  _bg_pipeline, _bg_texture, _bg_mesh,
                  _kitty_pipeline, _kitty_texture, _kitty_model, camera);
    }
};
