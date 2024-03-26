#include <raylib.h>
#include <nlohmann\json.hpp>
#include <entt\entt.hpp>

#include "Auxiliaries.h"
#include "TextureLoader.h"
#include "Player.h"
#include "DungeonGenerator.h"

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
		ClearDungeon();
		CreateDungeon("Lmao", 1);
	}
	if (IsKeyPressed(KEY_P)) {
		Player::Create(registry);
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

void Game::CreateDungeon(const std::string seed, int difficulty) {
	pcg32 rng(GetRandomSeed());
	DungeonGenerateData data = {
		.seed = seed,
		.width = GetRandomOddNumber(rng, MINIMUM_DUNGEON_SIZE, MINIMUM_DUNGEON_SIZE * (difficulty + 1), true),
		.height = GetRandomOddNumber(rng, MINIMUM_DUNGEON_SIZE, MINIMUM_DUNGEON_SIZE * (difficulty + 1), true),
		.roomAmount = MINIMUM_ROOM_AMOUNT * difficulty
	};

	DungeonGenerator dg;
	DungeonGrid dungeonGrid = dg.Generate(data);
	registry.ctx().emplace<DungeonGrid>(dungeonGrid);

	for (float y = 0; y < dungeonGrid.size(); y++) {
		for (float x = 0; x < dungeonGrid[y].size(); x++) {
			auto entity = registry.create();
			registry.emplace<TransformComponent>(entity, Vector2{x * TILE_SIZE, y * TILE_SIZE}, Direction{0,0});
			
			switch (dungeonGrid[y][x]) {
			case C_CORR:
			case C_DOOR:
				registry.emplace<TextureComponent>(entity, TextureLoader::GetTexture(TEXTURE_DUNGEON_CORR));
				break;
			case C_WALL:
				registry.emplace<TextureComponent>(entity, TextureLoader::GetTexture(TEXTURE_DUNGEON_WALL));
				break;
			case C_ROOM:
				registry.emplace<TextureComponent>(entity, TextureLoader::GetTexture(TEXTURE_DUNGEON_ROOM));
				break;
			}
		}
	}
}

void Game::ClearDungeon() {
	registry.ctx().erase<DungeonGrid>();
	for (auto [entity, texture] : registry.view<TextureComponent>().each()) {
		if (texture.targetLayer == RenderLayer::Dungeon) {
			registry.destroy(entity);
		}
	}
}