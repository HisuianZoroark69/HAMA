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
	if (IsKeyPressed(KEY_C)) {
		Player::Create(registry, "a", "resource/Player.png");
	}
}
void Game::update() {
	Player::Update(registry);
}

void Game::render() {
	auto renderObjects = registry.view<TransformComponent, TextureComponent>();

	BeginDrawing();
	ClearBackground(BLACK);
	//TODO: Draw
	for (auto [entity, transform, texture] : renderObjects.each()) {
		Rectangle frameRec = { texture.currentFrame * TILE_SIZE, texture.drawRectY, TILE_SIZE, TILE_SIZE };
		DrawTextureRec(texture.texture, frameRec, transform.position, WHITE);
	}

	EndDrawing();
}