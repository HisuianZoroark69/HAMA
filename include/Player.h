#pragma once

#include <raylib.h>
#include <raymath.h>
#include <entt\entt.hpp>

#include "Auxiliaries.h"

struct Player {
	/**
	* @struct Status
	* @brief Player statuses
	*/
	struct Status {
		const std::string name;
		int HP;
		Direction movingDirection;
	};

	/**
	 * @brief Create a player
	 * @param registry The ECS registry
	 * @param name Player's name
	 * @param textureFile Player's textureFile
	 */
	static entt::entity Create(entt::registry& registry, const std::string name, const char* textureFile);

	/**
	 * @brief Remove player entity
	 */
	static void Remove(entt::registry& registry, entt::entity& entity);

	/**
	 * @brief Handle input
	 */
	static void HandleInput(entt::registry& registry);

	/**
	 * @brief Update
	 */
	static void Update(entt::registry& registry);

private:
	static void ClampMovementInTile(Vector2& transform, Vector2& movingDirection);
};