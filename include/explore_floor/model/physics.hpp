#pragma once
#include <glm/glm.hpp>

/**
 * EXPLORE FLOOR PHYSICS
 * Shared collision logic for player and cat.
 */
namespace FloorPhysics {
    inline bool is_colliding(glm::vec3 p, float radius = 0.5f) {
        // --- Inner Objects---
        
        // Sofa 1 
        if (p.x > -4.8f - radius && p.x < 4.8f + radius && 
            p.z < -5.2f + radius) {
            return true;
        }

        // Sofa 2 
        if (p.x > 1.4f - radius && p.x < 5.1f + radius && 
            p.z < -3.2f + radius && p.z > -8.2f - radius) {
            return true;
        }

        // Plant Pot
        float dist_to_plant = glm::distance(glm::vec2(p.x, p.z), glm::vec2(-6.5f, 4.0f));
        if (dist_to_plant < 1.0f + radius) return true;

        // --- Walls ---
        float wall = 9.8f - radius;
        if (p.x < -wall || p.x > wall) return true;
        
        if (p.z < -wall) return true; 
        
        if (p.z > wall) {
            // Door exit zone 
            if (glm::abs(p.x) > 1.8f) return true;
            // Limit how far player can walk out of door
            if (p.z > 14.0f) return true; 
        }

        return false;
    }
}
