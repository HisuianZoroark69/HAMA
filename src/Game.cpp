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

	for (int currLayer = 0; currLayer < RenderLayer::ENUM_END; currLayer++) {
		for (auto [entity, transform, texture] : renderObjects.each()) {
			if (currLayer != texture.targetLayer) continue;
			
			Rectangle frameRec = { texture.currentFrame++ * TILE_SIZE, texture.yStart, TILE_SIZE, TILE_SIZE };
			if (texture.currentFrame > texture.xEnd) texture.currentFrame = texture.xStart;
			DrawTextureRec(texture.texture, frameRec, transform.position, WHITE);
		}
	}

	EndDrawing();
}