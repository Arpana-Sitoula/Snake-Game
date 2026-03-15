#pragma once
#include <vector>
#include <deque>

/**
 * SNAKE (Game Logic)
 * Purpose: Manages the player's snake state, movement logic, and growth.
 * 
 * Flow:
 * - Stores the snake's body as a list of grid coordinates (head is front, tail is back)
 * - Tracks direction, movement speed (timer), score, and alive status
 * - update(): Checks the timer and moves the snake automatically every interval
 * - move(): Calculates where the new head goes, checks self-collision, and removes tail
 */
struct Snake {
    enum class Direction { UP, DOWN, LEFT, RIGHT };
    
    std::deque<glm::ivec2> body;  // head is front, tail is back
    Direction direction = Direction::RIGHT;
    Direction next_direction = Direction::RIGHT;
    float move_timer = 0.0f;
    float move_interval = 0.15f;  // seconds between moves
    bool alive = true;
    int score = 0;
    
    void init(int start_x, int start_y) {
        body.clear();
        body.push_front(glm::ivec2(start_x, start_y));
        body.push_front(glm::ivec2(start_x + 1, start_y));
        body.push_front(glm::ivec2(start_x + 2, start_y));
        direction = Direction::RIGHT;
        next_direction = Direction::RIGHT;
        alive = true;
        score = 0;
    }
    
    void set_direction(Direction dir) {
        // Prevent 180-degree turns
        if ((dir == Direction::UP && direction != Direction::DOWN) ||
            (dir == Direction::DOWN && direction != Direction::UP) ||
            (dir == Direction::LEFT && direction != Direction::RIGHT) ||
            (dir == Direction::RIGHT && direction != Direction::LEFT)) {
            next_direction = dir;
        }
    }
    
    void update(float delta, const Board& board) {
        if (!alive) return;
        
        move_timer += delta;
        if (move_timer >= move_interval) {
            move_timer = 0.0f;
            direction = next_direction;
            move(board);
        }
    }
    
    void move(const Board& board) {
        glm::ivec2 head = body.front();
        glm::ivec2 new_head = head;
        
        switch (direction) {
            case Direction::UP:    new_head.y += 1; break;
            case Direction::DOWN:  new_head.y -= 1; break;
            case Direction::LEFT:  new_head.x -= 1; break;
            case Direction::RIGHT: new_head.x += 1; break;
        }
        
        // Check wall collision
        if (!board.is_valid(new_head.x, new_head.y)) {
            alive = false;
            return;
        }
        
        // Check self collision
        for (const auto& segment : body) {
            if (new_head == segment) {
                alive = false;
                return;
            }
        }
        
        body.push_front(new_head);
        body.pop_back();  // Remove tail (unless eating)
    }
    
    void grow() {
        // Add segment at tail position
        body.push_back(body.back());
        score++;
    }
    
    glm::ivec2 head() const { return body.front(); }
};
