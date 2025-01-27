#pragma once

#include <filesystem>
#include <random>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <cmath>
#include "prey.h"  // Include the full Prey definition
#include "environment.h"  // Fixed spelling

// Forward declare Prey as a struct to match its definition
struct Prey;

struct Predator {
public:
    std::array<double, 2> position;
    std::array<double, 2> velocity;
    double speed_p;

    Predator() : position({0.0, 0.0}), velocity({0.0, 0.0}), speed_p(1.0) {}

    void update(double dt, std::vector<Prey>& preys, double bounds, const Environment& env);

    auto log(const double time) const -> void;
    auto eat(int prey_index, std::vector<Prey>& preyList) -> void;
    auto increase_speed() -> void;
};