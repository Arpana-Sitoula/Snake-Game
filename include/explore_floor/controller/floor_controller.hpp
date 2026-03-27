#pragma once
#include "../../graphics_render/input.hpp"
#include "../../graphics_render/camera.hpp"
#include "../model/physics.hpp"
#include <glm/glm.hpp>

/**
 * FLOOR CONTROLLER
 * Purpose: Handles the 3D camera / movement for exploring the floor (FPP).
 */
struct FloorController {
    void handle_input(Camera& camera, float delta) {
        // Save current position for possible collision reversion
        old_pos = camera._position;
        is_moving = false;

        // 3D Camera Controls (WASD + Mouse)
        float speed = 8.0f * delta;
        float sensitivity = 0.003f;

        if (Keys::down(SDLK_W)) { camera.translate(0, 0, -speed); is_moving = true; }
        if (Keys::down(SDLK_S)) { camera.translate(0, 0, speed);  is_moving = true; }
        if (Keys::down(SDLK_A)) { camera.translate(-speed, 0, 0); is_moving = true; }
        if (Keys::down(SDLK_D)) { camera.translate(speed, 0, 0);  is_moving = true; }

        // -- WALKING PHYSICS --
        
        // 1. Bobbing effect while Walking 
        if (is_moving) {
            walk_timer += delta * 2.0f; // Speed of steps
            float bob = glm::sin(walk_timer) * 0.10f;
            camera._position.y = 2.3f + bob;
        } else {
            // Smoothly return to standing height
            camera._position.y = glm::mix(camera._position.y, 2.5f, 5.0f * delta);
            walk_timer = 0;
        }

        // 2. Proactive Collision Detection
        if (FloorPhysics::is_colliding(camera._position, 0.00007f)) {
            camera._position = old_pos;
        }

        // Mouse Look
        auto [dx, dy] = Mouse::delta();
        camera.rotate(-dy * sensitivity, -dx * sensitivity);
    }

    bool is_active() const { return is_moving; }

private:
    glm::vec3 old_pos;
    bool is_moving = false;
    float walk_timer = 0.0f;
};
