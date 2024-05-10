#pragma once

#include <raylib.h>
#include <raymath.h>
#include <entt\entt.hpp>

#include "DungeonGenerator.h"
#include "Auxiliaries.h"
/**
* @struct PlayerStatus
* @brief Player statuses
*/
struct PlayerStatus {
	int HP = 0;
	Direction movingDirection = { 0,0 };
	bool isSprinting = false;
	int hunger = 100;
	//std::vector<ItemID> items;
};
struct Player {
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
	static void Update(entt::entity& player, entt::registry& registry);

private:
	inline static const std::string TEXTURE_IDLE = "Player_Idle";
	inline static const std::string TEXTURE_RUNNING = "Player_Running";

	static void ClampMovementInTile(Vector2& transform, Vector2& movingDirection, bool isSprinting);
	static void ChangeTexture(TextureComponent& oldTexture, TextureComponent newTexture);
	static void UpdateCameraPosition(const Vector2& position, entt::registry& registry);
	static bool HandleInputDirection(Vector2& movingDirection);
	static void HandleMovementInDungeon(const DungeonGrid& dg, Vector2& position, Vector2& movingDirection);
};