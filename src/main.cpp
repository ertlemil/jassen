#include "Game.h"

int main()
{
    Game game;

    game.step(game.state.players.at(game.state.currentPlayer).cards.at(0).toArrayPosition());

    return 0;
}

