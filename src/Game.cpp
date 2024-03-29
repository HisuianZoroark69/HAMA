#include <raylib.h>

#include "Player.h"
#include "DungeonGenerator.h"
#include "TextureLoader.h"

#include "Game.h"

//Todo: Add game states such as menu and stuffs

//Todo: Add minimap

Game::Game(const char* title, int width, int height) {
	InitWindow(width, height, title);
	SetTargetFPS(FPS);
	registry.ctx().emplace<Camera2D>(Camera2D{ .zoom = 1.f});
	TextureLoader::LoadTextureFromJson("resource/textures.json");
}

Game::~Game() {
	TextureLoader::DeleteTextures();
	CloseWindow();
}


void Game::handleEvents() {
	Player::HandleInput(registry);

	if (IsKeyPressed(KEY_C)) {
		Player::Create(registry);
		ClearDungeon();
		CreateDungeon("Lmao", 1);
	}
}
void Game::update() {
	//TraceLog(LOG_INFO, std::to_string(GetFPS()).c_str());
	Player::Update(registry);
}

bool Game::IsTextureOnScreen(const Camera2D& camera, const Vector2& position, const Rectangle& frame) {
	const Vector2 currScrPos = GetWorldToScreen2D(position, camera);
	if (currScrPos.x + frame.width < 0) return false; //Left border
	if (currScrPos.x > SCREEN_SIZE) return false;     //Right border
	if (currScrPos.y + frame.height < 0) return false;//Top border
	if (currScrPos.y > SCREEN_SIZE) return false;     //Bottom border
	return true;
}

void Game::render() {
	auto renderObjects = registry.view<TransformComponent, TextureComponent>();
	BeginDrawing();
	ClearBackground(BLACK);

	const Camera2D camera = registry.ctx().get<const Camera2D>();

	for (int currLayer = 0; currLayer < RenderLayer::ENUM_END; currLayer++) {
		bool useCamera = GUILayers.find(currLayer) == GUILayers.end();

		if (useCamera) BeginMode2D(camera);
		for (auto [entity, transform, texture] : renderObjects.each()) {
			if (currLayer != texture.targetLayer) continue;
			if (++texture.currentFrame >= texture.frames.size())
				texture.currentFrame = 0;
			if(!useCamera || IsTextureOnScreen(camera, transform.position, texture.frames[texture.currentFrame]))
				DrawTextureRec(texture.texture, texture.frames[texture.currentFrame], transform.position, WHITE);
		}
		if (useCamera) EndMode2D();
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
	std::pair<int, int> spawnPos;
	DungeonGrid dungeonGrid = dg.Generate(data, spawnPos);

	//Make players spawn at designated position
	auto players = registry.view<TransformComponent, Player::Status>().each();
	for (auto [entity, transform, status] : players) {
		transform.position.x = spawnPos.first * TILE_SIZE;
		transform.position.y = spawnPos.second * TILE_SIZE;
		transform.orientation = Vector2Zero();
	}

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