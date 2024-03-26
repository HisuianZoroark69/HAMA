#pragma once

#include <entt\entt.hpp>

#include "Auxiliaries.h"
#include "TextureLoader.h"

#define FPS 60

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

	void CreateDungeon(const std::string seed, int difficulty);

	void ClearDungeon();

private:
	/**
	 * @brief Main Entity Component System (ECS) manager
	 */
	entt::registry registry;

	inline static const std::string TEXTURE_DUNGEON_WALL = "Dungeon_Wall";
	inline static const std::string TEXTURE_DUNGEON_CORR = "Dungeon_Corr";
	inline static const std::string TEXTURE_DUNGEON_ROOM = "Dungeon_Room";
};