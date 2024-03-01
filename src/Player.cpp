#include "Player.h"
#include "Auxiliaries.h"

entt::entity Player::Create(entt::registry& registry, const std::string name)
{
	entt::entity entity = registry.create();
	registry.emplace<Player::Status>(entity, name, 100);
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
		if (status.HP <= 0) registry.destroy(player);
	}
}

void Player::HandleInput(entt::registry& registry)
{
	auto playerView = registry.view< TransformComponent, Status>();
	for (auto [player, transform, status] : playerView.each()) {
		
	}
}