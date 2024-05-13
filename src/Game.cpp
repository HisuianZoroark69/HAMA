#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include "Player.h"
#include "TextureLoader.h"
#include "Game.h"

Game::Game(const char* title, int width, int height) {
	InitWindow(width, height, title);
	SetTargetFPS(FPS);
	SetExitKey(KEY_NULL);
	registry.ctx().emplace<Camera2D>(Camera2D{ .zoom = 1.f});
	TextureLoader::LoadTextureFromJson("resource/textures.json");

	currentState = MENU;
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
void Game::GameMenu() {
	currentState = MENU;
}
void Game::GameStart() {
	currentState = RUNNING;
	player = Player::Create(registry);
	ClearDungeon();
	dungeonDifficulty = 1;
	CreateDungeon(dungeonDifficulty++);
}
void Game::GameEnd() {
	currentState = OVER;
}

bool Game::CheckPlayerAtStair() {
	if (!registry.ctx().contains<DungeonGrid>(MINIMAP_REG_NAME))
		return false;

	const DungeonGrid mapGrid = registry.ctx().get<const DungeonGrid>(MINIMAP_REG_NAME);

	entt::entity playerEnt = registry.view<TransformComponent, PlayerStatus>().front();
	Vector2 playerPos = registry.get<TransformComponent>(playerEnt).position;
	playerPos = Vector2Scale(playerPos, 1.f / TILE_SIZE);

	return mapGrid[playerPos.y][playerPos.x] == C_STAIR;
}

void Game::handleEvents() {
	switch (currentState)
	{
	case Game::RUNNING:
		Player::HandleInput(registry);
		if (CheckPlayerAtStair()) {
			if (dungeonDifficulty > LEVELS) {
				currentState = OVER;
				return;
			}
			ClearDungeon();
			CreateDungeon(dungeonDifficulty++);
		}
		if (IsKeyPressed(KEY_M)) {
			minimapFullscreen = !minimapFullscreen;
			minimapFullscreenOffset = Vector2Zero();
		}
		if (minimapFullscreen && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			minimapFullscreenOffset = Vector2Add(minimapFullscreenOffset, GetMouseDelta());
		}
		break;
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
	switch (currentState) {
	case RUNNING:
		Player::Update(player, registry);
		if (!registry.valid(player) && currentState == RUNNING) {
			GameEnd();
			break;
		}
		UpdateMinimap();
		break;
	}
}

void Game::RenderMainMenu() {
	GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
	DrawText("Haki and Miyeon\n\n\n\n\t\tadventure", 90, 120, 60, WHITE);
	//Game start button
	if (GuiButton({ 320 - 50,320 - 25, 100, 50}, "Start")) {
		GameStart();
	}
}
void Game::RenderGameOver() {
	GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

	DrawText("Game Over", 170, 120, 60, WHITE);
	//Game start button
	if (GuiButton({ 320 - 150/2,320 - 25, 150, 50 }, "Play Again")) {
		GameStart();
	}
}

void Game::RenderMinimap() {
	if (!registry.ctx().contains<DungeonGrid>(MINIMAP_REG_NAME))
		return;

	float minimapPixelSize = TextureLoader::GetTexture("Minimap_Wall").frames[0].width;
	int minimapSize = SCREEN_SIZE / TILE_SIZE * minimapPixelSize;
	DungeonGrid mapGrid = registry.ctx().get<DungeonGrid>(MINIMAP_REG_NAME);

	Vector2 playerPos = registry.get<TransformComponent>(player).position;
	playerPos = Vector2Scale(playerPos, 1.f / TILE_SIZE);

	if (minimapFullscreen) {
		ClearBackground(BLACK);
		minimapPixelSize *= 2;
	}
	int extra = minimapFullscreen ? std::max(mapGrid.size(), mapGrid[0].size()) * minimapPixelSize : 0;
	int rad = minimapFullscreen ? (SCREEN_SIZE + extra) / minimapPixelSize / 2 : MINIMAP_RADIUS;
	Vector2 minimapMidOffset = Vector2AddValue(minimapFullscreenOffset, -extra / 2);
	
	for (int y = 0; y < rad * 2; y++) {
		for (int x = 0; x < rad * 2; x++) {
			if (playerPos.x - rad + x < 0 || playerPos.x - rad + x >= mapGrid[0].size())
				continue;
			if (playerPos.y - rad + y < 0 || playerPos.y - rad + y >= mapGrid.size())
				continue;

			auto currCell = mapGrid[playerPos.y - rad + y][playerPos.x - rad + x];
			if (DUNGEON_CELL_COLOR.find(currCell) != DUNGEON_CELL_COLOR.end()) {
				auto color = DUNGEON_CELL_COLOR.at(currCell);
				DrawRectangleV(
					Vector2Add({ x * minimapPixelSize, y * minimapPixelSize }, minimapMidOffset),
					{ minimapPixelSize, minimapPixelSize },
					color);
			}
		}
	}
	//Draw player on map
	DrawRectangleV(
		Vector2Add({ rad * minimapPixelSize, rad * minimapPixelSize }, minimapMidOffset),
		{ minimapPixelSize, minimapPixelSize },
	GREEN);
	
}

void Game::RenderHungerBar() {
	if (minimapFullscreen) return;

	PlayerStatus stat = registry.get<PlayerStatus>(player);
	DrawRectangleLinesEx({ 0, 70, 100, 10}, 2, BLACK);
	Color HungerColor;
	if (stat.hunger <= 15) HungerColor = RED;
	else if (stat.hunger <= 30) HungerColor = YELLOW;
	else HungerColor = GREEN;
	DrawRectangle(0, 70, stat.hunger, 10, HungerColor);

	DrawText(TextFormat("%0.0f", stat.hunger), 105, 70, 20, BLACK);
	DrawText(TextFormat("Apples: %d", stat.apples), 0, 90, 20, BLACK);
	DrawText(TextFormat("%dF", dungeonDifficulty - 1), 0, 110, 20, BLACK);
}

void Game::RenderTextureComponents() {
	Camera2D camera = registry.ctx().get<Camera2D>();
	auto renderObjects = registry.view<TransformComponent, TextureComponent>();

	std::map<RenderLayer, std::vector<std::pair<TransformComponent, TextureComponent>>> renderObjectMap;
	for (auto [_,transform, texture] : renderObjects.each()) {
		if (++texture.currentFrame >= texture.frames.size())
			texture.currentFrame = 0;
		if (!IsTextureOnScreen(camera, transform.position, texture.frames[texture.currentFrame]))
			continue;
		renderObjectMap[texture.targetLayer].push_back({ transform, texture });
	}

	for (int currLayer = 0; currLayer < RenderLayer::ENUM_END; currLayer++) {
		if (renderObjectMap.find(static_cast<RenderLayer>(currLayer)) == renderObjectMap.end()) continue;

		bool useCamera = GUILayers.find(currLayer) == GUILayers.end();
		if (useCamera) BeginMode2D(camera);

		for (auto [transform, texture] : renderObjectMap[static_cast<RenderLayer>(currLayer)]) {
			DrawTextureRec(texture.texture, texture.frames[texture.currentFrame], transform.position, WHITE);
		}

		if (useCamera) EndMode2D();
	}
}

void Game::render() {
	BeginDrawing();
	ClearBackground(BLACK);
	
	switch (currentState) {
	case MENU:
		RenderMainMenu();
		break;
	case RUNNING:
		RenderTextureComponents();
		RenderMinimap();
		RenderHungerBar();
		break;
	case OVER:
		RenderGameOver();
		break;
	}
	
	EndDrawing();
}

void Game::CreateDungeon(int difficulty, const std::string seed) {
	pcg32 rng(GetRandomSeed());
	DungeonGenerateData data = {
		.seed = (seed == "") ? std::to_string(GetRandomSeed()) : seed,
		.width = GetRandomOddNumber(rng, MINIMUM_DUNGEON_SIZE, MINIMUM_DUNGEON_SIZE * (std::sqrt(difficulty + 1) + 1)),
		.height = GetRandomOddNumber(rng, MINIMUM_DUNGEON_SIZE, MINIMUM_DUNGEON_SIZE * (std::sqrt(difficulty + 1) + 1)),
		.roomAmount = MINIMUM_ROOM_AMOUNT * difficulty
	};

	DungeonGenerator dg;
	std::pair<int, int> spawnPos;
	DungeonGrid dungeonGrid = dg.Generate(data, spawnPos);

	//Make players spawn at designated position
	auto players = registry.view<TransformComponent, PlayerStatus>().each();
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

	//Create items
	std::vector<std::pair<Vector2, entt::entity>> itemList;
	auto itemPosList = dg.GenerateApplesPosition(rng, dungeonGrid, 0.07 * powf(2, -(difficulty - 1)));
	for (auto itemPos : itemPosList) {
		auto entity = registry.create();
		auto itemPosReal = Vector2{ static_cast<float>(itemPos.first), static_cast<float>(itemPos.second) };

		itemPosReal = Vector2Scale(itemPosReal, TILE_SIZE);
		registry.emplace<TransformComponent>(entity, itemPosReal, Direction{0,0});
		registry.emplace<TextureComponent>(entity, TextureLoader::GetTexture("Apple"));

		itemList.push_back({ itemPosReal, entity });
	}
	registry.ctx().emplace_as<std::vector<std::pair<Vector2, entt::entity>>>(ITEM_LIST_REG_NAME, itemList);

	//Change player's hunger consumption rate
	registry.get<PlayerStatus>(player).hungerConsumePerTile = BASE_HUNGER_CONSUMPTION_PER_TILE * difficulty;
}

void Game::ClearDungeon() {
	registry.ctx().erase<DungeonGrid>(DUNGEON_REG_NAME);
	registry.ctx().erase<DungeonGrid>(MINIMAP_REG_NAME);
	registry.ctx().erase<std::vector<std::pair<Vector2, entt::entity>>>(ITEM_LIST_REG_NAME);

	for (auto [entity, texture] : registry.view<TextureComponent>().each()) {
		if (texture.targetLayer == RenderLayer::Dungeon || texture.targetLayer == RenderLayer::Item) {
			registry.destroy(entity);
		}
	}
}