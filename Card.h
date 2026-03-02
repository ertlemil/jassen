#pragma once

#include <array>
#include <random>
#include <algorithm>
#include <span>

enum class Farbe { Herz, Laub, Schell, Eichel };

enum class Wert { Sechs, Sieben, Acht, Neun, Zehn, Unter, Ober, König, Ass };

class Card
{
public:
    Farbe farbe;
    Wert wert;

    Card(Farbe farbe, Wert wert) : farbe(farbe), wert(wert) {}
};