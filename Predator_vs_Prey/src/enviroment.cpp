#include <algorithm>
#include <fmt/format.h>
#include "prey.h"
#include "enviroment.h"
#include "model.h"
#include <cmath>


auto Environment::restrict_position(std::array<double, 2>& position) const->void
{

    position[0] = std::clamp(position[0], x_limit[0], x_limit[1]);
    position[1] = std::clamp(position[1], y_limit[0], y_limit[1]);
}

auto Environment::reflect(const std::array<double, 2>& position, std::array<double, 2>& velocity) const->void
{
    if (position[0] == x_limit[0] || position[0] == x_limit[1])
    {
        velocity[0] *= -1.0;
    }
    if (position[1] == y_limit[0] || position[1] == y_limit[1])
    {
        velocity[1] *= -1.0;
    }
}

auto Environment::get_multiplier(const std::array<double, 2>& position) const->std::array<double, 2>
{

    if (position[0] < x_limit[1] && position[1] > (x_limit[1] / 2)) {
        return{ 5.0,5.0 };
    }
    if (position[0] < x_limit[1] && position[1] < -(x_limit[1] / 2)) {
        return{ 5.0,5.0 };
    }
    if (position[0] > (x_limit[1] / 2)) {
        return{ 5.0,5.0 };
    }
    if (position[0] < -(x_limit[1] / 2)) {
        return{ 5.0,5.0 };
    }
    else
    {
        return{ 1.0,1.0 };
    }



}

// An attempt to create a breeding system, couldnt figure out how to add into model
/*
void Environment::breed_zone(const std::array<double, 2>& position) {
    // Define the coordinates of the breeding zone
    constexpr double breed_zone_x_s = -50.0;//-50.0
    constexpr double breed_zone_x_f = 50.0;//50.0
    constexpr double breed_zone_y_s = -50.0;//-50.0
    constexpr double breed_zone_y_f = 50.0;//50.0
    // If the prey is within the breeding zone, call the breed function
    if (position[0] >= breed_zone_x_s && position[0] <= breed_zone_x_f) {
        if (position[1] >= breed_zone_y_s && position[1] <= breed_zone_y_f) {
            preys[0].breed(preys);


        }
    }


}*/