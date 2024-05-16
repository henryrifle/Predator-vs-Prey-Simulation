#pragma once

#include <filesystem>
#include <random>
#include <string>
#include <vector>
#include <array>

#include "enviroment.h"



struct Environment;
struct Prey;


struct Predator {
    auto update(const double time_delta, const Environment& environment,std::vector<Prey>& preys) -> void;

    auto log(const double time) -> void;

    
    auto eat(int prey_index,std::vector<Prey>& preyList) -> void; 

    auto increase_speed()->void;
    
    
    std::array<double, 2> position{};
    std::array<double, 2> velocity{};
    double speed_p;
    
    


};