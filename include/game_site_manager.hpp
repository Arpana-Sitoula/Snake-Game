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
        test_box.set_position(0, 0);
        test_box.set_scale(2.0f);
    }

    void destroy() {
        snake_game.destroy();
        test_box.destroy();
    }

    void handle_input() {
        // Toggle state with TAB key for debugging
        if (Keys::pressed(SDLK_TAB)) {
            if (current_state == State::SNAKE_GAME) current_state = State::TEST_MODE;
            else current_state = State::SNAKE_GAME;
        }

        if (current_state == State::SNAKE_GAME) {
            snake_game.handle_input();
        }
    }

    void update(float delta, const Audio& wow, SDL_AudioStream* wow_stream, const Audio& faaah, SDL_AudioStream* faaah_stream) {
        if (current_state == State::SNAKE_GAME) {
            snake_game.update(delta, wow, wow_stream, faaah, faaah_stream);
        }
    }

    void draw(Pipeline& pipeline) {
        if (current_state == State::SNAKE_GAME) {
            snake_game.draw(pipeline);
        } else {
            // Render a simple test square (red)
            pipeline.set_color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            test_box.draw();
        }
    }
};
