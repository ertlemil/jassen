cmake_minimum_required(VERSION 3.28)
project(Jassen)

set(CMAKE_CXX_STANDARD 26)

add_executable(Jassen main.cpp
        Card.h
        Deck.h
        Player.h
        Game.cpp
        Generic.h
        Game.h)
