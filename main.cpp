#include "Generic.h"
#include "Deck.h"
#include "Player.h"
#include "Game.h"

int main()
{
    Deck deck;

    Player player1(deck.getCards(0));
    Player player2(deck.getCards(1));
    Player player3(deck.getCards(2));
    Player player4(deck.getCards(3));


    //std::cout << sizeof(card) << std::endl;

    playGame(player1, player2, player3, player4);

    return 0;
}

