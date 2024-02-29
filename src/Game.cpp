#include "game.h"
#include <raylib.h>
#include <iostream>

Game::Game(const char* title, int width, int height) {
	InitWindow(width, height, title);
	SetTargetFPS(60);
}
Game::~Game() {
	CloseWindow();
}
void Game::handleEvents() {
	if (IsKeyPressed(KEY_A)) std::cout << "A";
}
void Game::update() {

}

void Game::render() {
	BeginDrawing();
	//TODO: Draw
	EndDrawing();
}