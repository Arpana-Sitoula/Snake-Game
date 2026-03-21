#pragma once
#include <vector>

/**
 * BOARD (Model)
 * Purpose: Defines the physical size and boundaries of the game grid.
 */
struct Board {
    int width = 20;   // grid cells horizontally
    int height = 20;  // grid cells vertically
    float cell_size = 0.3f;  // size of each cell in world units
    
    // Get world position from grid position (centered at origin)
    glm::vec3 grid_to_world(int x, int y) const {
        float offset_x = -(width * cell_size) / 2.0f + cell_size / 2.0f;
        float offset_y = -(height * cell_size) / 2.0f + cell_size / 2.0f;
        return glm::vec3(offset_x + x * cell_size, offset_y + y * cell_size, 0.0f);
    }
    
    // Check if position is inside board
    bool is_valid(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
};
