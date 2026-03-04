#pragma once
#include "Generic.h"
#include "Card.h"

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

    Trumpf setTrumpf()
    {
        return Trumpf::Eichel;
    }

    int points;

private:
    std::span<Card> cards;
};