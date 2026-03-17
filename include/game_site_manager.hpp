#pragma once
#include "snake_game.hpp"
#include "graphics_render/input.hpp"
#include "graphics_render/model.hpp"

/**
 * GAME SITE MANAGER
 * Purpose: Manages the high-level state of the application.
 * Switches between different modes like Snake Game or Test Mode.
 */
struct GameSiteManager {
    enum class State {
        SNAKE_GAME,
        TEST_MODE
    };

    State current_state = State::TEST_MODE;
    SnakeGame snake_game;
    Model test_box; // A simple square for test mode

    void init() {
        snake_game.init();
        test_box.init();
    }

    void destroy() {
        snake_game.destroy();
        test_box.destroy();
    }

    void handle_input() {
        // Toggle state with TAB key
        if (Keys::pressed(SDLK_TAB)) {
            if (current_state == State::SNAKE_GAME) {
                current_state = State::TEST_MODE;
                SDL_SetWindowRelativeMouseMode(SDL_GetGrabbedWindow(), true); // Lock mouse for 3D
            } else {
                current_state = State::SNAKE_GAME;
                SDL_SetWindowRelativeMouseMode(SDL_GetGrabbedWindow(), false); // Unlock mouse for 2D
            }
        }

        if (current_state == State::SNAKE_GAME) {
            snake_game.handle_input();
        }
    }

    void update(float delta, Camera& camera, const Audio& wow, SDL_AudioStream* wow_stream, const Audio& faaah, SDL_AudioStream* faaah_stream) {
        if (current_state == State::SNAKE_GAME) {
            // Reset camera for 2D game
            camera._position = glm::vec3(0, 0, 10);
            camera._rotation = glm::vec3(0, 0, 0);
            snake_game.update(delta, wow, wow_stream, faaah, faaah_stream);
            glDisable(GL_DEPTH_TEST);
        } else {
            glEnable(GL_DEPTH_TEST);
            
            // 3D Camera Controls (WASD + Mouse)
            float speed = 5.0f * delta;
            float sensitivity = 0.002f;

            if (Keys::down(SDLK_W)) camera.translate(0, 0, -speed);
            if (Keys::down(SDLK_S)) camera.translate(0, 0, speed);
            if (Keys::down(SDLK_A)) camera.translate(-speed, 0, 0);
            if (Keys::down(SDLK_D)) camera.translate(speed, 0, 0);

            // Mouse Look
            auto [dx, dy] = Mouse::delta();
            camera.rotate(-dy * sensitivity, -dx * sensitivity);
        }
    }

    // Helper to draw a box from primitives
    void draw_box(Pipeline& pipeline, glm::vec3 pos, glm::vec3 size, glm::vec4 color) {
        pipeline.set_color(color);
        
        // Front
        test_box.set_position(pos.x, pos.y, pos.z + size.z/2);
        test_box.transform._rotation = glm::vec3(0, 0, 0);
        test_box.set_scale(size.x, size.y, 1);
        test_box.draw();
        // Back
        test_box.set_position(pos.x, pos.y, pos.z - size.z/2);
        test_box.transform._rotation = glm::vec3(0, glm::radians(180.0f), 0);
        test_box.set_scale(size.x, size.y, 1);
        test_box.draw();
        // Top
        test_box.set_position(pos.x, pos.y + size.y/2, pos.z);
        test_box.transform._rotation = glm::vec3(glm::radians(-90.0f), 0, 0);
        test_box.set_scale(size.x, size.z, 1);
        test_box.draw();
        // Bottom
        test_box.set_position(pos.x, pos.y - size.y/2, pos.z);
        test_box.transform._rotation = glm::vec3(glm::radians(90.0f), 0, 0);
        test_box.set_scale(size.x, size.z, 1);
        test_box.draw();
        // Left
        test_box.set_position(pos.x - size.x/2, pos.y, pos.z);
        test_box.transform._rotation = glm::vec3(0, glm::radians(-90.0f), 0);
        test_box.set_scale(size.z, size.y, 1);
        test_box.draw();
        // Right
        test_box.set_position(pos.x + size.x/2, pos.y, pos.z);
        test_box.transform._rotation = glm::vec3(0, glm::radians(90.0f), 0);
        test_box.set_scale(size.z, size.y, 1);
        test_box.draw();
    }

    void draw_sofa(Pipeline& pipeline, glm::vec3 pos, float rotation) {
        auto rotate_vec = [&](glm::vec3 offset) {
            return glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0)) * glm::vec4(offset, 1.0f);
        };

        // Base
        draw_box(pipeline, pos + glm::vec3(rotate_vec(glm::vec3(0, 0.3f, 0))), glm::vec3(2.0f, 0.6f, 1.0f), glm::vec4(0.4f, 0.2f, 0.1f, 1.0f));
        // Backrest (offset in Z)
        draw_box(pipeline, pos + glm::vec3(rotate_vec(glm::vec3(0, 0.8f, -0.4f))), glm::vec3(2.0f, 1.0f, 0.2f), glm::vec4(0.3f, 0.15f, 0.1f, 1.0f));
        // Arms
        draw_box(pipeline, pos + glm::vec3(rotate_vec(glm::vec3(-0.9f, 0.6f, 0))), glm::vec3(0.2f, 0.6f, 1.0f), glm::vec4(0.35f, 0.18f, 0.1f, 1.0f));
        draw_box(pipeline, pos + glm::vec3(rotate_vec(glm::vec3(0.9f, 0.6f, 0))), glm::vec3(0.2f, 0.6f, 1.0f), glm::vec4(0.35f, 0.18f, 0.1f, 1.0f));
    }

    void draw(Pipeline& pipeline) {
        if (current_state == State::SNAKE_GAME) {
            snake_game.draw(pipeline);
        } else {
            // Draw two sofas facing each other
            draw_sofa(pipeline, glm::vec3(0, 0, -3), 0); 
            draw_sofa(pipeline, glm::vec3(0, 0, 3), glm::radians(180.0f)); // facing other way

            // Draw a floor
            draw_box(pipeline, glm::vec3(0, -0.1f, 0), glm::vec3(10, 0.1f, 10), glm::vec4(0.2f, 0.2f, 0.25f, 1.0f));
        }
    }
};
