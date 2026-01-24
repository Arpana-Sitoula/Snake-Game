#pragma once
#include "graphics_render/time.hpp"
#include "graphics_render/mesh.hpp"
#include "graphics_render/input.hpp"
#include "graphics_render/window.hpp"
#include "graphics_render/camera.hpp"
#include "graphics_render/texture.hpp"
#include "graphics_render/pipeline.hpp"
#include "graphics_render/transform.hpp"
#include "game_logic/board.hpp"
#include "game_logic/snake.hpp"
#include "game_logic/food.hpp"


struct Engine {
    Engine() {
        _time.init();

        // create render components
        _window.init(1280, 720);
        _pipeline_vertcols.init("default.vert", "vertcols.frag");
        _mesh.init();

        // set up camera for 2D top-down view
        _camera._position = glm::vec3(0, 0, 10);
        _camera._rotation = glm::vec3(0, 0, 0);
        
        // initialize snake game
        _snake.init(_board.width / 4, _board.height / 2);
        _food.spawn(_board, _snake);
    }
    ~Engine() {
        // destroy in reversed init() order
        _mesh.destroy();
        _pipeline_vertcols.destroy();
        _window.destroy();
    }

    auto handle_sdl_event(SDL_Event& event) -> SDL_AppResult {
        switch (event.type) {
            case SDL_EventType::SDL_EVENT_QUIT: return SDL_AppResult::SDL_APP_SUCCESS;
            default: break;
        }
        Input::register_event(event);
        return SDL_AppResult::SDL_APP_CONTINUE;
    }
    
    auto handle_sdl_frame() -> SDL_AppResult {
        _time.update();
        
        // dark background
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handle snake input
        if (Keys::pressed(SDLK_W) || Keys::pressed(SDLK_UP))    _snake.set_direction(Snake::Direction::UP);
        if (Keys::pressed(SDLK_S) || Keys::pressed(SDLK_DOWN))  _snake.set_direction(Snake::Direction::DOWN);
        if (Keys::pressed(SDLK_A) || Keys::pressed(SDLK_LEFT))  _snake.set_direction(Snake::Direction::LEFT);
        if (Keys::pressed(SDLK_D) || Keys::pressed(SDLK_RIGHT)) _snake.set_direction(Snake::Direction::RIGHT);
        
        // Restart game on R
        if (Keys::pressed(SDLK_R)) {
            _snake.init(_board.width / 4, _board.height / 2);
            _food.spawn(_board, _snake);
        }
        
        // Update snake
        _snake.update(_time._delta, _board);
        
        // Check food collision
        if (_food.check_eaten(_snake)) {
            _snake.grow();
            _food.spawn(_board, _snake);
        }

        // Set up pipeline
        _pipeline_vertcols.bind();
        _camera.bind();
        _mesh.bind();
        
        // Calculate cell scale
        float cell_scale = _board.cell_size * 0.9f;  // slight gap between cells
        
        // Draw board background
        for (int x = 0; x < _board.width; x++) {
            for (int y = 0; y < _board.height; y++) {
                _transform._position = _board.grid_to_world(x, y);
                _transform._scale = glm::vec3(cell_scale, cell_scale, 0.1f);
                _transform._rotation = glm::vec3(0, 0, 0);
                
                // Checkerboard pattern
                bool dark = (x + y) % 2 == 0;
                _pipeline_vertcols.set_color(dark ? glm::vec4(0.15f, 0.15f, 0.2f, 1.0f) 
                                     : glm::vec4(0.2f, 0.2f, 0.25f, 1.0f));
                
                _transform.bind();
                _mesh.draw();
            }
        }
        
        // Draw snake
        int segment_index = 0;
        for (const auto& segment : _snake.body) {
            _transform._position = _board.grid_to_world(segment.x, segment.y);
            _transform._scale = glm::vec3(cell_scale, cell_scale, 0.15f);
            
            // Head is brighter
            if (segment_index == 0) {
                _pipeline_vertcols.set_color(_snake.alive ? glm::vec4(0.2f, 0.9f, 0.3f, 1.0f) 
                                             : glm::vec4(0.7f, 0.2f, 0.2f, 1.0f));
            } else {
                _pipeline_vertcols.set_color(_snake.alive ? glm::vec4(0.1f, 0.7f, 0.2f, 1.0f) 
                                             : glm::vec4(0.5f, 0.15f, 0.15f, 1.0f));
            }
            
            _transform.bind();
            _mesh.draw();
            segment_index++;
        }
        
        // Draw food
        _transform._position = _board.grid_to_world(_food.position.x, _food.position.y);
        _transform._scale = glm::vec3(cell_scale * 0.8f, cell_scale * 0.8f, 0.15f);
        _pipeline_vertcols.set_color(glm::vec4(0.9f, 0.2f, 0.2f, 1.0f));  // red food
        _transform.bind();
        _mesh.draw();

        // present drawn image to screen
        SDL_GL_SwapWindow(_window._window_p);
        // clear single-frame inputs
        Input::flush();
        return SDL_AppResult::SDL_APP_CONTINUE;
    }

    Time _time;
    Mesh _mesh;
    Window _window;
    Camera _camera;
    Pipeline _pipeline_vertcols;
    Transform _transform;
    
    // Snake game components
    Board _board;
    Snake _snake;
    Food _food;
};