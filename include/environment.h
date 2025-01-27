#pragma once
#include <array>
#include <cmath>

struct Environment {
    std::array<double, 2> breeding_zone_center;
    double breeding_zone_radius;
    double breeding_cooldown;
    double breeding_timer;
    
    Environment() : 
        breeding_zone_center({0.0, 0.0}),
        breeding_zone_radius(10.0),
        breeding_cooldown(5.0),
        breeding_timer(0.0) {}

    bool is_in_breeding_zone(const std::array<double, 2>& position) const {
        double dx = position[0] - breeding_zone_center[0];
        double dy = position[1] - breeding_zone_center[1];
        return (dx*dx + dy*dy) <= (breeding_zone_radius * breeding_zone_radius);
    }

    double get_multiplier(const std::array<double, 2>& position) const {
        if (is_in_breeding_zone(position)) {
            return 0.5;  // Slower in breeding zone
        }
        return 1.0;
    }

    void update(double dt) {
        breeding_timer -= dt;
        if (breeding_timer < 0) {
            breeding_timer = 0;
        }
    }
};