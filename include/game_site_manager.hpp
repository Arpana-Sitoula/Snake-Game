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

    // Persist camera state to return to previous location
    glm::vec3 last_explore_pos = glm::vec3(0, 1.8f, 11.0f);
    glm::vec3 last_explore_rot = glm::vec3(0, 0, 0);

    void init() {
        snake_game.init();
        explore_floor.init();
    }

    void destroy() {
        snake_game.destroy();
        explore_floor.destroy();
    }

    void handle_input(Window& window, Camera& camera) {
        // Only switch to snake game when near the screen interaction zone
        if (Keys::pressed(SDLK_TAB)) {
            if (current_state == State::SNAKE_GAME) {
                // RETURN TO EXPLORE FLOOR
                current_state = State::EXPLORE_FLOOR;
                SDL_SetWindowRelativeMouseMode(window._window_p, true);
                
                // Restore previous camera state instead of resetting to the door
                camera._position = last_explore_pos;
                camera._rotation = last_explore_rot;
                camera._fov = 70.0f;
            } else if (near_screen(camera)) {
                // START SNAKE GAME
                // Save current camera state before switching
                last_explore_pos = camera._position;
                last_explore_rot = camera._rotation;

                current_state = State::SNAKE_GAME;
                SDL_SetWindowRelativeMouseMode(window._window_p, false);
                
                // Setup 2D view for snake game
                camera._position = glm::vec3(0, 0, 10);
                camera._rotation = glm::vec3(0, 0, 0);
                camera._fov = 50.0f;
            }
        }

        if (current_state == State::SNAKE_GAME) {
            snake_game.handle_input();
        } else {
            // EXPLORE FLOOR
            // Fail-safe: Ensure mouse is locked if we are in 3D mode (TODO: Find solution for exit)
            if (!SDL_GetWindowRelativeMouseMode(window._window_p)) {
                SDL_SetWindowRelativeMouseMode(window._window_p, true);
            }
        }
    }

    void update(float delta, Camera& camera) {
        if (current_state == State::SNAKE_GAME) {
            // Ensure camera stays locked in 2D position while playing
            camera._position = glm::vec3(0, 0, 10);
            camera._rotation = glm::vec3(0, 0, 0);
            camera._fov = 50.0f;
            snake_game.update(delta);
            glDisable(GL_DEPTH_TEST);
        } else {
            glEnable(GL_DEPTH_TEST);
            explore_floor.handle_input(camera, delta);
            explore_floor.update(delta, camera);
        }
    }

    void draw(Pipeline& pipeline, Camera& camera) {
        if (current_state == State::SNAKE_GAME) {
            pipeline.use_lighting(false);
            snake_game.draw(pipeline, camera);
        } else {
            explore_floor.draw(pipeline, camera, near_screen(camera));
        }
    }

    bool near_screen(const Camera& camera) const {
        glm::vec3 zone = glm::vec3(-7.5f, 1.8f, 0.0f);
        float dist = glm::length(glm::vec3(camera._position) - zone);
        return dist < 4.0f;
    }
};
