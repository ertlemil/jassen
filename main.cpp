#include "Generic.h"
#include "Deck.h"
#include "Player.h"
#include "Game.h"

int main()
{
    Game game;

    game.play();

    /*std::vector<Card> test = {Card(Farbe::Laub,Wert::Acht), Card(Farbe::Laub,Wert::Neun), Card(Farbe::Laub,Wert::Unter), Card(Farbe::Laub,Wert::Ass)};

    game.trumpf = Trumpf::Geiss;
    std::cout << game.decideWinner(test) << std::endl;*/

    return 0;
}

