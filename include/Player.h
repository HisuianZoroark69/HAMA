#pragma once

#include <raylib.h>
#include <raymath.h>
#include <entt\entt.hpp>

#include "DungeonGenerator.h"
#include "Auxiliaries.h"

constexpr int APPLE_RECOVERY_AMOUNT = 20;
constexpr float BASE_HUNGER_CONSUMPTION_PER_TILE = 0.08;

/**
* @struct PlayerStatus
* @brief Player statuses
*/
struct PlayerStatus {
	int playerCharacterId = 0;
	int HP = 0;
	Direction movingDirection = { 0,0 };
	bool isSprinting = false;
	float hungerConsumePerTile = BASE_HUNGER_CONSUMPTION_PER_TILE;
	float hunger = 100;
	int apples = 0;
};
struct Player {
	/**
	 * @brief Create a player
	 * @param registry The ECS registry
	 * @param name Player's name
	 * @param textureFile Player's textureFile
	 */
	static entt::entity Create(entt::registry& registry, int character);

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
	inline static const std::array<std::string, 2> TEXTURE_IDLE = { "Haki_Idle", "Miyeon_Idle"};
	inline static const std::array<std::string, 2> TEXTURE_RUNNING = { "Haki_Running" , "Miyeon_Running"};

	static void ClampMovementInTile(Vector2& transform, Vector2& movingDirection, bool isSprinting);
	static void ChangeTexture(TextureComponent& oldTexture, TextureComponent newTexture);
	static void UpdateCameraPosition(const Vector2& position, entt::registry& registry);
	static bool HandleInputDirection(Vector2& movingDirection);
	static bool HandleMovementInDungeon(const DungeonGrid& dg, Vector2& position, Vector2& movingDirection);
};