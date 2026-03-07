#pragma once
#include "Generic.h"
#include "Card.h"

class Player
{
public:
    Player(const std::span<Card> cardSpan) : cardSpan(cardSpan)
    {
        std::fill(cards.begin(), cards.end(), 0.0f);

        for (auto card : cardSpan)
        {
            cards.at(card.toArrayPosition()) = 1.0f;
        }
    };

    Card playCard(std::array<float, 314> data) const
    {






        return cardSpan[0];
    }

    Trumpf setTrumpf()
    {
        return Trumpf::Laub;
    }

    std::array<float, 36> cards;

private:


    std::span<Card> cardSpan;

};