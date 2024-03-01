#include "Game.h"
#include "Player.h"
#include "Auxiliaries.h"
#include <raylib.h>

#include <iostream>

Game::Game(const char* title, int width, int height) {
	InitWindow(width, height, title);
	SetTargetFPS(FPS);
}
Game::~Game() {
	CloseWindow();
}
void Game::handleEvents() {
}
void Game::update() {
	Player::Update(registry);
}

void Game::render() {
	BeginDrawing();
	//TODO: Draw
	DrawRectangle(0, 0, 128, 128, BLUE);
	DrawRectangle(128, 0, 256, 256, RED);
	DrawRectangle(128 + 256, 0, 64, 64, GREEN);
	EndDrawing();
}