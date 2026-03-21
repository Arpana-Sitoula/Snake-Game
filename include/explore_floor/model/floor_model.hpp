#pragma once
#include <glm/glm.hpp>
#include <vector>

/**
 * FLOOR MODEL
 * Purpose: Contains the data for the floor and objects in the scene.
 */
struct FloorModel {
    struct Sofa {
        glm::vec3 position;
        float rotation;
        glm::vec4 color;
    };

    std::vector<Sofa> sofas;
    glm::vec3 floor_pos;
    glm::vec3 floor_size;
    glm::vec4 floor_color;

    void init() {
        sofas.push_back({glm::vec3(0, 0, -3), 0, glm::vec4(0.4f, 0.2f, 0.1f, 1.0f)});
        sofas.push_back({glm::vec3(0, 0, 3), glm::radians(180.0f), glm::vec4(0.4f, 0.2f, 0.1f, 1.0f)});
        
        floor_pos = glm::vec3(0, -0.1f, 0);
        floor_size = glm::vec3(10, 0.1f, 10);
        floor_color = glm::vec4(0.2f, 0.2f, 0.25f, 1.0f);
    }
};
