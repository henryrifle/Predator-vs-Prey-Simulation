#pragma once

#include <filesystem>
#include <random>
#include <string>
#include <vector>


#include "predator.h"
#include "enviroment.h"


class Predator;
class Environment;


struct Prey {
    auto update(const double time_delta, Environment& environment, Predator& predator) -> void;

    auto log(const double time) -> void;

    std::array<double, 2> position{};
    std::array<double, 2> velocity{};
    double speed;

    Prey() : speed(0.0) {};


    Prey(const std::array<double, 2>& pos, const std::array<double, 2>& vel, double spd)
        : position(pos), velocity(vel), speed(spd) {}

    //auto breed(std::vector<Prey>& preys) -> void;

};