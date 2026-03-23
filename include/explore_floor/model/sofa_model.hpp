#pragma once
#include <glm/glm.hpp>
#include <vector>

/**
 * SOFA MODEL
 * Purpose: Contains the data for a single sofa.
 */
struct Sofa {
    glm::vec3 position;
    float rotation;
    glm::vec4 color;
    float width = 12.0f;
    bool has_left_arm = true;
    bool has_right_arm = true;
};

/**
 * SOFA COLLECTION MODEL
 * Purpose: Manages multiple sofas in the scene.
 */
struct SofaModel {
    std::vector<Sofa> sofas;

    void init() {
        // Sofa 1 (Back segment, facing entrance)
        sofas.push_back({glm::vec3(0.00f, 0, -7.25f), 0, glm::vec4(0.5f, 0.25f, 0.15f, 1.0f), 7.5f, true, false});

        // Sofa 2 (Side segment, perpendicular)
        sofas.push_back({glm::vec3(3.25f, 0, -5.75f), glm::radians(-90.0f), glm::vec4(0.5f, 0.25f, 0.15f, 1.0f), 4.5f, true, true});
    }
};
