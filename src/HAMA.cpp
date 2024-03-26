// HAMA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <raylib.h>
#include "Game.h"

int main()
{
    Game game("Haki and Miyeon adventure", 640, 640);
    while (!WindowShouldClose()) {
        game.handleEvents();
        game.update();
        game.render();
    }
}