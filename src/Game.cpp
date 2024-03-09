#include <raylib.h>
#include <nlohmann\json.hpp>
#include <entt\entt.hpp>

#include "Auxiliaries.h"
#include "TextureLoader.h"
#include "Player.h"

#include "Game.h"


Game::Game(const char* title, int width, int height) {
	InitWindow(width, height, title);
	SetTargetFPS(FPS);
	TextureLoader::LoadTextureFromJson("resource/textures.json");
}

Game::~Game() {
	TextureLoader::DeleteTextures();
	CloseWindow();
}


void Game::handleEvents() {
	Player::HandleInput(registry);

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

	for (int currLayer = 0; currLayer < RenderLayer::ENUM_END; currLayer++) {
		for (auto [entity, transform, texture] : renderObjects.each()) {
			if (currLayer != texture.targetLayer) continue;
			if (++texture.currentFrame >= texture.frames.size())
				texture.currentFrame = 0;
			DrawTextureRec(texture.texture, texture.frames[texture.currentFrame], transform.position, WHITE);
		}
	}

	EndDrawing();
}