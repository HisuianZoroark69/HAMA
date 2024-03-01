#include "Player.h"
#include "Auxiliaries.h"

entt::entity Player::Create(entt::registry& registry, const std::string name, const char* textureFile)
{
	entt::entity entity = registry.create();
	registry.emplace<Player::Status>(entity, name, 100);
	registry.emplace<TransformComponent>(entity, Vector2{ 0,0 }, Direction::Right);
	registry.emplace<TextureComponent>(entity, LoadTexture(textureFile), 0, 0);
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
		if (status.HP-- <= 0) {
			registry.destroy(player);
		}
		transform.position.x++;
	}
}

void Player::HandleInput(entt::registry& registry)
{
	auto playerView = registry.view< TransformComponent, Status>();
	for (auto [player, transform, status] : playerView.each()) {
		
	}
}