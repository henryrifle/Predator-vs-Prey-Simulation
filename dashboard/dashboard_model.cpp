#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <nlohmann/json.hpp>
namespace py = pybind11;

#include "model.h"
#include "prey.h"
#include "predator.h"

// Convert nlohmann::json to Python dict
py::dict json_to_dict(const nlohmann::json& j) {
    py::dict d;
    for (auto& el : j.items()) {
        if (el.value().is_number()) {
            d[py::str(el.key())] = el.value().get<double>();
        } else if (el.value().is_string()) {
            d[py::str(el.key())] = el.value().get<std::string>();
        } else if (el.value().is_boolean()) {
            d[py::str(el.key())] = el.value().get<bool>();
        }
    }
    return d;
}

PYBIND11_MODULE(model, m) {
    py::class_<Model>(m, "Model")
        .def(py::init<const std::string&>())
        .def("update", &Model::update)
        .def_readwrite("predator", &Model::predator)
        .def_readwrite("preys", &Model::preys)
        .def_property_readonly("configuration", 
            [](const Model& model) {
                return json_to_dict(model.configuration);
            }
        );

    py::class_<Predator>(m, "Predator")
        .def(py::init<>())
        .def_readwrite("position", &Predator::position)
        .def_readwrite("velocity", &Predator::velocity)
        .def_readwrite("speed_p", &Predator::speed_p);

    py::class_<Prey>(m, "Prey")
        .def(py::init<>())
        .def_readwrite("position", &Prey::position)
        .def_readwrite("velocity", &Prey::velocity)
        .def_readwrite("speed", &Prey::speed)
        .def_readwrite("alive", &Prey::alive);
}