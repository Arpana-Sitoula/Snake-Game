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

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>
#include <algorithm>

struct Mesh {
    void init() {
        // Default: Simple 2D quad
        float p = +0.5f;
        float n = -0.5f;
        
        std::vector<Vertex> vertices = {
            Vertex{ glm::vec3{n, n, 0}, glm::vec3(0, 0, 1), glm::vec2{0, 1}, glm::vec4{1, 1, 1, 1} }, // bottom-left -> UV(0,1)
            Vertex{ glm::vec3{p, n, 0}, glm::vec3(0, 0, 1), glm::vec2{1, 1}, glm::vec4{1, 1, 1, 1} }, // bottom-right -> UV(1,1)
            Vertex{ glm::vec3{n, p, 0}, glm::vec3(0, 0, 1), glm::vec2{0, 0}, glm::vec4{1, 1, 1, 1} }, // top-left -> UV(0,0)
            Vertex{ glm::vec3{p, p, 0}, glm::vec3(0, 0, 1), glm::vec2{1, 0}, glm::vec4{1, 1, 1, 1} }, // top-right -> UV(1,0)
        };
        std::vector<Index> indices = { 0, 1, 3, 3, 2, 0 };
        
        init_gpu_buffers(vertices, indices);
    }

    void load_obj(const std::string& path, const std::string& target_obj = "", const std::string& target_mtl = "") {
        std::vector<glm::vec3> temp_positions;
        std::vector<glm::vec2> temp_uvs;
        std::vector<glm::vec3> temp_normals;
        
        std::vector<Vertex> vertices;
        std::vector<Index> indices;
        std::map<std::string, Index> unique_vertices;

        std::ifstream file(path);
        if (!file.is_open()) return;

        std::string line;
        std::string current_obj = "";
        std::string current_mtl = "";
        // If filters are empty, we load everything by default
        bool loading_target = (target_obj == "" && target_mtl == "");

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "o") {
                ss >> current_obj;
                loading_target = (target_obj == "" || current_obj == target_obj) && (target_mtl == "" || current_mtl == target_mtl);
            } else if (prefix == "usemtl") {
                ss >> current_mtl;
                loading_target = (target_obj == "" || current_obj == target_obj) && (target_mtl == "" || current_mtl == target_mtl);
            } else if (prefix == "v") {
                glm::vec3 v;
                ss >> v.x >> v.y >> v.z;
                temp_positions.push_back(v);
            } else if (prefix == "vt") {
                glm::vec2 vt;
                ss >> vt.x >> vt.y;
                temp_uvs.push_back(vt);
            } else if (prefix == "vn") {
                glm::vec3 vn;
                ss >> vn.x >> vn.y >> vn.z;
                temp_normals.push_back(vn);
            } else if (prefix == "f" && loading_target) {
                std::vector<std::string> face_vertices;
                std::string vert_str;
                while (ss >> vert_str) face_vertices.push_back(vert_str);

                auto parse_vert = [&](const std::string& s) -> Index {
                    if (unique_vertices.contains(s)) return unique_vertices[s];

                    Vertex v{};
                    v.color = glm::vec4(1.0f);
                    
                    int pi = 0, ti = 0, ni = 0;
                    if (s.find("//") != std::string::npos) {
                        sscanf(s.c_str(), "%d//%d", &pi, &ni);
                    } else if (std::count(s.begin(), s.end(), '/') == 2) {
                        sscanf(s.c_str(), "%d/%d/%d", &pi, &ti, &ni);
                    } else if (std::count(s.begin(), s.end(), '/') == 1) {
                        sscanf(s.c_str(), "%d/%d", &pi, &ti);
                    } else {
                        sscanf(s.c_str(), "%d", &pi);
                    }

                    if (pi > 0) v.position = temp_positions[pi - 1]; else if (pi < 0) v.position = temp_positions[temp_positions.size() + pi];
                    if (ti > 0) v.uv = glm::vec2(temp_uvs[ti - 1].x, 1.0f - temp_uvs[ti - 1].y); else if (ti < 0) v.uv = glm::vec2(temp_uvs[temp_uvs.size() + ti].x, 1.0f - temp_uvs[temp_uvs.size() + ti].y);
                    if (ni > 0) v.normal = temp_normals[ni - 1]; else if (ni < 0) v.normal = temp_normals[temp_normals.size() + ni];

                    Index idx = (Index)vertices.size();
                    vertices.push_back(v);
                    unique_vertices[s] = idx;
                    return idx;
                };

                for (size_t i = 1; i < face_vertices.size() - 1; ++i) {
                    indices.push_back(parse_vert(face_vertices[0]));
                    indices.push_back(parse_vert(face_vertices[i]));
                    indices.push_back(parse_vert(face_vertices[i + 1]));
                }
            }
        }
        init_gpu_buffers(vertices, indices);
    }

    void init_gpu_buffers(const std::vector<Vertex>& vertices, const std::vector<Index>& indices) {
        _index_count = indices.size();
        glCreateBuffers(1, &_buffer_vertices);
        glNamedBufferStorage(_buffer_vertices, vertices.size() * sizeof(Vertex), vertices.data(), BufferStorageMask::GL_NONE_BIT);
        glCreateBuffers(1, &_buffer_indices);
        glNamedBufferStorage(_buffer_indices, indices.size() * sizeof(Index), indices.data(), BufferStorageMask::GL_NONE_BIT);
        glCreateVertexArrays(1, &_buffer_mesh);
        glVertexArrayVertexBuffer(_buffer_mesh, 0, _buffer_vertices, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(_buffer_mesh, _buffer_indices);
        glVertexArrayAttribFormat(_buffer_mesh, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(_buffer_mesh, 0, 0);
        glEnableVertexArrayAttrib(_buffer_mesh, 0);
        glVertexArrayAttribFormat(_buffer_mesh, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex::position));
        glVertexArrayAttribBinding(_buffer_mesh, 1, 0);
        glEnableVertexArrayAttrib(_buffer_mesh, 1);
        glVertexArrayAttribFormat(_buffer_mesh, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex::position) + sizeof(Vertex::normal));
        glVertexArrayAttribBinding(_buffer_mesh, 2, 0);
        glEnableVertexArrayAttrib(_buffer_mesh, 2);
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