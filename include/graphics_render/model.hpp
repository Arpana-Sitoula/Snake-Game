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
    Mesh* _mesh_p = nullptr;
    Mesh _internal_mesh;
    Transform transform;
    
    void init() {
        _internal_mesh.init();
        _mesh_p = &_internal_mesh;
    }

    void init(Mesh* mesh) {
        _mesh_p = mesh;
    }
    
    void destroy() {
        _internal_mesh.destroy();
    }
    
    // Set position in world space
    void set_position(const glm::vec3& pos) {
        transform._position = pos;
    }

    void set_position(float x, float y, float z) {
        transform._position = glm::vec3(x, y, z);
    }
    
    void set_position(float x, float y) {
        transform._position = glm::vec3(x, y, 0);
    }
    
    // Set size (uniform scale)
    void set_scale(float size) {
        transform._scale = glm::vec3(size, size, size);
    }

    // Set size (per-axis scale)
    void set_scale(float x, float y, float z) {
        transform._scale = glm::vec3(x, y, z);
    }
    
    // Draw this model 
    void draw() {
        transform.bind();
        if (_mesh_p) {
            _mesh_p->bind();
            _mesh_p->draw();
        }
    }
};
