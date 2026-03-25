#pragma once
#include <glm/glm.hpp>

struct Screen {
    glm::vec3 position;
    glm::vec3 size;
    glm::vec4 color;
};

struct ScreenModel {
    Screen screen;

    void init() {
        screen = {glm::vec3(-9.85f, 2.5f, 0.0f), glm::vec3(0.1f, 3.5f, 6.5f), glm::vec4(0.01f, 0.01f, 0.01f, 1.0f)};
    }
};
