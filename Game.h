#pragma once

#include "Deck.h"
#include "Player.h"
#include "Card.h"
#include "Generic.h"

class Game
{
public:
    Trumpf trumpf;


    void play()
    {
        trumpf = players[0].setTrumpf();

        for (int round = 0; round < NUM_ROUNDS; round++)
        {
            for (int i = 0; i < NUM_PLAYERS; i++)
            {
                currentCards.push_back(players.at((currentPlayer + i) % NUM_PLAYERS).playCard(round));
            }

            currentWinner = decideWinner(currentCards);
            players[(lastWinner + currentWinner) % NUM_PLAYERS].points += calculatePoints(currentCards, round);
            lastWinner = currentWinner;
            currentCards.clear();
            currentCards.shrink_to_fit();
        }

        for (auto player : players)
        {
            std::cout << player.points << std::endl;
        }

        //std::cout << collectivePoints << std::endl;
        //std::cout << lastWinner << std::endl;
    }

private:
    static constexpr int NUM_ROUNDS = 9;
    static constexpr int NUM_PLAYERS = 4;
    static constexpr int POINTS_LAST_TRICK = 5;

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


    int decideWinner(std::vector<Card>& cards)
    {
        if (cards.size() != 4)
            throw std::length_error("Not the right amount of cards for winner");

        int highestPlayer = 0;
        Farbe dominantColor = {};
        Wert highestCard = {};

        switch (trumpf)
        {
            case Trumpf::Eichel:
                dominantColor = cards.at(0).farbe;

                for (auto const card : cards)
                {
                    if (card.farbe == static_cast<Farbe>(Trumpf::Eichel))
                    {
                        dominantColor = static_cast<Farbe>(Trumpf::Eichel);
                        break;
                    }
                }

                if (dominantColor != static_cast<Farbe>(Trumpf::Eichel))
                {
                    dominantColor = cards.at(0).farbe;
                }

                for (int i = 0; i < 4; i++)
                {
                    std::cout << cards.at(i).asString() << std::endl;
                    if (cards.at(i).farbe == dominantColor && cards.at(i).wert > highestCard ||
                        cards.at(i).farbe == static_cast<Farbe>(Trumpf::Eichel) && cards.at(i).wert == Wert::Unter ||
                        cards.at(i).farbe == static_cast<Farbe>(Trumpf::Eichel) && cards.at(i).wert == Wert::Neun)
                    {
                        highestCard = cards.at(i).wert;
                        highestPlayer = i;

                        if (cards.at(i).farbe == static_cast<Farbe>(Trumpf::Eichel) && cards.at(i).wert == Wert::Unter)
                        {
                            highestCard = Wert::Bauer;
                        } else if (highestCard != Wert::Bauer && cards.at(i).farbe == static_cast<Farbe>(Trumpf::Eichel) && cards.at(i).wert == Wert::Neun)
                            highestCard = Wert::Nell;
                    }
                }

                std::cout << highestPlayer << std::endl;
                return highestPlayer;

            case Trumpf::Laub:

                return 0;

            case Trumpf::Herz:

                return 0;

            case Trumpf::Schell:

                return 0;

            case Trumpf::Bock:

                return 0;

            case Trumpf::Geiss:

                return 0;

            case Trumpf::Slalom_Bock:

                return 0;

            case Trumpf::Slalom_Geiss:

                return 0;

            default:
                return -1;
        }
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
};
