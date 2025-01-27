#include "prey.h"

void Prey::update(double dt, const std::array<double, 2>& predator_pos, double bounds, const Environment& env) {
    if (!alive) return;

    // Update breeding cooldown for new prey
    if (is_new_prey) {
        breeding_cooldown -= dt;
        if (breeding_cooldown <= 0) {
            is_new_prey = false;
            breeding_cooldown = 0;
        }
    }

    // Update zone time if in breeding zone
    if (env.is_in_breeding_zone(position)) {
        zone_time += dt;
    } else {
        zone_time = 0;  // Reset when outside zone
    }

    // Calculate direction from predator
    double dx = position[0] - predator_pos[0];
    double dy = position[1] - predator_pos[1];
    double distance = std::sqrt(dx*dx + dy*dy);

    // Get speed multiplier from environment
    double speed_mult = env.get_multiplier(position);

    // Force prey to leave breeding zone if stayed too long
    if (zone_time > MAX_ZONE_TIME) {
        // Move away from breeding zone center
        dx = position[0] - env.breeding_zone_center[0];
        dy = position[1] - env.breeding_zone_center[1];
        double dist = std::sqrt(dx*dx + dy*dy);
        if (dist > 0) {
            velocity[0] = (dx / dist) * speed;
            velocity[1] = (dy / dist) * speed;
        }
    }
    // Normal movement logic
    else if (distance < 20.0 && !env.is_in_breeding_zone(position)) {
        // Flee from predator
        if (distance > 0) {
            velocity[0] = (dx / distance) * speed * speed_mult;
            velocity[1] = (dy / distance) * speed * speed_mult;
        }
    } else {
        // Random movement or move towards breeding zone
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(-1, 1);
        
        if (!env.is_in_breeding_zone(position) && breeding_cooldown <= 0 && !is_new_prey) {
            // Move towards breeding zone
            dx = env.breeding_zone_center[0] - position[0];
            dy = env.breeding_zone_center[1] - position[1];
            distance = std::sqrt(dx*dx + dy*dy);
            if (distance > 0) {
                velocity[0] = (dx / distance) * speed * speed_mult;
                velocity[1] = (dy / distance) * speed * speed_mult;
            }
        } else {
            velocity[0] += dis(gen) * 0.1;
            velocity[1] += dis(gen) * 0.1;
        }
        
        // Normalize velocity
        double v_mag = std::sqrt(velocity[0]*velocity[0] + velocity[1]*velocity[1]);
        if (v_mag > speed * speed_mult) {
            velocity[0] = (velocity[0] / v_mag) * speed * speed_mult;
            velocity[1] = (velocity[1] / v_mag) * speed * speed_mult;
        }
    }

    // Update breeding cooldown
    if (!is_new_prey) {
        breeding_cooldown -= dt;
        if (breeding_cooldown < 0) {
            breeding_cooldown = 0;
        }
    }

    // Update position
    position[0] += velocity[0] * dt;
    position[1] += velocity[1] * dt;

    // Bound check
    for (int i = 0; i < 2; i++) {
        if (position[i] > bounds) {
            position[i] = bounds;
            velocity[i] *= -1;
        }
        if (position[i] < -bounds) {
            position[i] = -bounds;
            velocity[i] *= -1;
        }
    }
}