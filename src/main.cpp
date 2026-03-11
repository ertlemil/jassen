#include "Game.h"

int main()
{
    Game game;

    game.reset();
    game.step(0);
    game.step(0);

    std::cout << game.state.deck.getCards(0)[0].asString() << std::endl;
    game.reset();
    std::cout << game.state.deck.getCards(0)[0].asString() << std::endl;

    return 0;
}

