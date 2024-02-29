#include "Player.h"
#include "Auxiliaries.h"

entt::entity Player::CreatePlayer(entt::registry& registry, const std::string name)
{
	entt::entity entity = registry.create();
	registry.emplace<PlayerStatus>(entity, name, 100);
	return entity;
}

void Player::RemovePlayer(entt::registry& registry, entt::entity& entity)
{
	registry.destroy(entity);
}
