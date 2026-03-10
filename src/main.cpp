#include "Game.h"

int main()
{
    Game game;

    //game.play();

    std::cout << game.state.deck.getCards(0)[0].asString() << std::endl;
    game.reset();
    std::cout << game.state.deck.getCards(0)[0].asString() << std::endl;

    return 0;
}

