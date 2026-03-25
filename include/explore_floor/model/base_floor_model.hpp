#pragma once
#include <glm/glm.hpp>

/**
 * BASE FLOOR MODEL
 * Purpose: Contains the data for the floor itself.
 */
struct BaseFloorModel {
    glm::vec3 position;
    glm::vec3 size;
    glm::vec4 color;

    void init() {
        position = glm::vec3(0, -0.1f, 0);
        size = glm::vec3(20, 0.1f, 20);
        color = glm::vec4(0.2f, 0.2f, 0.25f, 1.0f);
    }
};
