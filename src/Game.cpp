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

bool Game::IsTextureOnScreen(const Camera2D& camera, const Vector2& position, const Rectangle& frame) {
	const Vector2 currScrPos = GetWorldToScreen2D(position, camera);
	if (currScrPos.x + frame.width < 0) return false; //Left border
	if (currScrPos.x > SCREEN_SIZE) return false;     //Right border
	if (currScrPos.y + frame.height < 0) return false;//Top border
	if (currScrPos.y > SCREEN_SIZE) return false;     //Bottom border
	return true;
}

void Game::handleEvents() {
	Player::HandleInput(registry);

	if (IsKeyPressed(KEY_C)) {
		Player::Create(registry);
		ClearDungeon();
		CreateDungeon("Lmao", 1);
	}
}
void Game::UpdateMinimap() {
	if (!registry.ctx().contains<DungeonGrid>(MINIMAP_REG_NAME))
		return;

	DungeonGrid mapGrid = registry.ctx().get<DungeonGrid>(MINIMAP_REG_NAME);
	DungeonGrid dg = registry.ctx().get<DungeonGrid>(DUNGEON_REG_NAME);

	const Camera2D camera = registry.ctx().get<Camera2D>();
	for (float x = 0; x * TILE_SIZE < SCREEN_SIZE; x++) {
		for (float y = 0; y * TILE_SIZE < SCREEN_SIZE; y++) {
			Vector2 worldVec = GetScreenToWorld2D({ x * TILE_SIZE, y * TILE_SIZE }, camera);
			Vector2 dgVec = Vector2Scale(worldVec, 1.f / TILE_SIZE);

			if (dgVec.x < 0 || dgVec.y < 0 || dgVec.y >= dg.size() || dgVec.x >= dg[0].size())
				continue;

			mapGrid[dgVec.y][dgVec.x] = dg[dgVec.y][dgVec.x];
		}
	}

	registry.ctx().insert_or_assign(MINIMAP_REG_NAME, mapGrid);
}
void Game::update() {
	//TraceLog(LOG_INFO, std::to_string(GetFPS()).c_str());
	Player::Update(registry);
	UpdateMinimap();
}

void Game::RenderMinimap() {
	if (!registry.ctx().contains<DungeonGrid>(MINIMAP_REG_NAME))
		return;

	int minimapPixelSize = TextureLoader::GetTexture("Minimap_Wall").frames[0].width;
	int minimapSize = SCREEN_SIZE / TILE_SIZE * minimapPixelSize;
	//RenderTexture2D mapTex = LoadRenderTexture(minimapSize, minimapSize);
	DungeonGrid mapGrid = registry.ctx().get<DungeonGrid>(MINIMAP_REG_NAME);

	entt::entity playerEnt = registry.view<TransformComponent, Player::Status>().front();
	Vector2 playerPos = registry.get<TransformComponent>(playerEnt).position;
	playerPos = Vector2Scale(playerPos, 1.f / TILE_SIZE);
	//BeginScissorMode(0, 0, minimapSize, minimapSize);

	for (int y = 0; y < 2 * MINIMAP_RADIUS; y++) {
		for (int x = 0; x < 2 * MINIMAP_RADIUS; x++) {
			if (playerPos.x - MINIMAP_RADIUS + x < 0 || playerPos.x - MINIMAP_RADIUS + x >= mapGrid[0].size())
				continue;
			if (playerPos.y - MINIMAP_RADIUS + y < 0 || playerPos.y - MINIMAP_RADIUS + y >= mapGrid.size())
				continue;

			switch (mapGrid[playerPos.y - MINIMAP_RADIUS + y][playerPos.x - MINIMAP_RADIUS + x]) {
			case C_WALL:
				DrawRectangle(x * minimapPixelSize, y * minimapPixelSize,
					minimapPixelSize, minimapPixelSize, WHITE);
				break;
			case C_STAIR:
				DrawRectangle(x * minimapPixelSize, y * minimapPixelSize,
					minimapPixelSize, minimapPixelSize, BLUE);
				break;
			}
		}
	}

	DrawRectangle(MINIMAP_RADIUS * minimapPixelSize, MINIMAP_RADIUS * minimapPixelSize,
		minimapPixelSize, minimapPixelSize, GREEN);

	//EndScissorMode();
}

void Game::RenderTextureComponents() {
	const Camera2D camera = registry.ctx().get<const Camera2D>();
	auto renderObjects = registry.view<TransformComponent, TextureComponent>();
	for (int currLayer = 0; currLayer < RenderLayer::ENUM_END; currLayer++) {
		bool useCamera = GUILayers.find(currLayer) == GUILayers.end();

		if (useCamera) BeginMode2D(camera);
		for (auto [entity, transform, texture] : renderObjects.each()) {
			if (currLayer != texture.targetLayer) continue;
			if (++texture.currentFrame >= texture.frames.size())
				texture.currentFrame = 0;
			if (!useCamera || IsTextureOnScreen(camera, transform.position, texture.frames[texture.currentFrame]))
				DrawTextureRec(texture.texture, texture.frames[texture.currentFrame], transform.position, WHITE);
		}
		if (useCamera) EndMode2D();
	}
}

void Game::render() {
	BeginDrawing();
	ClearBackground(BLACK);
	
	RenderTextureComponents();
	RenderMinimap();

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

	registry.ctx().emplace_as<DungeonGrid>(DUNGEON_REG_NAME, dungeonGrid);
	registry.ctx().emplace_as<DungeonGrid>(MINIMAP_REG_NAME, 
		DungeonGrid(dungeonGrid.size(), std::vector<int>(dungeonGrid[0].size(), 0)
	));

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
	registry.ctx().erase<DungeonGrid>(DUNGEON_REG_NAME);
	registry.ctx().erase<DungeonGrid>(MINIMAP_REG_NAME);
	for (auto [entity, texture] : registry.view<TextureComponent>().each()) {
		if (texture.targetLayer == RenderLayer::Dungeon) {
			registry.destroy(entity);
		}
	}
}