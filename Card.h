#pragma once

enum class Farbe { Herz, Laub, Schell, Eichel };

enum class Wert { Sechs, Sieben, Acht, Neun, Zehn, Unter, Ober, Koenig, Ass, Nell, Bauer };

class Card
{
public:
    Farbe farbe;
    Wert wert;

    Card(Farbe farbe, Wert wert) : farbe(farbe), wert(wert) {}

    std::string farbeToString() const
    {
        switch (farbe)
        {
            case Farbe::Herz: return "Herz";
            case Farbe::Laub: return "Laub";
            case Farbe::Schell: return "Schell";
            case Farbe::Eichel: return "Eichel";
            default: return "unknown";
        }
    }
    std::string wertToString() const
    {
        switch (wert)
        {
            case Wert::Sechs: return "Sechs";
            case Wert::Sieben: return "Sieben";
            case Wert::Acht: return "Acht";
            case Wert::Neun: return "Neun";
            case Wert::Zehn: return "Zehn";
            case Wert::Unter: return "Unter";
            case Wert::Ober: return "Ober";
            case Wert::Koenig: return "Koenig";
            case Wert::Ass: return "Ass";
            default: return "unknown";
        }
    }
    std::string asString() const
    {
        return farbeToString() + "  " + wertToString();
    }
};