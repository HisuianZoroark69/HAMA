#include "Player.h"

entt::entity Player::Create(entt::registry& registry, const std::string name, const char* textureFile)
{
	entt::entity entity = registry.create();
	registry.emplace<Player::Status>(entity, name, 100);
	registry.emplace<TransformComponent>(entity, Vector2{ 0,0 }, Direction{0,0});

	std::vector<Frame> playerTexture;
	generateFrames(playerTexture, 0, 0, 2 * TILE_SIZE);

	registry.emplace<TextureComponent>(entity, LoadTexture(textureFile), RenderLayer::Player, playerTexture);
	return entity;
}

void Player::Remove(entt::registry& registry, entt::entity& entity)
{
	registry.destroy(entity);
}

void Player::ClampMovementInTile(Vector2& position, Vector2& movingDirection)
{
	//Force movement in tile
	if (abs(movingDirection.x) < GetFrameTime() * MOVE_SPEED) {
		position.x = round(position.x / TILE_SIZE) * TILE_SIZE;
		movingDirection.x = 0;
	}
	else {
		movingDirection.x -= getSign(movingDirection.x) * MOVE_SPEED * GetFrameTime();
		position.x += round(getSign(movingDirection.x) * TILE_SIZE * MOVE_SPEED * GetFrameTime());
	}

	if (abs(movingDirection.y) < GetFrameTime() * MOVE_SPEED) {
		position.y = round(position.y / TILE_SIZE) * TILE_SIZE;
		movingDirection.y = 0;
	}
	else {
		movingDirection.y -= getSign(movingDirection.y) * MOVE_SPEED * GetFrameTime();
		position.y -= round(getSign(movingDirection.y) * TILE_SIZE * MOVE_SPEED * GetFrameTime());
	}
}

void Player::Update(entt::registry& registry)
{
	auto playerView = registry.view< TransformComponent, Status>();
	for (auto [player, transform, status] : playerView.each()) {
		if (status.HP <= 0) {
			registry.destroy(player);
		}

		//TODO: Add player's update
		transform.orientation = status.movingDirection;

		ClampMovementInTile(transform.position, status.movingDirection);
	}
}

void Player::HandleInput(entt::registry& registry)
{
	auto playerView = registry.view< TransformComponent, Status>();
	for (auto [player, transform, status] : playerView.each()) {
		//TODO: Add player input
		if (status.movingDirection.x == 0 && status.movingDirection.y == 0) {
			if(IsKeyDown(KEY_A)){
				status.movingDirection.x = -1;				
			}
			else if(IsKeyDown(KEY_D)){
				status.movingDirection.x = 1;
			}
			if (IsKeyDown(KEY_W)) {
				status.movingDirection.y = 1;
			}
			else if (IsKeyDown(KEY_S)) {
				status.movingDirection.y = -1;
			}
		}
	}
}