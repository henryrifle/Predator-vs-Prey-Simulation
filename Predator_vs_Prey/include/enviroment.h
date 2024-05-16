#pragma once

#include <array>
#include <vector>
#include "prey.h"

class Prey;


struct Environment
{
    auto restrict_position(std::array<double, 2>& position) const->void;

    auto reflect(const std::array<double, 2>& position, std::array<double, 2>& velocity) const->void;

    auto get_multiplier(const std::array<double, 2>& position)const->std::array<double, 2>;

    //void breed_zone(const std::array<double, 2>& position);

    std::array<double, 2> x_limit{};
    std::array<double, 2> y_limit{};
    std::vector<Prey> preys;

};