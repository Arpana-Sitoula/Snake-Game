#pragma once

/**
 * CAMERA (Graphics Render)
 * Purpose: Acts as the "eye" through which the player sees the 3D world.
 * 
 * Flow:
 * - Stores its position, rotation, and lens properties (field of view, aspect ratio)
 * - bind(): Calculates two special Matrices:
 *   1. Transform Matrix: Moves everything in the world *opposite* to the camera to simulate looking around
 *   2. Projection Matrix: Squeezes the 3D world into the 2D window (giving perspective)
 * - Sends these matrices to the GPU so it knows how to draw objects from this viewpoint
 */
struct Camera {
    // translate relative to camera direction
    void translate(float x, float y, float z) {
        // Calculate the rotation quaternion for the current euler angles
        glm::quat q = glm::quat(glm::vec3(_rotation.x, _rotation.y, _rotation.z));
        _position += q * glm::vec3(x, y, z);
    }

    void rotate(float pitch, float yaw) {
        _rotation.x += pitch;
        _rotation.y += yaw;
        // Limit pitch to prevent flipping
        _rotation.x = glm::clamp(_rotation.x, glm::radians(-89.0f), glm::radians(89.0f));
    }

    void bind() {
        // projection matrix
        glm::mat4x4 projection_matrix = glm::perspectiveFov(_fov, _width, _height, _near_plane, _far_plane);

        // transformation matrix (inverse of normal transform, since it is the camera)
        glm::mat4x4 transform_matrix(1.0f);
        transform_matrix = glm::rotate(transform_matrix, - _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        transform_matrix = glm::rotate(transform_matrix, - _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        transform_matrix = glm::translate(transform_matrix, - _position);
        // upload to GPU
        glUniformMatrix4fv(8, 1, false, glm::value_ptr(transform_matrix));
        glUniformMatrix4fv(12, 1, false, glm::value_ptr(projection_matrix));
    }

    glm::vec3 _position{ 0, 0, 0 };
    glm::vec3 _rotation{ 0, 0, 0 }; // euler rotation
    float _near_plane = 0.1f;  // nearest distance the camera can render
    float _far_plane = 100.0f; // furthest distance the camera can render
    float _fov = 70.0f; // field of view
    float _width = 1080.0f; // should be equal to window width
    float _height = 1080.0f; // should be equal to window height
};