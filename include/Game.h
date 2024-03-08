#pragma once

#define FPS 60

#include <entt\entt.hpp>
#include <raylib.h>

/**
 * @brief Render layer enum
 * @var RenderLayers::ENUM_END Used to get the layer size for rendering
 */
enum RenderLayer { Background, Player, GUI, ENUM_END };

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
};