#include <fmt/format.h>
#include "predator.h"
#include "enviroment.h"
#include "prey.h"
#include "model.h"

#include <fstream>

#include <nlohmann/json.hpp>

constexpr double MIN_HUNT_DISTANCE = 7.0;
constexpr double TWO_PI = 2.0 * M_PI;




auto Predator::update(const double time_delta, const Environment& environment, std::vector<Prey>& preys) -> void
{

    if (!preys.empty()) {
        // Choose a random prey to hunt
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, preys.size() - 1);
        int prey_index = dis(gen);
        const auto& prey_position = preys[prey_index];


        double dx = prey_position.position[0] - position[0];
        double dy = prey_position.position[1] - position[1];
        double distance = std::hypot(dx, dy);


        if (distance > MIN_HUNT_DISTANCE) {
            // Normalize direction vector
            dx /= distance;
            dy /= distance;

            // Calculate velocity components based on speed
            velocity[0] = speed_p * dx;
            velocity[1] = speed_p * dy;

            // Update predator position based on velocity and time delta
            position[0] += velocity[0] * time_delta;
            position[1] += velocity[1] * time_delta;

            environment.restrict_position(position);
            environment.reflect(position, velocity);



        }
        else {
            // If the prey is close enough, "eat" it
            eat(prey_index, preys);
        }
    }
    else {
        // If there are no preys, move randomly within the environment
        // For simplicity, let's move randomly in a random direction

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, TWO_PI);
        double random_direction = dis(gen);

        // Calculate velocity components based on random direction and speed
        velocity[0] = speed_p * std::cos(random_direction);
        velocity[1] = speed_p * std::sin(random_direction);

        // Update predator position based on velocity and time delta
        position[0] += velocity[0] * time_delta;
        position[1] += velocity[1] * time_delta;
    }


    environment.restrict_position(position);
    environment.reflect(position, velocity);
}

auto Predator::log(const double time) -> void
{
    fmt::print("{:.2f},{:.2f},{:.2f}\n", time, position[0], position[1]);
}

auto Predator::eat(int prey_index, std::vector<Prey>& preyList)->void
{
    // clear an index in the preyList
    if (!preyList.empty()) {

        // Remove the selected prey
        preyList.erase(preyList.begin() + prey_index - 1);
        //increase speed by 0.25 for each prey eaten
        increase_speed();
    }
}

auto Predator::increase_speed()->void {
    speed_p += 0.25;
}

