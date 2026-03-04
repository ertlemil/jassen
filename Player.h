#pragma once
#include "Generic.h"
#include "Card.h"


enum class Trumpf { Herz, Laub, Schell, Eichel, Bock, Geiss, Slalom_Bock, Slalom_Geiss };

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
        trumpf = Trumpf::Eichel;
    }

    inline static Trumpf trumpf;
    int points;

private:
    std::span<Card> cards;
};