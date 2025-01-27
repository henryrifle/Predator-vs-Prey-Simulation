#include "predator.h"
#include "prey.h"
#include "environment.h"

void Predator::update(double dt, std::vector<Prey>& preys, double bounds, const Environment& env) {
    if (preys.empty()) return;

    // Find closest prey that's not in breeding zone
    Prey* closest_prey = nullptr;
    double min_distance = std::numeric_limits<double>::max();
    
    for (auto& prey : preys) {
        if (!prey.alive) continue;
        // Skip prey in breeding zone
        if (env.is_in_breeding_zone(prey.position)) continue;
        
        double dx = prey.position[0] - position[0];
        double dy = prey.position[1] - position[1];
        double distance = std::sqrt(dx*dx + dy*dy);
        
        if (distance < min_distance) {
            min_distance = distance;
            closest_prey = &prey;
        }
    }

    if (closest_prey) {
        // Calculate direction to closest prey
        double dx = closest_prey->position[0] - position[0];
        double dy = closest_prey->position[1] - position[1];
        double distance = std::sqrt(dx*dx + dy*dy);
        
        // Update velocity towards prey
        if (distance > 0) {
            velocity[0] = (dx / distance) * speed_p;
            velocity[1] = (dy / distance) * speed_p;
        }

        // If close enough, eat the prey
        if (distance < 1.0) {
            closest_prey->alive = false;
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