#pragma once

#include <array>
#include <random>
#include <algorithm>

enum class Farbe { Herz, Laub, Schell, Eichel };

enum class Wert { Sechs, Sieben, Acht, Neun, Zehn, Unter, Ober, König, Ass };

class Card
{
public:
    Farbe farbe;
    Wert wert;

    Card(Farbe farbe, Wert wert);
};

class Deck
{
public:
    void shuffle()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::shuffle(cards.begin(), cards.end(), gen);
    }

    Deck() { shuffle(); }

private:
    std::array<Card, 36> cards = {
        Card(Farbe(Farbe::Herz), Wert(Wert::Sechs)),
        Card(Farbe(Farbe::Laub), Wert(Wert::Sechs)),
        Card(Farbe(Farbe::Schell), Wert(Wert::Sechs)),
        Card(Farbe(Farbe::Eichel), Wert(Wert::Sechs)),

        Card(Farbe(Farbe::Herz), Wert(Wert::Sieben)),
        Card(Farbe(Farbe::Laub), Wert(Wert::Sieben)),
        Card(Farbe(Farbe::Schell), Wert(Wert::Sieben)),
        Card(Farbe(Farbe::Eichel), Wert(Wert::Sieben)),

        Card(Farbe(Farbe::Herz), Wert(Wert::Acht)),
        Card(Farbe(Farbe::Laub), Wert(Wert::Acht)),
        Card(Farbe(Farbe::Schell), Wert(Wert::Acht)),
        Card(Farbe(Farbe::Eichel), Wert(Wert::Acht)),

        Card(Farbe(Farbe::Herz), Wert(Wert::Neun)),
        Card(Farbe(Farbe::Laub), Wert(Wert::Neun)),
        Card(Farbe(Farbe::Schell), Wert(Wert::Neun)),
        Card(Farbe(Farbe::Eichel), Wert(Wert::Neun)),

        Card(Farbe(Farbe::Herz), Wert(Wert::Zehn)),
        Card(Farbe(Farbe::Laub), Wert(Wert::Zehn)),
        Card(Farbe(Farbe::Schell), Wert(Wert::Zehn)),
        Card(Farbe(Farbe::Eichel), Wert(Wert::Zehn)),

        Card(Farbe(Farbe::Herz), Wert(Wert::Unter)),
        Card(Farbe(Farbe::Laub), Wert(Wert::Unter)),
        Card(Farbe(Farbe::Schell), Wert(Wert::Unter)),
        Card(Farbe(Farbe::Eichel), Wert(Wert::Unter)),

        Card(Farbe(Farbe::Herz), Wert(Wert::Ober)),
        Card(Farbe(Farbe::Laub), Wert(Wert::Ober)),
        Card(Farbe(Farbe::Schell), Wert(Wert::Ober)),
        Card(Farbe(Farbe::Eichel), Wert(Wert::Ober)),

        Card(Farbe(Farbe::Herz), Wert(Wert::König)),
        Card(Farbe(Farbe::Laub), Wert(Wert::König)),
        Card(Farbe(Farbe::Schell), Wert(Wert::König)),
        Card(Farbe(Farbe::Eichel), Wert(Wert::König)),

        Card(Farbe(Farbe::Herz), Wert(Wert::Ass)),
        Card(Farbe(Farbe::Laub), Wert(Wert::Ass)),
        Card(Farbe(Farbe::Schell), Wert(Wert::Ass)),
        Card(Farbe(Farbe::Eichel), Wert(Wert::Ass))
    };
};