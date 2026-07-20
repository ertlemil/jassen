#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Game.h"

namespace py = pybind11;

PYBIND11_MODULE(jass_engine, m) {
    m.doc() = "C++ Jass Engine Environment für Reinforcement Learning";

    py::class_<Game::StepResult>(m, "StepResult")
        .def_readonly("nextObservation", &Game::StepResult::nextActorObservation)
        .def_readonly("nextCriticObservation", &Game::StepResult::nextCriticObservation)
        .def_readonly("mask", &Game::StepResult::mask)
        .def_readonly("reward", &Game::StepResult::reward)
        .def_readonly("done", &Game::StepResult::done);


    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def("reset", &Game::reset, py::call_guard<py::gil_scoped_release>())
        .def("step", &Game::step, py::arg("cardIdx"), py::call_guard<py::gil_scoped_release>());

    m.def("noop", []() {}, py::call_guard<py::gil_scoped_release>());
}