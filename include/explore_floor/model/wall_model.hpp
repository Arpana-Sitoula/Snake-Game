#pragma once
#include <glm/glm.hpp>
#include <vector>

/**
 * WALL MODEL
 * Purpose: Contains the data for a single wall.
 */
struct Wall {
    glm::vec3 position;
    glm::vec3 size;
    glm::vec4 color;
};

/**
 * WALL COLLECTION MODEL
 * Purpose: Manages the walls in the scene.
 */
struct WallModel {
    std::vector<Wall> walls;

    void init() {
        // Rear Wall
        walls.push_back({glm::vec3(0, 2.5f, -10), glm::vec3(20.1f, 5, 0.1f), glm::vec4(0.3f, 0.3f, 0.35f, 1.0f)});
        // Left Wall
        walls.push_back({glm::vec3(-10, 2.5f, 0), glm::vec3(0.1f, 5, 20.1f), glm::vec4(0.3f, 0.3f, 0.35f, 1.0f)});
        // Right Wall
        walls.push_back({glm::vec3(10, 2.5f, 0), glm::vec3(0.1f, 5, 20.1f), glm::vec4(0.3f, 0.3f, 0.35f, 1.0f)});
        
        // Front Wall (Door Entrance)
        // Left part of front wall (x from -10 to -2)
        walls.push_back({glm::vec3(-6.0f, 2.5f, 10), glm::vec3(8, 5, 0.1f), glm::vec4(0.3f, 0.3f, 0.35f, 1.0f)});
        // Right part of front wall (x from 2 to 10)
        walls.push_back({glm::vec3(6.0f, 2.5f, 10), glm::vec3(8, 5, 0.1f), glm::vec4(0.3f, 0.3f, 0.35f, 1.0f)});
        // Top part above door (y from 3 to 5)
        walls.push_back({glm::vec3(0, 4.0f, 10), glm::vec3(4, 2, 0.1f), glm::vec4(0.3f, 0.3f, 0.35f, 1.0f)});

        // Ceiling
        walls.push_back({glm::vec3(0, 5.05f, 0), glm::vec3(20.1f, 0.1f, 20.1f), glm::vec4(0.25f, 0.25f, 0.3f, 1.0f)});
    }
};
