#pragma once
#include <entt\entt.hpp>

struct Player {
	struct Status {
		const std::string name;
		int HP;
	};

	static entt::entity Create(entt::registry& registry, const std::string name);
	static void Remove(entt::registry& registry, entt::entity& entity);
	static void HandleInput(entt::registry& registry);
	static void Update(entt::registry& registry);
};