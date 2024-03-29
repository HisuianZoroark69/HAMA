#include "Player.h"
#include "TextureLoader.h"


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

void Player::UpdateCameraPosition(const Vector2& position, entt::registry& registry) {
	Camera2D camera = registry.ctx().get<Camera2D>();
	Vector2 currScrPos = GetWorldToScreen2D(position, camera);
	float scrHeight = GetScreenHeight(), scrWidth = GetScreenWidth();
	//camera.target = transform.position;
	if (currScrPos.x < TILE_SIZE * (CAMERA_BORDER_SIZE)) camera.target.x -= TILE_SIZE * (CAMERA_BORDER_SIZE)-currScrPos.x;
	if (currScrPos.x > SCREEN_SIZE - TILE_SIZE * (CAMERA_BORDER_SIZE + 1)) camera.target.x += currScrPos.x - (SCREEN_SIZE - TILE_SIZE * (CAMERA_BORDER_SIZE + 1));
	if (currScrPos.y < TILE_SIZE * (CAMERA_BORDER_SIZE)) camera.target.y -= TILE_SIZE * (CAMERA_BORDER_SIZE)-currScrPos.y;
	if (currScrPos.y > SCREEN_SIZE - TILE_SIZE * (CAMERA_BORDER_SIZE + 1)) camera.target.y += currScrPos.y - (SCREEN_SIZE - TILE_SIZE * (CAMERA_BORDER_SIZE + 1));
	registry.ctx().insert_or_assign(camera);
}

void Player::Update(entt::registry& registry)
{
	auto playerView = registry.view< TransformComponent, TextureComponent, Status>();

	for (auto [player, transform, texture, status] : playerView.each()) {
		if (status.HP <= 0) {
			registry.destroy(player);
		}

		//Update camera position
		UpdateCameraPosition(transform.position, registry);

		transform.orientation = status.movingDirection;
		ClampMovementInTile(transform.position, status.movingDirection);
	}
}

void Player::HandleInputDirection(Vector2& movingDirection) {
	if (IsKeyDown(KEY_A)) {
		movingDirection.x = -1;
	}
	else if (IsKeyDown(KEY_D)) {
		movingDirection.x = 1;
	}
	if (IsKeyDown(KEY_W)) {
		movingDirection.y = 1;
	}
	else if (IsKeyDown(KEY_S)) {
		movingDirection.y = -1;
	}
}

void Player::HandleMovementInDungeon(const DungeonGrid& dg, Vector2& position, Vector2& movingDirection) {
	Vector2 posCur = Vector2Scale(position, 1.f / TILE_SIZE);
	Vector2 posAfter = { posCur.x + movingDirection.x, posCur.y - movingDirection.y };
	if (movingDirection.x != 0 && movingDirection.y != 0) {
		if (dg[posAfter.y][posAfter.x] == C_WALL) {
			movingDirection = Vector2Zero();
			posAfter = posCur;
		}
	}
	if (movingDirection.x != 0) {
		if (dg[position.y / TILE_SIZE][posAfter.x] == C_WALL) {
			movingDirection.x = 0;
		}
	}
	if (movingDirection.y != 0) {
		if (dg[posAfter.y][position.x / TILE_SIZE] == C_WALL) {
			movingDirection.y = 0;
		}
	}
}

void Player::HandleInput(entt::registry& registry)
{
	auto playerView = registry.view<TransformComponent, TextureComponent, Status>();
	for (auto [player, transform, texture, status] : playerView.each()) {
		if (status.movingDirection.x == 0 && status.movingDirection.y == 0) {
			ChangeTexture(texture, TextureLoader::GetTexture(TEXTURE_IDLE));
			HandleInputDirection(status.movingDirection);
			const auto dg = registry.ctx().get<const DungeonGrid>();
			HandleMovementInDungeon(dg, transform.position, status.movingDirection);
		}
		else {
			ChangeTexture(texture, TextureLoader::GetTexture(TEXTURE_RUNNING));
		}
	}
}
