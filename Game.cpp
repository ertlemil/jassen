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
    {Wert::König, 4},
    {Wert::Ass, 11}
};

void playGame(Player player1, Player player2, Player player3, Player player4)
{
    std::vector<Card> cardsPlayed;
    std::vector<Card> currentCards;
    std::array<Player, 4> players = {player1, player2, player3, player4};
    int lastWinner = 0;
    int currentPlayer = 0;
    int collectivePoints = 0;

    //Set Trumpf
    player1.setTrumpf();

    for (int round = 0; round < 9; round++)
    {
        for (int i = 0; i < 4; i++)
        {
            currentCards.push_back(players.at(currentPlayer + i % 4).playCard(round));
        }

        collectivePoints += calculatePoints(currentCards, round);
        currentCards.clear();
    }

    for (auto card : currentCards)
    {
        std::cout << card.asString() << std::endl;
    }

    std::println("{}", collectivePoints);
}

int calculatePoints(std::vector<Card>& cards, int round)
{
    //int color = static_cast<int>(cards.at(0).farbe);

    if (cards.size() != 4)
        throw std::length_error("Fehlerhafte Anzahl Karten bei Punktebewertung");

    return (round == 8 ? 5 : 0) + std::accumulate(
        cards.begin(),
        cards.end(), 0,
        [](int sum, const Card& card)
        {
            int value = cardValues.at(card.wert);

            if (Player::trumpf == static_cast<int>(card.farbe))
            {
                if (card.wert == Wert::Neun)
                    value = 14;
                if (card.wert == Wert::Unter)
                    value = 20;
            } else if (
                Player::trumpf == static_cast<int>(Trumpf::Bock) ||
                Player::trumpf == static_cast<int>(Trumpf::Geiß) ||
                Player::trumpf == static_cast<int>(Trumpf::Slalom_Bock) ||
                Player::trumpf == static_cast<int>(Trumpf::Slalom_Geiß))
            {
                if (card.wert == Wert::Acht)
                    value = 8;
            }

            return sum + value;
        }
    );

}

