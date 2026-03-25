#pragma once
#include <glm/glm.hpp>

struct PlantModel {
    glm::vec3 position;
    glm::vec3 scale;

    void init() {
        position = glm::vec3(-6.5f, 0.0f, 4.0f);
        scale = glm::vec3(0.5f, 0.5f, 0.5f); 
    }
};
