#pragma once
#include "snake_game.hpp"
#include "explore_floor.hpp"
#include "graphics_render/input.hpp"
#include "graphics_render/model.hpp"

/**
 * GAME SITE MANAGER
 * Purpose: Manages the high-level state of the application.
 * Switches between different modes like Snake Game or Explore Floor.
 */
struct GameSiteManager {
    enum class State {
        SNAKE_GAME,
        EXPLORE_FLOOR
    };

    State current_state = State::EXPLORE_FLOOR;
    SnakeGame snake_game;
    ExploreFloor explore_floor;

    void init() {
        snake_game.init();
        explore_floor.init();
        
        if (current_state == State::EXPLORE_FLOOR) {
            SDL_SetWindowRelativeMouseMode(SDL_GetGrabbedWindow(), true);
        }
    }

    void destroy() {
        snake_game.destroy();
        explore_floor.destroy();
    }

    void handle_input(Window& window, Camera& camera) {
        // Toggle state with TAB key
        if (Keys::pressed(SDLK_TAB)) {
            if (current_state == State::SNAKE_GAME) {
                current_state = State::EXPLORE_FLOOR;
                SDL_SetWindowRelativeMouseMode(window._window_p, true); // Lock mouse for 3D
                // Start at the door
                camera._position = glm::vec3(0, 1.8f, 11.0f);
                camera._rotation = glm::vec3(0, 0, 0);
                camera._fov = 70.0f; // Original 70 degrees for 3D exploration
            } else {
                current_state = State::SNAKE_GAME;
                SDL_SetWindowRelativeMouseMode(window._window_p, false); // Unlock mouse for 2D
                // Reset camera for 2D game
                camera._position = glm::vec3(0, 0, 10);
                camera._rotation = glm::vec3(0, 0, 0);
                camera._fov = 50.0f; // Approx original look for 2D board
            }
        }

        if (current_state == State::SNAKE_GAME) {
            snake_game.handle_input();
        }
    }

    void update(float delta, Camera& camera) {
        if (current_state == State::SNAKE_GAME) {
            // Reset camera for 2D game
            camera._position = glm::vec3(0, 0, 10);
            camera._rotation = glm::vec3(0, 0, 0);
            camera._fov = 50.0f;
            snake_game.update(delta);
            glDisable(GL_DEPTH_TEST);
        } else {
            glEnable(GL_DEPTH_TEST);
            explore_floor.handle_input(camera, delta);
            explore_floor.update(delta);
        }
    }

    void draw(Pipeline& pipeline, Camera& camera) {
        if (current_state == State::SNAKE_GAME) {
            snake_game.draw(pipeline, camera);
        } else {
            explore_floor.draw(pipeline, camera);
        }
    }
};
