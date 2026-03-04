#pragma once

#include "Generic.h"

void playGame(Player player1, Player player2, Player player3, Player player4);
int calculatePoints(std::vector<Card>& cards, int round);
int decideWinner(std::vector<Card>& cards);