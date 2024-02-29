#pragma once
#include <entt\entt.hpp>

struct PlayerStatus {
	const std::string name;
	int HP;
};

struct Player {
	static entt::entity CreatePlayer(entt::registry& registry, const std::string name);
	static void RemovePlayer(entt::registry& registry, entt::entity& entity);
};