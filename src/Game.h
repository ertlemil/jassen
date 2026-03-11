#pragma once

#include "Deck.h"
#include "Player.h"
#include "Card.h"
#include "Generic.h"

class Game
{
public:
    struct GameState
    {
        Deck deck = {};
        int previousWinner = 0;
        int currentWinner = 0;
        int currentPlayer = 0;
        Trumpf trick = {};
        std::array<int, 4> points = {};
        std::array<int, 2> teamPoints = {};
        std::vector<Card> currentCards = {};
        std::array<Player, 4> players = {deck.getCards(0), deck.getCards(1), deck.getCards(2), deck.getCards(3),};
        std::array<std::vector<Card>, 4> revealedCards;
        std::vector<Card> playedCards;
        std::array<std::array<float, 314>, 4> playerPerspective;
        std::array<std::array<float, 4>, 4> provenEmptyCards;
        float slalomDirection = 1.0f;
    };

    struct StepResult
    {
        std::array<float, 314> nextObservation;
        float reward;
        bool done;
    };

    GameState state = {};

    std::array<float, 314> reset()
    {
        state = {};
        createPlayerPerspective();
        return state.playerPerspective.at(state.currentPlayer);
    }

    StepResult step(int cardIdx)
    {
        Card playedCard = state.players[state.currentPlayer].playCard(cardIdx);
        state.currentCards.push_back(playedCard);
        state.playedCards.push_back(playedCard);

        float reward = 0;
        bool done = false;

        if (state.currentCards.size() == 4)
        {
            state.currentWinner = (decideWinner(state.currentCards) + state.previousWinner) % NUM_PLAYERS;

            int points = calculatePoints(state.currentCards, state.playedCards.size() / 4);
            state.teamPoints[state.currentWinner % 2] += points;

            reward = static_cast<float>(points);

            if (state.playedCards.size() == 36)
                done = true;
        } else
        {
            state.currentPlayer = (state.currentPlayer + 1) % NUM_PLAYERS;
        }

        createPlayerPerspective();

        return {state.playerPerspective[state.currentPlayer], reward, done};
    }

    bool checkLegalMove(int cardIdx)
    {
        //Untertrumpfen
        if (state.currentCards[0].farbe != static_cast<Farbe>(state.trick))
        {
            Wert highestTrick = {};
            for (auto card : state.currentCards)
            {
                if (card.farbe == static_cast<Farbe>(state.trick) && card.wert > highestTrick)
                    highestTrick = card.wert;
            }

            if (state.players[state.currentPlayer].cards.at(cardIdx).farbe == static_cast<Farbe>(state.trick) && state.players[state.currentPlayer].cards.at(cardIdx).wert < highestTrick)
                return false;
        }

        //Farbzwang
        if (state.currentCards[0].farbe != state.players[state.currentPlayer].cards.at(cardIdx).farbe)
        {
            auto betterCard = std::find_if(state.players[state.currentPlayer].cards.begin(), state.players[state.currentPlayer].cards.end(), [this](Card& card)
            {
                return card.farbe == state.currentCards[0].farbe;
            });

            if (betterCard != state.players[state.currentPlayer].cards.end())
                return false;
        }

        return true;
    }

private:
    static constexpr int NUM_ROUNDS = 9;
    static constexpr int NUM_PLAYERS = 4;
    static constexpr int POINTS_LAST_TRICK = 5;

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


    void createPlayerPerspective()
    {
        int baseIndex = 0;
        std::fill(state.playerPerspective.at(state.currentPlayer).begin(), state.playerPerspective.at(state.currentPlayer).end(), 0.0f);

        //Eigene Punkte
        state.playerPerspective.at(state.currentPlayer).at(baseIndex) = state.teamPoints.at(state.currentPlayer % 2);
        baseIndex++;

        //Gegener Punkte
        state.playerPerspective.at(state.currentPlayer).at(baseIndex) = state.teamPoints.at((state.currentPlayer + 1) % 2);
        baseIndex++;

        //Spielmodus
        state.playerPerspective.at(state.currentPlayer).at(baseIndex + (static_cast<int>(state.trick) == 7 ? 6 : static_cast<int>(state.trick))) = 1.0f;
        baseIndex += 7;

        //Slalom Richtung
        if (state.trick != Trumpf::Slalom_Geiss && state.trick != Trumpf::Geiss)
            state.playerPerspective.at(state.currentPlayer).at(baseIndex) = 1.0f;
        baseIndex++;

        //Trumpf Macher
        state.playerPerspective.at(state.currentPlayer).at(baseIndex + state.previousWinner) = 1.0f;
        baseIndex += 4;

        //Meine Hand
        std::copy(state.players.at(state.currentPlayer).cardsEnc.begin(), state.players.at(state.currentPlayer).cardsEnc.end(), state.playerPerspective.at(state.currentPlayer).begin() + 14);
        baseIndex += 36;

        //Bereits gespielte Karten
        for (auto card : state.playedCards)
        {
            state.playerPerspective.at(state.currentPlayer).at(baseIndex + card.toArrayPosition()) = 1.0f;
        }
        baseIndex += 36;

        //Bekannte Hand Links
        for (auto card : state.revealedCards.at((state.currentPlayer + 1) % 4))
        {
            state.playerPerspective.at(state.currentPlayer).at(baseIndex + card.toArrayPosition()) = 1.0f;
        }
        baseIndex += 36;

        //Bekannte Hand Gegenüber
        for (auto card : state.revealedCards.at((state.currentPlayer + 2) % 4))
        {
            state.playerPerspective.at(state.currentPlayer).at(baseIndex + card.toArrayPosition()) = 1.0f;
        }
        baseIndex += 36;

        //bekannte Hand Rechts
        for (auto card : state.revealedCards.at((state.currentPlayer + 3) % 4))
        {
            state.playerPerspective.at(state.currentPlayer).at(baseIndex + card.toArrayPosition()) = 1.0f;
        }
        baseIndex += 36;

        //Void Farben Links
        std::copy(state.provenEmptyCards.at((state.currentPlayer + 1) % 4).begin(), state.provenEmptyCards.at((state.currentPlayer + 1) % 4).end(), state.playerPerspective.at(state.currentPlayer).begin() + baseIndex);
        baseIndex += 4;

        //Void Farben Gegenüber
        std::copy(state.provenEmptyCards.at((state.currentPlayer + 2) % 4).begin(), state.provenEmptyCards.at((state.currentPlayer + 2) % 4).end(), state.playerPerspective.at(state.currentPlayer).begin() + baseIndex);
        baseIndex += 4;

        //Void Farben Rechts
        std::copy(state.provenEmptyCards.at((state.currentPlayer + 3) % 4).begin(), state.provenEmptyCards.at((state.currentPlayer + 3) % 4).end(), state.playerPerspective.at(state.currentPlayer).begin() + baseIndex);
        baseIndex += 4;

        //1. Karte im Stich
        if (state.currentCards.size() > 0)
        {
            state.playerPerspective.at(state.currentPlayer).at(baseIndex + state.currentCards.at(0).toArrayPosition()) = 1.0f;
        }
        baseIndex += 36;

        //2. Karte im Stich
        if (state.currentCards.size() > 1)
        {
            state.playerPerspective.at(state.currentPlayer).at(baseIndex + state.currentCards.at(1).toArrayPosition()) = 1.0f;
        }
        baseIndex += 36;

        //3. Karte im Stich
        if (state.currentCards.size() > 2)
        {
            state.playerPerspective.at(state.currentPlayer).at(baseIndex + state.currentCards.at(0).toArrayPosition()) = 1.0f;
        }
        baseIndex += 36;
    }

    int decideWinner(std::vector<Card>& cards)
    {
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

                if (state.trick == static_cast<Trumpf>(card.farbe))
                {
                    if (card.wert == Wert::Neun)
                        value = 14;
                    if (card.wert == Wert::Unter)
                        value = 20;
                } else if (
                    state.trick == Trumpf::Bock ||
                    state.trick == Trumpf::Geiss ||
                    state.trick == Trumpf::Slalom_Bock ||
                    state.trick == Trumpf::Slalom_Geiss)
                {
                    if (card.wert == Wert::Acht)
                        value = 8;
                }

                return sum + value;
            }
        );
    }

    bool isCardHigher(const Card& currentHighest, const Card& newCard) const
    {
        if (state.trick == Trumpf::Geiss || state.trick == Trumpf::Slalom_Geiss)
            return newCard.wert < currentHighest.wert;

        if (newCard.farbe == static_cast<Farbe>(state.trick) && currentHighest.farbe != newCard.farbe)
            return true;

        if (newCard.farbe == static_cast<Farbe>(state.trick))
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
