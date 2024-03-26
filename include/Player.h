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
		int HP = 0;
		Direction movingDirection = { 0,0 };
	};


	/**
	 * @brief Create a player
	 * @param registry The ECS registry
	 * @param name Player's name
	 * @param textureFile Player's textureFile
	 */
	static entt::entity Create(entt::registry& registry);

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
	inline static const std::string TEXTURE_IDLE = "Player_Idle";
	inline static const std::string TEXTURE_RUNNING = "Player_Running";

	static void ClampMovementInTile(Vector2& transform, Vector2& movingDirection);
	static void ChangeTexture(TextureComponent& oldTexture, TextureComponent newTexture);
};