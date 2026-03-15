#pragma once
#include <random>

/**
 * FOOD (Game Logic)
 * Purpose: Manages the position and spawning of the collectible fruit.
 * 
 * Flow:
 * - Stores a single grid coordinate for the food
 * - spawn(): Randomly picks a new X/Y spot on the board, making sure it doesn't land on the snake
 * - check_eaten(): Simply returns true if the food's coordinate matches the snake's head
 */
struct Food {
    glm::ivec2 position = {5, 5};
    
    void spawn(const Board& board, const Snake& snake) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        
        std::uniform_int_distribution<> dist_x(0, board.width - 1);
        std::uniform_int_distribution<> dist_y(0, board.height - 1);
        
        // Keep trying until we find empty spot
        bool valid = false;
        while (!valid) {
            position.x = dist_x(gen);
            position.y = dist_y(gen);
            
            // Check not on snake
            valid = true;
            for (const auto& segment : snake.body) {
                if (position == segment) {
                    valid = false;
                    break;
                }
            }
        }
    }
    
    bool check_eaten(const Snake& snake) const {
        return position == snake.head();
    }
};
