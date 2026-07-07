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
        int trumpfDecider = 0;
        Trumpf trumpf = {};
        std::array<int, 4> points = {};
        std::array<int, 2> teamPoints = {};
        std::vector<Card> currentCards = {};
        std::array<Player, 4> players = {deck.getCards(0), deck.getCards(1), deck.getCards(2), deck.getCards(3)};
        std::array<std::vector<Card>, 4> revealedCards;
        std::vector<Card> playedCards;
        std::array<float, 314> playerPerspective;
        std::array<std::array<float, 4>, 4> provenEmptyCards;
        float slalomDirection = 1.0f;
    };

    struct StepResult
    {
        std::array<float, 314> nextActorObservation;
        std::array<float, 374> nextCriticObservation;
        std::array<bool, 36> mask;
        int pointDiff;
        bool done;
    };


    GameState state = {};



    std::array<float, 314> reset()
    {
        state = {};
        createPlayerPerspective();
        return state.playerPerspective;
    }

    StepResult step(int cardIdx)
    {
        Card playedCard = state.players[state.currentPlayer].playCard(cardIdx);
        state.currentCards.push_back(playedCard);
        state.playedCards.push_back(playedCard);

        int pointDiff = 0;
        bool done = false;

        if (state.currentCards.size() == 4)
        {
            state.currentWinner = decideWinner(state.currentCards);
            state.previousWinner = state.currentWinner;
            state.currentPlayer = state.currentWinner;

            int points = calculatePoints(state.currentCards, state.playedCards.size() / 4);
            state.teamPoints[state.currentWinner % 2] += points;

            state.currentCards.clear();

            //Game finished
            if (state.playedCards.size() == 36)
            {
                done = true;
                pointDiff = state.teamPoints.at(0) - state.teamPoints.at(1);

                state.slalomDirection = 1.0f;
            }

            //Switching Slalom Direction
            if (state.trumpf == Trumpf::Slalom_Bock || state.trumpf == Trumpf::Slalom_Geiss)
            {
                state.slalomDirection = state.slalomDirection - 1.0f;
                state.slalomDirection *= -1;
            }

        } else
        {
            state.currentPlayer = (state.currentPlayer + 1) % NUM_PLAYERS;
        }

        createPlayerPerspective();

        return {state.playerPerspective, createCriticInformation(), createMask() ,pointDiff, done};
    }

    std::array<bool, 36> createMask()
    {
        bool playableCardfound;
        std::array<bool, 36> mask;
        for (int i = 0; i < 36; i++)
        {
            bool result = checkLegalCard(Card::arrayIdxToCard(i));
            mask.at(i) = result;

            if (result)
                playableCardfound = true;
        }

        if (!playableCardfound)
        {
            for (auto card : state.players.at(state.currentPlayer).cards)
            {
                mask.at(card.toArrayPosition()) = true;
            }
        }

        return mask;
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

    bool checkLegalCard(Card card)
    {
        //Besitzt Karte
        if (std::find(state.players[state.currentPlayer].cards.begin(),
            state.players[state.currentPlayer].cards.end(),
            card) == state.players[state.currentPlayer].cards.end())
            return false;

        if (state.currentCards.empty())
            return true;

        //Untertrumpfen
        if (state.currentCards[0].farbe != static_cast<Farbe>(state.trumpf) &&
            std::none_of(state.players[state.currentPlayer].cards.begin(),
                state.players[state.currentPlayer].cards.end(),
                [this](Card& c)
                {
                    return c.farbe != static_cast<Farbe>(state.trumpf);
                }
            ))
        {
            Card highestCard = card;
            for (auto card : state.currentCards)
            {
                if (card.farbe == static_cast<Farbe>(state.trumpf) && isCardHigherOrEqual(highestCard, card))
                    highestCard = card;
            }

            if (card.farbe == static_cast<Farbe>(state.trumpf) && isCardHigherOrEqual(card, highestCard))
                return false;
        }

        //Farbzwang
        if (card.farbe != state.currentCards[0].farbe &&
        std::any_of(state.players[state.currentPlayer].cards.begin(),
                    state.players[state.currentPlayer].cards.end(),
                    [this](const Card& c) { return c.farbe == state.currentCards[0].farbe; }))
            return false;

        return true;
    }

    void createPlayerPerspective()
    {
        int baseIndex = 0;
        std::fill(state.playerPerspective.begin(), state.playerPerspective.end(), 0.0f);

        //Eigene Punkte
        state.playerPerspective.at(baseIndex) = state.teamPoints.at(state.currentPlayer % 2);
        baseIndex++;

        //Gegener Punkte
        state.playerPerspective.at(baseIndex) = state.teamPoints.at((state.currentPlayer + 1) % 2);
        baseIndex++;

        //Spielmodus
        state.playerPerspective.at(baseIndex + (static_cast<int>(state.trumpf) == 7 ? 6 : static_cast<int>(state.trumpf))) = 1.0f;
        baseIndex += 7;

        //Slalom Richtung
        state.playerPerspective.at(baseIndex) = state.slalomDirection;
        baseIndex++;

        //Trumpf Macher
        state.playerPerspective.at(baseIndex + state.trumpfDecider) = 1.0f;
        baseIndex += 4;

        //Meine Hand
        std::copy(state.players.at(state.currentPlayer).cardsEnc.begin(), state.players.at(state.currentPlayer).cardsEnc.end(), state.playerPerspective.begin() + baseIndex);
        baseIndex += 36;

        //Bereits gespielte Karten
        for (auto card : state.playedCards)
        {
            state.playerPerspective.at(baseIndex + card.toArrayPosition()) = 1.0f;
        }
        baseIndex += 36;

        //Bekannte Karten
        for (int player = 1; player < NUM_PLAYERS; player++)
        {
            for (auto card : state.revealedCards.at((state.currentPlayer + player) % 4))
            {
                state.playerPerspective.at(baseIndex + card.toArrayPosition()) = 1.0f;
            }
            baseIndex += 36;
        }

        //void Farben
        for (int player = 1; player < NUM_PLAYERS; player++)
        {
            std::copy(state.provenEmptyCards.at((state.currentPlayer + player) % 4).begin(), state.provenEmptyCards.at((state.currentPlayer + player) % 4).end(), state.playerPerspective.begin() + baseIndex);
            baseIndex += 4;
        }

        //Karten im Stich
        //Muss an letzter Stelle durgchgegeben werden (foreach)
        for (auto card : state.playedCards)
        {
            state.playerPerspective.at(baseIndex + card.toArrayPosition()) = 1.0f;
            baseIndex += 36;
        }
    }

    std::array<float, 374> createCriticInformation()
    {
        std::array<float,374> fullInformation = {};
        int baseIndex = 0;

        //Spielmodus
        fullInformation.at(baseIndex + (static_cast<int>(state.trumpf) == 7 ? 6 : static_cast<int>(state.trumpf))) = 1.0f;
        baseIndex += 7;

        //Spieler Karten
        for (int player = 0; player < NUM_PLAYERS; player++)
        {
            std::copy(state.players.at((player + state.currentPlayer) % 4).cardsEnc.begin(), state.players.at((player + state.currentPlayer) % 4).cardsEnc.end(), fullInformation.begin() + baseIndex);
            baseIndex += 36;
        }

        //Trumpf Ansager
        fullInformation.at(baseIndex + (4 - state.currentPlayer) % 4) = 1.0f;
        baseIndex += 4;

        //Slalom Richtung
        fullInformation.at(baseIndex) = state.slalomDirection;
        baseIndex++;

        //Gezeigte Karten
        for (int player = 0; player < NUM_PLAYERS; player++)
        {
            for (auto card : state.revealedCards.at(player))
            {
                fullInformation.at(baseIndex + ((player + state.currentPlayer) % 4) * 36 + card.toArrayPosition()) = 1.0f;
            }
            baseIndex += 36;
        }

        fullInformation.at(baseIndex) = state.teamPoints[state.currentPlayer % 2] / 157.0f;
        baseIndex++;

        fullInformation.at(baseIndex) = state.teamPoints[(state.currentPlayer + 1) % 2] / 157.0f;
        baseIndex++;

        //Karten im Stich
        //Muss an letzter Stelle durgchgegeben werden (foreach)
        for (auto card : state.currentCards)
        {
            fullInformation.at(baseIndex + card.toArrayPosition()) = 1.0f;
            baseIndex += 36;
        }

        return fullInformation;
    }

    int decideWinner(std::vector<Card>& cards)
    {
        int result;

        result = static_cast<int>(std::distance(cards.begin(), std::max_element(cards.begin(),cards.end(),
            [this](const Card& a, const Card& b)
            {
                return isCardHigherOrEqual(a, b);
            }
        )));
        return result;
    }

    int calculatePoints(std::vector<Card>& cards, int round)
    {
        if (cards.size() != NUM_PLAYERS)
            throw std::length_error("Fehlerhafte Anzahl Karten bei Punktebewertung");

        return (round == NUM_ROUNDS ? POINTS_LAST_TRICK : 0) + std::accumulate(
            cards.begin(),
            cards.end(), 0,
            [this](int sum, const Card& card)
            {
                int value = cardValues.at(card.wert);

                if (state.trumpf == static_cast<Trumpf>(card.farbe))
                {
                    if (card.wert == Wert::Neun)
                        value = 14;
                    if (card.wert == Wert::Unter)
                        value = 20;
                } else if (
                    state.trumpf == Trumpf::Bock ||
                    state.trumpf == Trumpf::Geiss ||
                    state.trumpf == Trumpf::Slalom_Bock ||
                    state.trumpf == Trumpf::Slalom_Geiss)
                {
                    if (card.wert == Wert::Acht)
                        value = 8;
                }

                return sum + value;
            }
        );
    }

    bool isCardHigherOrEqual(const Card& currentHighest, const Card& newCard) const
    {
        if (currentHighest == newCard)
            return true;

        if (state.trumpf == Trumpf::Geiss || state.slalomDirection == 0.0f)
            return newCard.wert < currentHighest.wert;

        if (newCard.farbe == static_cast<Farbe>(state.trumpf) && currentHighest.farbe != newCard.farbe)
            return true;

        if (newCard.farbe == static_cast<Farbe>(state.trumpf))
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
