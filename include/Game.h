#pragma once

#include <string>
#include <entt/entt.hpp>
#include <raylib.h>
#include <map>
#include "DungeonGenerator.h"

constexpr int LEVELS = 5;

class Game {
public:

	/**
	 * @brief Initialize a new game
	 * @param title Window's title
	 * @param width Window's width
	 * @param height Windows's height
	 */
	Game(const char* title, int width, int height);

	/**
	* @brief Deinitialize the game and quit
	*/
	~Game();

	/**
	 * @brief Handle events and inputs
	 */
	void handleEvents();

	/**
	 * @brief Update objects
	 */
	void update();

	/**
	 * @brief Render objects
	 */
	void render();


private:
	/**
	 * @brief Main Entity Component System (ECS) manager
	 */
	entt::registry registry;
	entt::entity player;

	bool minimapFullscreen;
	Vector2 minimapFullscreenOffset;
	int dungeonDifficulty = 5;

	inline static const std::array<std::string, 3> TEXTURE_DUNGEON_WALL = { "Dungeon_Wall_Spring", "Dungeon_Wall_Autumn", "Dungeon_Wall_Winter"};
	inline static const std::array<std::string, 3> TEXTURE_DUNGEON_CORR = { "Dungeon_Corr_Spring", "Dungeon_Corr_Autumn", "Dungeon_Corr_Winter" };
	inline static const std::array<std::string, 3> TEXTURE_DUNGEON_ROOM = { "Dungeon_Room_Spring", "Dungeon_Room_Autumn", "Dungeon_Room_Winter" };
	inline static const std::string TEXTURE_DUNGEON_STAIR = "Dungeon_Stair";

	inline static const std::map<int, Color> DUNGEON_CELL_COLOR = { {C_WALL, WHITE}, {C_WALL_INVIS, WHITE}, { C_ROOM, GRAY }, { C_ROOM_INVIS, GRAY }, {C_STAIR, BLUE} };
	
	enum GameState {MENU, CHAR_CHOOSE, RUNNING, OVER, WIN};
	GameState currentState;

	bool IsTextureOnScreen(const Camera2D& camera, const Vector2& position, const Rectangle& frame);
	void GameMenu();
	void GameStart(int character);
	void GameEnd(bool win);
	bool CheckPlayerAtStair();
	void UpdateMinimap();
	void RenderMainMenu();
	void RenderCharacterChooseMenu();
	void RenderGameOver(bool win);
	void RenderMinimap();
	void RenderHungerBar();
	void RenderTextureComponents();
	void PlaceDungeonComponent(pcg32& rng, const DungeonGrid& dungeonGrid);
	void CreateDungeon(int difficulty, const std::string seed = "");
	void CreateBossFloor();
	void ClearDungeon();
};