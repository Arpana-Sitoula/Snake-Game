#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>
#include "../physics.hpp"

enum class CatState { IDLE, WALKING };

struct CatModel {
    glm::vec3 position;
    glm::vec3 rotation; // X=Pitch, Y=Yaw, Z=Roll
    glm::vec3 scale;
    
    CatState state = CatState::IDLE;
    glm::vec3 target_pos;
    float state_timer = 0;
    
    float speed = 0.6f;
    float anim_time = 0.0f;
    
    void init() {
        // Floor surface is at Y = -0.05f
        position = glm::vec3(0.0f, -0.05f, 0.0f);
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
            // Reset animation when standing still
            anim_time = 0;
            position.y = -0.05f;
            rotation.z = 0; 
        } else {
            glm::vec3 to_target = target_pos - position;
            to_target.y = 0; 
            float dist = glm::length(to_target);
            
            if (dist < 0.005f) {
                state = CatState::IDLE;
                state_timer = glm::linearRand(3.0f, 8.0f);
            } else {
                glm::vec3 next_pos = position + glm::normalize(to_target) * speed * delta;
                
                if (FloorPhysics::is_colliding(next_pos, 0.3f)) {
                    state = CatState::IDLE;
                    state_timer = 2.0f;
                    return;
                }
                
                position = next_pos;
                float target_yaw = atan2(to_target.x, to_target.z);
                rotation.y = target_yaw;

                // --- Walking animation of cat (Wobble & Bounce) ---
                anim_time += delta * 12.0f;
                position.y = -0.05f + abs(sin(anim_time)) * 0.08f; 
                rotation.z = sin(anim_time) * 0.15f; 
            }
        }
    }
    
    void pick_new_target() {
        for (int i = 0; i < 30; ++i) {
            glm::vec3 p = glm::vec3(glm::linearRand(-8.5f, 8.5f), -0.05f, glm::linearRand(-8.5f, 8.5f));
            // Ensure target isn't inside furniture
            if (!FloorPhysics::is_colliding(p, 0.6f)) {
                target_pos = p;
                state = CatState::WALKING;
                return;
            }
        }
        target_pos = glm::vec3(0, -0.05f, 0);
        state = CatState::WALKING;
    }
};