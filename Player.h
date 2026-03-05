#pragma once
#include "Generic.h"
#include "Card.h"

class Player
{
public:
    Player(const std::span<Card> cardSpan) : cardSpan(cardSpan), points(0)
    {
        std::fill(cards.begin(), cards.end(), 0.0f);
    };

    int getPoints() const
    {
        return points;
    }

    Card playCard(const int i) const
    {
        return cardSpan[i];
    }

    Trumpf setTrumpf()
    {
        return Trumpf::Laub;
    }

    int points;

private:
    std::span<Card> cardSpan;
    std::array<float, 36> cards;
};