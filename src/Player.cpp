#include "Player.h"
#include "Auxiliaries.h"

entt::entity Player::Create(entt::registry& registry, const std::string name, const char* textureFile)
{
	entt::entity entity = registry.create();
	registry.emplace<Player::Status>(entity, name, 100);
	registry.emplace<TransformComponent>(entity, Vector2{ 0,0 }, Direction::Right);

	std::vector<Frame> playerTexture;
	generateFrames(playerTexture, 0, 0, 2 * TILE_SIZE);

	registry.emplace<TextureComponent>(entity, LoadTexture(textureFile), RenderLayer::Player, playerTexture);
	return entity;
}

void Player::Remove(entt::registry& registry, entt::entity& entity)
{
	registry.destroy(entity);
}

void Player::Update(entt::registry& registry)
{
	auto playerView = registry.view< TransformComponent, Status>();
	for (auto [player, transform, status] : playerView.each()) {
		if (status.HP <= 0) {
			registry.destroy(player);
		}

		//TODO: Add player's update
	}
}

void Player::HandleInput(entt::registry& registry)
{
	auto playerView = registry.view< TransformComponent, Status>();
	for (auto [player, transform, status] : playerView.each()) {
		//TODO: Add player input
		
	}
}