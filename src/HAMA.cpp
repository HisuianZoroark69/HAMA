// HAMA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Game.h"
#include "Auxiliaries.h"

int main()
{
    Game game("Haki and Miyeon adventure", SCREEN_SIZE, SCREEN_SIZE);
    while (!WindowShouldClose()) {
        game.handleEvents();
        game.update();
        game.render();
    }
}