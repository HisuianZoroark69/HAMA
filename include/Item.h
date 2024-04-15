#pragma once
#include <string>
#include <map>
#include <vector>

#include <raylib.h>
#include <entt\entt.hpp>
#include "Player.h"
typedef std::string ItemID;

const std::map<ItemID, void(*)(PlayerStatus&, entt::registry&)> AllItems = {
	{"Apple", [](PlayerStatus& stat, entt::registry& reg) {stat.HP += 10; }}
};

void ActivateItem(ItemID name, PlayerStatus& stat, entt::registry& reg);
std::vector<std::pair<Vector2, ItemID>> GenerateItems(pcg32& rng, DungeonGrid dg, int itemPlaceChance);