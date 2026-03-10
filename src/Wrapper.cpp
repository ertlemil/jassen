#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Zwingend notwendig für std::array und std::vector
#include "Game.h"

namespace py = pybind11;

PYBIND11_MODULE(jass_engine, m) {
    m.doc() = "C++ Jass Engine Environment für Reinforcement Learning";

    // 1. Wir müssen StepResult binden, da step() dies zurückgibt
    // Wir machen die Member readonly, da Python die Werte nur lesen, nicht überschreiben soll.
    py::class_<Game::StepResult>(m, "StepResult")
        .def_readonly("nextObservation", &Game::StepResult::nextObservation)
        .def_readonly("reward", &Game::StepResult::reward)
        .def_readonly("done", &Game::StepResult::done);

    // 2. Wir binden die Game Klasse
    py::class_<Game>(m, "Game")
        .def(py::init<>()) // Standardkonstruktor freigeben (Game game = Game())
        .def("reset", &Game::reset, "Setzt das Spiel zurück und liefert die initiale Perspektive")
        .def("step", &Game::step, py::arg("cardIdx"), "Führt einen Schritt mit der Karten-ID aus");
}