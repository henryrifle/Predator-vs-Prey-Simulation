#pragma once

#include <filesystem>
#include <random>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "predator.h"
#include "prey.h"
#include "enviroment.h"
#include "model.h"

using json = nlohmann::json;

struct Model
{
    Model(const std::string&);

    void update(const double time_delta);

    const Predator& getPredator() const;

    double time{};

    json configuration{};
    std::vector<Prey> preys{};
    Predator predator{};
    Environment environment{};
};