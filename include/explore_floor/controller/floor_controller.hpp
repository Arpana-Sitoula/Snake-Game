#pragma once
#include "../../graphics_render/input.hpp"
#include "../../graphics_render/camera.hpp"

/**
 * FLOOR CONTROLLER
 * Purpose: Handles the 3D camera / movement for exploring the floor.
 */
struct FloorController {
    void handle_input(Camera& camera, float delta) {
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
};
