#include "Player.h"
#include "Card.h"
#include "Game.h"

void playGame(Player player1, Player player2, Player player3, Player player4)
{
    std::vector<Card> cardsPlayed;
    std::array<Player, 4> players = {player1, player2, player3, player4};
    int lastWinner = 0;
    int currentPlayer = 0;

    //Set Trumpf
    player1.setTrumpf();

    for (int round = 0; round < 9; round++)
    {
        for (int i = 0; i < 4; i++)
        {
            cardsPlayed.push_back(players.at(currentPlayer + i % 4).playCard(round));
        }


    }

    for (auto card : cardsPlayed)
    {
        std::cout << card.asString() << std::endl;
    }
}