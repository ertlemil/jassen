#pragma once
#include "Generic.h"
#include "Card.h"

class Player
{
public:
    Player(const std::span<Card> cardSpan) : cardSpan(cardSpan)
    {
        std::fill(cardsEnc.begin(), cardsEnc.end(), 0.0f);

        cards.reserve(cardSpan.size());

        setCards(cardSpan);
    };

    Card playCard(int cardIdx)
    {
        Card chosenCard = Card::arrayIdxToCard(cardIdx);

        auto it = std::find(cards.begin(), cards.end(), chosenCard);

        if (it == cards.end())
        {
            // Print what's actually in hand when crash happens
            std::cout << "Tried to play: " << chosenCard.asString() << std::endl;
            std::cout << "Hand contains: " << std::endl;
            for (auto& c : cards) std::cout << "  " << c.asString() << std::endl;
            throw std::out_of_range("Gespielte Karte nicht in der Spielerhand");
        }

        cards.erase(it);
        cardsEnc.at(chosenCard.toArrayPosition()) = 0;

        return chosenCard;
    }

    void setCards(const std::span<Card>& cardSpan)
    {
        std::copy(cardSpan.begin(), cardSpan.end(), std::back_inserter(cards));

        for (auto card : cards)
        {
            cardsEnc.at(card.toArrayPosition()) = 1.0f;
        }
    }

    Trumpf setTrumpf()
    {
        return Trumpf::Laub;
    }

    std::array<float, 36> cardsEnc;
    std::vector<Card> cards;

    std::array<float, 36> revealedCards;

private:
    std::span<Card> cardSpan;

};