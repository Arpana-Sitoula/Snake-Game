#pragma once
#include "../../graphics_render/transform.hpp"
#include <glm/glm.hpp>
#include <vector>

/**
 * SCONCE MODEL
 * Purpose: Stores the data for a wall light (Position, Color, etc.)
 */
struct SconceModel {
    Transform transform;
    glm::vec3 light_color = glm::vec3(1.0f, 0.7f, 0.3f);
    
    void init(glm::vec3 pos, float angle_deg, float scale = 0.015f) {
        transform._position = pos;
        transform._rotation = glm::vec3(0, glm::radians(angle_deg), 0);
        transform._scale = glm::vec3(scale);
    }
};
