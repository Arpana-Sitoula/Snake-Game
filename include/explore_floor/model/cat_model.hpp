#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>

enum class CatState { IDLE, WALKING };

struct CatModel {
    glm::vec3 position;
    glm::vec3 rotation; // X=Pitch, Y=Yaw, Z=Roll
    glm::vec3 scale;
    
    CatState state = CatState::IDLE;
    glm::vec3 target_pos;
    float state_timer = 0;
    
    float speed = 0.6f;
    
    void init() {
        // Floor surface is at Y = -0.05f
        position = glm::vec3(0.0f, -0.05f, 0.0f);
        
        // IMPORTANT: OpenGL/GLM uses RADIANS, not degrees. 
        // -90.0f degrees is about -1.57 radians.
        rotation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
        
        scale = glm::vec3(0.03f); 
        state = CatState::IDLE;
        state_timer = 2.0f; 
        pick_new_target();
    }
    
    void update(float delta) {
        if (state == CatState::IDLE) {
            state_timer -= delta;
            if (state_timer <= 0) {
                pick_new_target();
            }
        } else {
            glm::vec3 to_target = target_pos - position;
            to_target.y = 0; 
            float dist = glm::length(to_target);
            
            if (dist < 0.2f) {
                state = CatState::IDLE;
                state_timer = glm::linearRand(3.0f, 8.0f);
            } else {
                position += glm::normalize(to_target) * speed * delta;
                
                // Yaw should stay in radians
                float target_yaw = atan2(to_target.x, to_target.z);
                rotation.y = target_yaw;
            }
        }
        // Ensure it stays on floor
        position.y = -0.05f;
    }
    
    bool is_valid(glm::vec3 p) {
        // Wall bounds
        if (p.x < -8.5f || p.x > 8.5f || p.z < -8.5f || p.z > 8.5f) return false;
        // Avoid Sofa area
        if (p.x > -5.0f && p.x < 7.0f && p.z < -4.0f) return false;
        // Avoid Plant area
        if (p.x < -5.0f && p.x > -8.5f && p.z > 2.0f && p.z < 6.0f) return false;
        return true;
    }
    
    void pick_new_target() {
        for (int i = 0; i < 20; ++i) {
            glm::vec3 p = glm::vec3(glm::linearRand(-8.5f, 8.5f), -0.05f, glm::linearRand(-8.5f, 8.5f));
            if (is_valid(p)) {
                target_pos = p;
                state = CatState::WALKING;
                return;
            }
        }
        target_pos = glm::vec3(0, -0.05f, 0);
        state = CatState::WALKING;
    }
};