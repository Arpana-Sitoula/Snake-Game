#pragma once
#include <glm/glm.hpp>
#include <vector>

/**
 * CARPET MODEL
 * Purpose: Contains the data for a carpet.
 */
struct Carpet {
    glm::vec3 position;
    glm::vec3 size;
    glm::vec4 color;
};

/**
 * CARPET COLLECTION MODEL
 * Purpose: Manages carpets in the scene.
 */
struct CarpetModel {
    std::vector<Carpet> carpets;

    void init() {
        carpets.push_back({glm::vec3(0, -0.05f, -1), glm::vec3(8, 0.01f, 12), glm::vec4(0.6f, 0.15f, 0.15f, 1.0f)});
    }
};
