#pragma once



class Card
{
public:
    Farbe farbe;
    Wert wert;

    Card(Farbe farbe, Wert wert) : farbe(farbe), wert(wert) {}


    std::string asString() const
    {
        return farbeToString() + "  " + wertToString();
    }

    int toArrayPosition()
    {
        return static_cast<int>(farbe) * NUM_ROUNDS + static_cast<int>(wert);
    }

    static Card arrayIdxToCard(int idx)
    {
        return Card(static_cast<Farbe>(idx / 9), static_cast<Wert>(idx % 9));
    }

    bool operator==(const Card& other) const
    {
        return wert == other.wert && farbe == other.farbe;
    }
private:
    static constexpr int NUM_ROUNDS = 9;

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
};