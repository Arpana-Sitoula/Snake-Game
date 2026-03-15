#pragma once
#include "mesh.hpp"
#include "transform.hpp"

/**
 * MODEL (Graphics Render)
 * Purpose: Represents an actual object in the world by combining a shape (Mesh) with a 
 * position, rotation, and size (Transform).
 * 
 * Flow:
 * - Contains a Mesh (what to draw) and a Transform (where to draw it)
 * - Has helper functions to quickly scale and position the object
 * - draw(): Binds its transform and tells its mesh to draw
 */
struct Model {
    Mesh mesh;
    Transform transform;
    
    void init() {
        mesh.init();
    }
    
    void destroy() {
        mesh.destroy();
    }
    
    // Set position in world space
    void set_position(float x, float y) {
        transform._position = glm::vec3(x, y, 0);
    }
    
    // Set size (uniform scale)
    void set_scale(float size) {
        transform._scale = glm::vec3(size, size, 1);
    }
    
    // Draw this model (caller must set color via pipeline first!)
    void draw() {
        transform.bind();
        mesh.bind();
        mesh.draw();
    }
};
