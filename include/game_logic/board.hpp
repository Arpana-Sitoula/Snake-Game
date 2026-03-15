#pragma once
#include <vector>

/**
 * BOARD (Game Logic)
 * Purpose: Defines the physical size and boundaries of the game grid.
 * 
 * Flow:
 * - Stores how many cells wide and tall the grid is, and how big a cell is in 3D units
 * - grid_to_world(): Converts a grid coordinate (e.g. 5, 5) into real 3D space 
 *   so the graphics engine knows exactly where to draw it centered on the screen
 * - is_valid(): Checks if a coordinate is outside the map boundaries (used for wall collisions)
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
