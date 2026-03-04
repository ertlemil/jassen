#include "Player.h"
#include "Card.h"
#include "Game.h"

std::map<Wert, int> cardValues = {
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

void playGame(Player player1, Player player2, Player player3, Player player4)
{
    std::vector<Card> cardsPlayed;
    std::vector<Card> currentCards;
    std::array<Player, 4> players = {player1, player2, player3, player4};
    int lastWinner = 0;
    int currentWinner;
    int currentPlayer = 0;
    int collectivePoints = 0;

    //Set Trumpf
    player1.setTrumpf();

    for (int round = 0; round < 9; round++)
    {
        for (int i = 0; i < 4; i++)
        {
            currentCards.push_back(players.at((currentPlayer + i) % 4).playCard(round));
        }

        currentWinner = decideWinner(currentCards);
        players[(lastWinner + currentWinner) % 4].points += calculatePoints(currentCards, round);
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

int calculatePoints(std::vector<Card>& cards, int round)
{

    if (cards.size() != 4)
        throw std::length_error("Fehlerhafte Anzahl Karten bei Punktebewertung");

    return (round == 8 ? 5 : 0) + std::accumulate(
        cards.begin(),
        cards.end(), 0,
        [](int sum, const Card& card)
        {
            int value = cardValues.at(card.wert);

            if (Player::trumpf == static_cast<Trumpf>(card.farbe))
            {
                if (card.wert == Wert::Neun)
                    value = 14;
                if (card.wert == Wert::Unter)
                    value = 20;
            } else if (
                Player::trumpf == Trumpf::Bock ||
                Player::trumpf == Trumpf::Geiss ||
                Player::trumpf == Trumpf::Slalom_Bock ||
                Player::trumpf == Trumpf::Slalom_Geiss)
            {
                if (card.wert == Wert::Acht)
                    value = 8;
            }

            return sum + value;
        }
    );
}

int decideWinner(std::vector<Card>& cards)
{
    if (cards.size() != 4)
        throw std::length_error("Not the right amount of cards for winner");

    int highestPlayer = 0;
    Farbe dominantColor = {};
    Wert highestCard = {};

    switch (Player::trumpf)
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