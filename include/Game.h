#pragma once

#include <string>
#include <entt/entt.hpp>
#include <raylib.h>
#include <map>
#include "DungeonGenerator.h"

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

	inline static const std::string TEXTURE_DUNGEON_WALL = "Dungeon_Wall";
	inline static const std::string TEXTURE_DUNGEON_CORR = "Dungeon_Corr";
	inline static const std::string TEXTURE_DUNGEON_ROOM = "Dungeon_Room";

	inline static const std::map<int, Color> DUNGEON_CELL_COLOR = { {C_WALL, WHITE}, { C_ROOM, GRAY }, {C_STAIR, BLUE} };
	
	enum GameState {MENU, PAUSE, RUNNING, OVER};
	GameState currentState;



	bool IsTextureOnScreen(const Camera2D& camera, const Vector2& position, const Rectangle& frame);
	void GameMenu();
	void GameStart();
	void GameEnd();
	bool CheckPlayerAtStair();
	void UpdateMinimap();
	void RenderMainMenu();
	void RenderMinimap();
	void RenderTextureComponents();
	void CreateDungeon(int difficulty, const std::string seed = "");
	void ClearDungeon();
};