#include "Player.h"
#include "TextureLoader.h"
#include <DungeonGenerator.h>

entt::entity Player::Create(entt::registry& registry)
{
	entt::entity entity = registry.create();
	registry.emplace<Player::Status>(entity, 100);
	registry.emplace<TransformComponent>(entity, Vector2{ 64 , 64 }, Direction{0,0});
	registry.emplace<TextureComponent>(entity, TextureLoader::GetTexture(TEXTURE_IDLE));
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

void Player::ChangeTexture(TextureComponent& oldTexture, TextureComponent newTexture)
{
	if (newTexture.name != oldTexture.name) oldTexture = newTexture;
}


void Player::Update(entt::registry& registry)
{
	auto playerView = registry.view< TransformComponent, TextureComponent, Status>();
	for (auto [player, transform, texture, status] : playerView.each()) {
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
	auto playerView = registry.view<TransformComponent, TextureComponent, Status>();
	for (auto [player, transform, texture, status] : playerView.each()) {
		//TODO: Add player input
		if (status.movingDirection.x == 0 && status.movingDirection.y == 0) {
			DungeonGrid dg = registry.ctx().get<DungeonGrid>();
			ChangeTexture(texture, TextureLoader::GetTexture(TEXTURE_IDLE));
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

			Vector2 posCur = Vector2Scale(transform.position, 1.f / TILE_SIZE);
			Vector2 posAfter = {posCur.x + status.movingDirection.x, posCur.y - status.movingDirection.y };
			if (status.movingDirection.x != 0 && status.movingDirection.y != 0) {
				if (dg[posAfter.y][posAfter.x] == C_WALL) {
					status.movingDirection = Vector2Zero();
					posAfter = posCur;
				}
			}
			if (status.movingDirection.x != 0) {
				if (dg[transform.position.y/TILE_SIZE][posAfter.x] == C_WALL) {
					status.movingDirection.x = 0;
				}
			}
			if (status.movingDirection.y != 0) {
				if (dg[posAfter.y][transform.position.x / TILE_SIZE] == C_WALL) {
					status.movingDirection.y = 0;
				}
			}
		}
		else {
			ChangeTexture(texture, TextureLoader::GetTexture(TEXTURE_RUNNING));
		}
	}
}
