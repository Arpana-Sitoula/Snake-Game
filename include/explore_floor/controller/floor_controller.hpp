#pragma once
#include "../../graphics_render/input.hpp"
#include "../../graphics_render/camera.hpp"
#include <glm/glm.hpp>

/**
 * FLOOR CONTROLLER
 * Purpose: Handles the 3D camera / movement for exploring the floor (FPP).
 */
struct FloorController {
    void handle_input(Camera& camera, float delta) {
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

        // 2. Collision Detection 
        float boundary = 9.8f;
        float door_width = 2.0f;

        // Clamp Z (Forward/Back)
        if (glm::abs(camera._position.x) > door_width) {
            // If not in the door opening, cannot go past z = 9.8
            camera._position.z = glm::clamp(camera._position.z, -boundary, boundary);
        } else {
            // In the door opening, can go further out (to "spawn" area)
            camera._position.z = glm::clamp(camera._position.z, -boundary, 15.0f);
        }

        // Clamp X (Left/Right)
        if (camera._position.z < boundary) {
            // Inside the room, clamp to walls
            camera._position.x = glm::clamp(camera._position.x, -boundary, boundary);
        } else {
            // Entering/Leaving, clamp to door width
            camera._position.x = glm::clamp(camera._position.x, -door_width, door_width);
        }

        // Mouse Look
        auto [dx, dy] = Mouse::delta();
        camera.rotate(-dy * sensitivity, -dx * sensitivity);
    }
};
