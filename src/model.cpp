#include "predator.h"
#include "prey.h"
#include "environment.h"
#include "model.h"

#include <fstream>
#include <random>
#include <nlohmann/json.hpp>

Model::Model(const std::string& config_file)
    :time(0.0),
    preys{},
    predator{},
    environment{} {

    // Read configuration file
    std::ifstream config_stream(config_file);
    config_stream >> configuration;
    
    // Get bounds from configuration
    double bounds = configuration["region_limit"];
    
    // Initialize environment with breeding zone
    environment.breeding_zone_center = {0.0, 0.0};  // Center of the region
    environment.breeding_zone_radius = bounds * 0.2;  // 20% of region size
    environment.breeding_cooldown = 5.0;
    environment.breeding_timer = 0.0;
    
    // Initialize predator
    predator.position = {0.0, 0.0};
    predator.velocity = {0.0, 0.0};
    predator.speed_p = configuration["predator_speed"];
    
    // Initialize prey
    int num_prey = configuration["num_prey"];
    preys.reserve(num_prey);
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-bounds, bounds);
    
    for (int i = 0; i < num_prey; ++i) {
        Prey prey;
        prey.position = {dis(gen), dis(gen)};
        prey.velocity = {0.0, 0.0};
        prey.speed = configuration["prey_speed"];
        preys.push_back(prey);
    }
}

void Model::update(double time_delta) {
    // Update environment
    environment.update(time_delta);
    
    // Update prey positions
    for (auto& prey : preys) {
        prey.update(time_delta, predator.position, configuration["region_limit"], environment);
    }
    
    // Update predator position
    predator.update(time_delta, preys, configuration["region_limit"], environment);
    
    // Remove dead prey
    preys.erase(
        std::remove_if(preys.begin(), preys.end(),
            [](const Prey& prey) { return !prey.alive; }),
        preys.end()
    );
    
    // Handle breeding
    if (environment.breeding_timer <= 0) {
        std::vector<Prey> new_prey;
        for (auto& prey : preys) {
            if (environment.is_in_breeding_zone(prey.position) && prey.can_breed()) {
                // Create a new prey
                Prey offspring;
                offspring.position = prey.position;
                offspring.speed = prey.speed;
                offspring.is_new_prey = true;
                offspring.breeding_cooldown = Prey::NEW_PREY_COOLDOWN;  // 1 minute cooldown
                new_prey.push_back(offspring);
                
                // Update parent prey
                prey.breeding_count++;
                prey.breeding_cooldown = 5.0;
            }
        }
        
        // Add new prey and reset breeding timer if any breeding occurred
        if (!new_prey.empty()) {
            preys.insert(preys.end(), new_prey.begin(), new_prey.end());
            environment.breeding_timer = environment.breeding_cooldown;
        }
    }
}

const Predator& Model::getPredator() const {
    return predator;
}




