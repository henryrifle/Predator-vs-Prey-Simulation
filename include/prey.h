#pragma once

#include <array>
#include <cmath>
#include <random>
#include "environment.h"

struct Prey {
    std::array<double, 2> position;
    std::array<double, 2> velocity;
    double speed;
    bool alive;
    double breeding_cooldown;
    int breeding_count;  // Track number of times bred
    double zone_time;         // Time spent in breeding zone
    bool is_new_prey;        // Flag for newly bred prey
    static const int MAX_BREEDING = 3;  // Maximum number of times a prey can breed
    static constexpr double MAX_ZONE_TIME = 5.0;  // Maximum time allowed in breeding zone (seconds)
    static constexpr double NEW_PREY_COOLDOWN = 60.0;  // 1 minute cooldown for new prey

    Prey() : position({0.0, 0.0}), velocity({0.0, 0.0}), speed(0.7), alive(true), breeding_cooldown(0.0), breeding_count(0), zone_time(0.0), is_new_prey(false) {}

    bool can_breed() const {
        return breeding_count < MAX_BREEDING && 
               breeding_cooldown <= 0 && 
               !is_new_prey;
    }

    // Only declare the function, don't define it here
    void update(double dt, const std::array<double, 2>& predator_pos, double bounds, const Environment& env);
};