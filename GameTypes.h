#pragma once

enum class Trumpf { Herz, Laub, Schell, Eichel, Bock, Geiss, Slalom_Bock, Slalom_Geiss };
enum class Farbe { Herz, Laub, Schell, Eichel };
enum class Wert { Sechs, Sieben, Acht, Neun, Zehn, Unter, Ober, Koenig, Ass, Nell, Bauer };
struct GameState
{
    float ownPoints;
    float enemyPoints;
    Trumpf trick;
    float slalomDirection;
    std::array<float, 4> trickDecider;
    std::array<float, 36>& ownCards;
    std::array<float, 36>& playedCards;
    std::array<float, 108>& knownCards;
    std::array<float, 12>& voidColors;
    std::array<float, 36>& currentTrick_1;
    std::array<float, 36>& currentTrick_2;
    std::array<float, 36>& currentTrick_3;
};