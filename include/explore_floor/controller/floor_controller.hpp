#pragma once
#include "../../graphics_render/input.hpp"
#include "../../graphics_render/camera.hpp"
#include "../physics.hpp"
#include <glm/glm.hpp>

/**
 * FLOOR CONTROLLER
 * Purpose: Handles the 3D camera / movement for exploring the floor (FPP).
 */
struct FloorController {
    void handle_input(Camera& camera, float delta) {
        // Save current position for possible collision reversion
        old_pos = camera._position;

        // 3D Camera Controls (WASD + Mouse)
        float speed = 8.0f * delta;
        float sensitivity = 0.003f;

        if (Keys::down(SDLK_W)) camera.translate(0, 0, -speed);
        if (Keys::down(SDLK_S)) camera.translate(0, 0, speed);
        if (Keys::down(SDLK_A)) camera.translate(-speed, 0, 0);
        if (Keys::down(SDLK_D)) camera.translate(speed, 0, 0);

        // -- WALKING PHYSICS --
        // 1. Lock height to eye level
        camera._position.y = 2.5f;

        // 2. Proactive Collision Detection
        if (FloorPhysics::is_colliding(camera._position, 0.6f)) {
            // If movement resulted in collision, REVERT (simple fix)
            camera._position = old_pos;
        }

        // Mouse Look
        auto [dx, dy] = Mouse::delta();
        camera.rotate(-dy * sensitivity, -dx * sensitivity);
    }
private:
    glm::vec3 old_pos;
};
