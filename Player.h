#pragma once
#include "Generic.h"
#include "Card.h"


enum class Trumpf { Herz, Laub, Schell, Eichel, Bock, Geiß, Slalom_Bock, Slalom_Geiß };

class Player
{
public:
    Player(const std::span<Card> cards) : cards(cards), points(0) {};

    int getPoints() const
    {
        return points;
    }

    Card playCard(const int i) const
    {
        return cards[i];
    }

    void setTrumpf()
    {
        trumpf = static_cast<int>(Trumpf::Bock);
    }

    inline static int trumpf;

private:
    std::span<Card> cards;
    int points;
};