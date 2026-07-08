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
        .def_readonly("reward", &Game::StepResult::reward, "Punkteunterschied zwischen Team 1 und 2")
        .def_readonly("done", &Game::StepResult::done);


    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def("reset", &Game::reset, "Setzt das Spiel zurück und liefert die initiale Perspektive")
        .def("step", &Game::step, py::arg("cardIdx"), "Führt einen Schritt mit der Karten-ID aus");
}