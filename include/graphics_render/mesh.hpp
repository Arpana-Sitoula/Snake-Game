#pragma once

/**
 * MESH (Graphics Render)
 * Purpose: Defines the raw 3D geometry (vertices, triangles) to be drawn by OpenGL.
 * 
 * Flow:
 * - Stores the raw data points (Vertex: position, normal, uv, color)
 * - init(): Sends this data from the CPU to the graphics card (GPU)
 * - bind() & draw(): Tells the GPU to draw the loaded geometry
 * 
 * Note: A mesh is just the shape itself. It has no position or scale in the world.
 */

// sizeof(Vertex) = sizeof(float) * 4 = 16 bytes
struct Vertex {
    glm::vec3 position; // x, y, z
    glm::vec3 normal; // x, y, z
    glm::vec2 uv; // u, v coordinates for textures
    glm::vec4 color; // r, g, b, a
};
// sizeof(Index) = 4 bytes
using Index = GLuint;

struct Mesh {
    void init() {
        float p = +0.5f; // for readability
        float n = -0.5f; // for readability
        
        // Simple 2D quad (4 vertices instead of 24 for cube)
        // Faces toward +Z (camera direction)
        std::vector<Vertex> vertices = {
            Vertex{ glm::vec3{n, n, 0}, glm::vec3(0, 0, 1), glm::vec2{0, 0}, glm::vec4{1, 1, 1, 1} }, // bottom-left
            Vertex{ glm::vec3{p, n, 0}, glm::vec3(0, 0, 1), glm::vec2{1, 0}, glm::vec4{1, 1, 1, 1} }, // bottom-right
            Vertex{ glm::vec3{n, p, 0}, glm::vec3(0, 0, 1), glm::vec2{0, 1}, glm::vec4{1, 1, 1, 1} }, // top-left
            Vertex{ glm::vec3{p, p, 0}, glm::vec3(0, 0, 1), glm::vec2{1, 1}, glm::vec4{1, 1, 1, 1} }, // top-right
        };
        // 2 triangles = 6 indices
        std::vector<Index> indices = { 
            0, 1, 3,  // first triangle
            3, 2, 0,  // second triangle
        };
        _index_count = indices.size();

        // create GPU buffer to store vertices
        glCreateBuffers(1, &_buffer_vertices);
        // upload to GPU
        glNamedBufferStorage(_buffer_vertices, 
            vertices.size() * sizeof(Vertex), // number of bytes to upload
            vertices.data(), // raw data to upload
            BufferStorageMask::GL_NONE_BIT);
        
        // create index buffer the same way
        glCreateBuffers(1, &_buffer_indices);
        // upload to GPU
        glNamedBufferStorage(_buffer_indices,
            indices.size() * sizeof(Index),
            indices.data(),
            BufferStorageMask::GL_NONE_BIT);

        // create vertex array (basically a mesh)
        glCreateVertexArrays(1, &_buffer_mesh);
        // assign vertex and index buffer
        glVertexArrayVertexBuffer(_buffer_mesh, 0, _buffer_vertices, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(_buffer_mesh, _buffer_indices);

        // describe the data inside each Vertex
        // Vertex::position
        glVertexArrayAttribFormat(_buffer_mesh, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(_buffer_mesh, 0, 0);
        glEnableVertexArrayAttrib(_buffer_mesh, 0);
        // Vertex::normal
        glVertexArrayAttribFormat(_buffer_mesh, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex::position));
        glVertexArrayAttribBinding(_buffer_mesh, 1, 0);
        glEnableVertexArrayAttrib(_buffer_mesh, 1);
        // Vertex::uv
        glVertexArrayAttribFormat(_buffer_mesh, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex::position) + sizeof(Vertex::normal));
        glVertexArrayAttribBinding(_buffer_mesh, 2, 0);
        glEnableVertexArrayAttrib(_buffer_mesh, 2);
        // Vertex::color
        glVertexArrayAttribFormat(_buffer_mesh, 3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex::position) + sizeof(Vertex::normal) + sizeof(Vertex::uv));
        glVertexArrayAttribBinding(_buffer_mesh, 3, 0);
        glEnableVertexArrayAttrib(_buffer_mesh, 3);
    }
    void destroy() {
        glDeleteBuffers(1, &_buffer_indices);
        glDeleteBuffers(1, &_buffer_vertices);
        glDeleteVertexArrays(1, &_buffer_mesh);
    }

    void bind() {
        glBindVertexArray(_buffer_mesh);
    }
    void draw() {
        glDrawElements(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr);
    }

    GLuint _buffer_vertices;
    GLuint _buffer_indices;
    GLuint _buffer_mesh;
    GLuint _index_count;
};