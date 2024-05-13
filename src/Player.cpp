#include "Player.h"
#include "TextureLoader.h"


entt::entity Player::Create(entt::registry& registry)
{
	entt::entity entity = registry.create();
	registry.emplace<PlayerStatus>(entity, 100);
	registry.emplace<TransformComponent>(entity, Vector2{ 64 , 64 }, Direction{0,0});
	registry.emplace<TextureComponent>(entity, TextureLoader::GetTexture(TEXTURE_IDLE));
	return entity;
}

void Player::Remove(entt::registry& registry, entt::entity& entity)
{
	registry.destroy(entity);
}

void Player::ClampMovementInTile(Vector2& position, Vector2& movingDirection, bool isSprinting)
{
	//Force movement in tile
	/*if (isSprinting) {
		position.x = round(position.x / TILE_SIZE) * TILE_SIZE;
		position.y = round(position.y / TILE_SIZE) * TILE_SIZE;

		position.x += round(getSign(movingDirection.x) * TILE_SIZE);
		position.y -= round(getSign(movingDirection.y) * TILE_SIZE);

		movingDirection = Vector2Zero();
		return;
	}*/

	auto moveSpeed = MOVE_SPEED * (isSprinting ? 10 : 1);

	if (abs(movingDirection.x) < GetFrameTime() * moveSpeed) {
		position.x = round(position.x / TILE_SIZE) * TILE_SIZE;
		movingDirection.x = 0;
	}
	else {
		movingDirection.x -= getSign(movingDirection.x) * moveSpeed * GetFrameTime();
		position.x += round(getSign(movingDirection.x) * TILE_SIZE * moveSpeed * GetFrameTime());
	}

	if (abs(movingDirection.y) < GetFrameTime() * moveSpeed) {
		position.y = round(position.y / TILE_SIZE) * TILE_SIZE;
		movingDirection.y = 0;
	}
	else {
		movingDirection.y -= getSign(movingDirection.y) * moveSpeed * GetFrameTime();
		position.y -= round(getSign(movingDirection.y) * TILE_SIZE * moveSpeed * GetFrameTime());
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

void Player::Update(entt::entity& player, entt::registry& registry)
{
	if (!registry.valid(player)) return;
	auto [status, transform, texture] = registry.get<PlayerStatus, TransformComponent, TextureComponent>(player);
	if (status.hunger <= 0) {
		registry.destroy(player);
	}

	//Update camera position
	UpdateCameraPosition(transform.position, registry);

	transform.orientation = status.movingDirection;
	ClampMovementInTile(transform.position, status.movingDirection, status.isSprinting);

	//Pick up items
	auto &itemList = registry.ctx().get<std::vector<std::pair<Vector2, entt::entity>>>(ITEM_LIST_REG_NAME);
	auto it = std::find_if(itemList.begin(), itemList.end(), [transform](std::pair<Vector2, entt::entity> a) {
		return a.first.x == transform.position.x && a.first.y == transform.position.y;
	});
	if (it != itemList.end()) {
		status.apples++;
		registry.destroy(it->second);
		itemList.erase(it);
	}
}

bool Player::HandleInputDirection(Vector2& movingDirection) {
	bool ret = false;
	if (IsKeyDown(KEY_A)) {
		movingDirection.x = -1;
		ret = true;
	}
	else if (IsKeyDown(KEY_D)) {
		movingDirection.x = 1;
		ret = true;
	}
	if (IsKeyDown(KEY_W)) {
		movingDirection.y = 1;
		ret = true;
	}
	else if (IsKeyDown(KEY_S)) {
		movingDirection.y = -1;
		ret = true;
	}
	return ret;
}

bool Player::HandleMovementInDungeon(const DungeonGrid& dg, Vector2& position, Vector2& movingDirection) {
	bool ret = true;
	Vector2 posCur = Vector2Scale(position, 1.f / TILE_SIZE);
	Vector2 posAfter = { posCur.x + movingDirection.x, posCur.y - movingDirection.y };
	if (movingDirection.x != 0 && movingDirection.y != 0) {
		if (dg[posAfter.y][posAfter.x] == C_WALL) {
			ret = false;
			movingDirection = Vector2Zero();
			posAfter = posCur;
		}
	}
	if (movingDirection.x != 0) {
		if (dg[position.y / TILE_SIZE][posAfter.x] == C_WALL) {
			ret = false;
			movingDirection.x = 0;
		}
	}
	if (movingDirection.y != 0) {
		if (dg[posAfter.y][position.x / TILE_SIZE] == C_WALL) {
			ret = false;
			movingDirection.y = 0;
		}
	}
	return ret;
}


void Player::HandleInput(entt::registry& registry)
{
	auto playerView = registry.view<TransformComponent, TextureComponent, PlayerStatus>();
	for (auto [player, transform, texture, status] : playerView.each()) {
		if (IsKeyPressed(KEY_E) && status.apples > 0) {
			status.apples--;
			status.hunger += APPLE_RECOVERY_AMOUNT;
			status.hunger = Clamp(status.hunger, 0, 100);
		}

		status.isSprinting = IsKeyDown(KEY_LEFT_SHIFT);

		if (status.movingDirection.x == 0 && status.movingDirection.y == 0) {
			const auto dg = registry.ctx().get<const DungeonGrid>(DUNGEON_REG_NAME);

			if (HandleInputDirection(status.movingDirection) && HandleMovementInDungeon(dg, transform.position, status.movingDirection)){
				ChangeTexture(texture, TextureLoader::GetTexture(TEXTURE_RUNNING));
				status.hunger -= status.hungerConsumePerTile;
			}
			else {
				ChangeTexture(texture, TextureLoader::GetTexture(TEXTURE_IDLE));
			}
		}
		else {
			ChangeTexture(texture, TextureLoader::GetTexture(TEXTURE_RUNNING));
		}
	}
}
