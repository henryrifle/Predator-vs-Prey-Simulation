#include <fmt/format.h>
#include "prey.h"
#include "enviroment.h"
#include "model.h"
#include "predator.h"
#include <cmath> 
#include <random> 

constexpr double MIN_FLEE_DISTANCE = 7.0;
constexpr double TWO_PI = 2.0 * M_PI;


auto Prey::update(const double time_delta, Environment& environment, Predator& predator) -> void
{
    double distance_to_predator = std::hypot(position[0] - predator.position[0], position[1] - predator.position[1]);
    if (distance_to_predator < MIN_FLEE_DISTANCE) {
        // Calculate vector pointing away from the predator
        double dx = position[0] - predator.position[0];
        double dy = position[1] - predator.position[1];
        double distance = std::hypot(dx, dy);
        dx /= distance;
        dy /= distance;


        // Calculate velocity components based on speed and direction
        velocity[0] = speed * dx;
        velocity[1] = speed * dy;


        auto modification = environment.get_multiplier(position);
        position[0] += velocity[0] * modification[0] * time_delta;
        position[1] += velocity[1] * modification[1] * time_delta;
    }
    else {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, TWO_PI);
        double random_direction = dis(gen);

        // Calculate velocity components based on random direction and speed
        velocity[0] = speed * std::cos(random_direction);
        velocity[1] = speed * std::sin(random_direction);
        // Move randomly if predator is not too close
        auto modification = environment.get_multiplier(position);
        position[0] += velocity[0] * modification[0] * time_delta;
        position[1] += velocity[1] * modification[1] * time_delta;
    }
    environment.restrict_position(position);
    environment.reflect(position, velocity);
    //environment.breed_zone(position);



}


auto Prey::log(const double time) -> void
{
    fmt::print("{:.2f},{:.2f},{:.2f}\n", time, position[0], position[1]);
}

// An attempt to create a breeding system, couldnt figure out how to add into model
/*
auto Prey::breed(std::vector<Prey>& preys)-> void
{
    if (preys.size() < 1000) {
        // Create a new prey and add it to the simulation
        
        Prey new_prey;
        preys.push_back(new_prey);
        fmt::print("breed\n");
    }


}*/