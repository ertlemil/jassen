#pragma once
#include "Generic.h"
#include "Card.h"

class Player
{
public:
    Player(const std::span<Card> cardSpan) : cardSpan(cardSpan)
    {
        std::fill(cardsEnc.begin(), cardsEnc.end(), 0.0f);

        cards.reserve(9);

        std::copy(cardSpan.begin(), cardSpan.end(), cards.begin());

        for (auto card : cards)
        {
            cardsEnc.at(card.toArrayPosition()) = 1.0f;
        }
    };

    Card playCard(int cardIdx)
    {
        Card chosenCard = cards.at(cardIdx);

        cards.erase(cards.begin() + cardIdx);
        cardsEnc.at(chosenCard.toArrayPosition()) = 0;
        return cardSpan[0];
    }

    Trumpf setTrumpf()
    {
        return Trumpf::Laub;
    }

    std::array<float, 36> cardsEnc;
    std::vector<Card> cards;

private:


    std::span<Card> cardSpan;

};