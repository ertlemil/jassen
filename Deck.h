#pragma once

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

    std::span<Card> getCards(int playerNumber)
    {
        if (playerNumber < 1 || playerNumber > 4)
        {
            throw std::out_of_range("Spielernummber außerhalb des erlaubten Bereichs");
        }

        return {cards.begin() + (playerNumber - 1) * 9, 9};
    }

private:
    std::array<Card, 36> cards = {
        Card(Farbe::Herz, Wert::Sechs),
        Card(Farbe::Laub, Wert::Sechs),
        Card(Farbe::Schell, Wert::Sechs),
        Card(Farbe::Eichel, Wert::Sechs),

        Card(Farbe::Herz, Wert::Sieben),
        Card(Farbe::Laub, Wert::Sieben),
        Card(Farbe::Schell, Wert::Sieben),
        Card(Farbe::Eichel, Wert::Sieben),

        Card(Farbe::Herz, Wert::Acht),
        Card(Farbe::Laub, Wert::Acht),
        Card(Farbe::Schell, Wert::Acht),
        Card(Farbe::Eichel, Wert::Acht),

        Card(Farbe::Herz, Wert::Neun),
        Card(Farbe::Laub, Wert::Neun),
        Card(Farbe::Schell, Wert::Neun),
        Card(Farbe::Eichel, Wert::Neun),

        Card(Farbe::Herz, Wert::Zehn),
        Card(Farbe::Laub, Wert::Zehn),
        Card(Farbe::Schell, Wert::Zehn),
        Card(Farbe::Eichel, Wert::Zehn),

        Card(Farbe::Herz, Wert::Unter),
        Card(Farbe::Laub, Wert::Unter),
        Card(Farbe::Schell, Wert::Unter),
        Card(Farbe::Eichel, Wert::Unter),

        Card(Farbe::Herz, Wert::Ober),
        Card(Farbe::Laub, Wert::Ober),
        Card(Farbe::Schell, Wert::Ober),
        Card(Farbe::Eichel, Wert::Ober),

        Card(Farbe::Herz, Wert::König),
        Card(Farbe::Laub, Wert::König),
        Card(Farbe::Schell, Wert::König),
        Card(Farbe::Eichel, Wert::König),

        Card(Farbe::Herz, Wert::Ass),
        Card(Farbe::Laub, Wert::Ass),
        Card(Farbe::Schell, Wert::Ass),
        Card(Farbe::Eichel, Wert::Ass)
    };
};