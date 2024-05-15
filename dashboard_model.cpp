#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "model.h"
#include "prey.h"
#include "predator.h"

PYBIND11_MODULE(model, m) {
    py::class_<Model>(m, "Model")
        .def(py::init<const std::string&>())
        .def("update", &Model::update)
        .def_readwrite("predator", &Model::predator)
        .def_readwrite("preys", &Model::preys);

    py::class_<Predator>(m, "Predator")
        .def_readwrite("position", &Predator::position);

    py::class_<Prey>(m, "Prey")
        .def_readwrite("position", &Prey::position);
}
