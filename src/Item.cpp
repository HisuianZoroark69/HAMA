#include "Item.h"
void ActivateItem(ItemID name, PlayerStatus& stat, entt::registry& reg) {
	AllItems.at(name)(stat, reg);
}

std::vector<std::pair<Vector2, ItemID>> GenerateItems(pcg32& rng, DungeonGrid dg, int itemPlaceChance) {
	std::vector<std::pair<Vector2, ItemID>> ret;
	for (int y = 0; y < dg.size(); y++) {
		for (int x = 0; x < dg[y].size(); x++) {
			int cell = dg[y][x];
			if (cell == C_ROOM && GetRandomNumber(rng, 0, 100) < itemPlaceChance) {
				auto it = AllItems.begin();
				std::advance(it, GetRandomNumber(rng, 0, AllItems.size() - 1));
				ret.push_back({ Vector2{(float)x, (float)y}, it->first });
			}
		}
	}
	return ret;
}