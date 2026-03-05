#pragma once

#include "Deck.h"
#include "Player.h"
#include "Card.h"
#include "Generic.h"

class Game
{
public:
    Trumpf trumpf;

    GameState currentGameState;

    void play()
    {
        trumpf = players[0].setTrumpf();
        std::fill(currentGameState.trick.begin(), currentGameState.trick.end(), 0.0f);
        currentGameState.trick.at(static_cast<int>(trumpf) == 7 ? 6 : static_cast<int>(trumpf)) = 1.0f;

        /////////////Temporär
        currentGameState.slalomDirection = 1.0f;


        for (int round = 0; round < NUM_ROUNDS; round++)
        {
            for (int i = 0; i < NUM_PLAYERS; i++)
            {
                currentCards.at(i) = players.at(i).playCard(currentGameState);
            }

            currentWinner = decideWinner(currentCards);
            points[(lastWinner + currentWinner) % NUM_PLAYERS] += calculatePoints(currentCards, round);
            lastWinner = currentWinner;
            currentCards.clear();
        }


        //std::cout << collectivePoints << std::endl;
        //std::cout << lastWinner << std::endl;
    }














private:
    static constexpr int NUM_ROUNDS = 9;
    static constexpr int NUM_PLAYERS = 4;
    static constexpr int POINTS_LAST_TRICK = 5;

    std::array<int, 4> points;
    std::array<float, 2> scaledPoints;
    std::array<float, 4> trickDecider = {1.0f, 0.0f, 0.0f, 0.0f};

    float slalomDirection;

    Deck deck;

    std::vector<Card> cardsPlayed;
    std::array<Player, 4> players = {Player(deck.getCards(0)), Player(deck.getCards(1)), Player(deck.getCards(2)), Player(deck.getCards(3))};
    std::vector<Card> currentCards;

    const std::map<Wert, int> cardValues = {
        {Wert::Sechs, 0},
        {Wert::Sieben, 0},
        {Wert::Acht, 0},
        {Wert::Neun, 0},
        {Wert::Zehn, 10},
        {Wert::Unter, 2},
        {Wert::Ober, 3},
        {Wert::Koenig, 4},
        {Wert::Ass, 11}
    };

    int lastWinner = 0;
    int currentWinner;
    int currentPlayer = 0;
    int collectivePoints = 0;

    std::array<std::array<float, 36>, 4> availableCards = {};

    int decideWinner(std::vector<Card>& cards)
    {
        if (cards.size() != 4)
            throw std::length_error("Not the right amount of cards for winner");

        int result;

        for (auto card : cards)
        {
            std::cout << card.asString() << std::endl;
        }

        result = static_cast<int>(std::distance(cards.begin(), std::max_element(cards.begin(),cards.end(),
            [this](const Card& a, const Card& b)
            {
                return isCardHigher(a, b);
            }
        )));
        std::cout << result << std::endl;
        return result;


    }

    int calculatePoints(std::vector<Card>& cards, int round)
    {
        if (cards.size() != NUM_PLAYERS)
            throw std::length_error("Fehlerhafte Anzahl Karten bei Punktebewertung");

        return (round == NUM_ROUNDS - 1 ? POINTS_LAST_TRICK : 0) + std::accumulate(
            cards.begin(),
            cards.end(), 0,
            [this](int sum, const Card& card)
            {
                int value = cardValues.at(card.wert);

                if (trumpf == static_cast<Trumpf>(card.farbe))
                {
                    if (card.wert == Wert::Neun)
                        value = 14;
                    if (card.wert == Wert::Unter)
                        value = 20;
                } else if (
                    trumpf == Trumpf::Bock ||
                    trumpf == Trumpf::Geiss ||
                    trumpf == Trumpf::Slalom_Bock ||
                    trumpf == Trumpf::Slalom_Geiss)
                {
                    if (card.wert == Wert::Acht)
                        value = 8;
                }

                return sum + value;
            }
        );
    }

    bool isCardHigher(const Card& currentHighest, const Card& newCard)
    {
        if (trumpf == Trumpf::Geiss || trumpf == Trumpf::Slalom_Geiss)
            return newCard.wert < currentHighest.wert;

        if (newCard.farbe == static_cast<Farbe>(trumpf) && currentHighest.farbe != newCard.farbe)
            return true;

        if (newCard.farbe == static_cast<Farbe>(trumpf))
        {
            if (newCard.wert == Wert::Unter)
                return true;

            if (newCard.wert == Wert::Neun && currentHighest.wert != Wert::Unter)
                return true;

            if (currentHighest.wert != Wert::Neun && currentHighest.wert != Wert::Unter)
                return newCard.wert > currentHighest.wert;

            return false;
        }

        if (newCard.farbe == currentHighest.farbe)
            return newCard.wert > currentHighest.wert;

        return false;
    }
};
