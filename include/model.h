#pragma once

#include <filesystem>
#include <random>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "predator.h"
#include "prey.h"
#include "environment.h"
#include "model.h"

using json = nlohmann::json;

struct Model
{
    Model(const std::string&);

    void update(double dt);

    const Predator& getPredator() const;

    double time{};

    json configuration{};
    std::vector<Prey> preys{};
    Predator predator{};
    Environment environment{};
};