
#include "predator.h"
#include "prey.h"
#include "enviroment.h"
#include "model.h"

#include <fstream>

#include <nlohmann/json.hpp>

Model::Model(const std::string& configuration_file)
    :time(0.0),
    preys{},
    predator{},
    environment{} {


    auto cfs = std::ifstream{ configuration_file };
    configuration = json::parse(cfs);

    auto bounds = configuration["region_limit"].get<std::double_t>();

    environment.x_limit = std::array<double, 2>{ -bounds, bounds };
    environment.y_limit = std::array<double, 2>{ -bounds, bounds };

    auto number_of_prey = configuration["number_of_preys"].get<std::double_t>();
    preys.resize(number_of_prey);


    auto preys_speed = configuration["prey_speed"].get<std::double_t>();

    for (auto& prey : preys) {
        prey.speed = preys_speed;
    }


    auto pred_speed = configuration["predator_speed"].get<std::double_t>();
    predator.speed_p = pred_speed;



    auto eng = std::mt19937{ configuration["seed"] };
    auto dis = std::uniform_real_distribution<>{ -10, 10 };


    std::ranges::generate(preys, [&]() -> Prey {return Prey{ {dis(eng), dis(eng)}, {dis(eng), dis(eng)}, preys_speed };});



}
void Model::update(const double time_delta)
{
    time += time_delta;

    for (auto& prey : preys)
    {
        prey.update(time_delta, environment, predator);
    }


    predator.update(time_delta, environment, preys);

}




