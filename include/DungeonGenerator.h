#pragma once

#include <vector>
#include <string>
#include <PCG\pcg_random.hpp>

struct DungeonGenerateData {
	std::string seed;

	int width;
	int height;

	int roomAmount;
	int roomMaxWidth;
	int roomMaxHeight;
};

/**
 * @brief 
 * @see https://www.tomstephensondeveloper.co.uk/post/creating-simple-procedural-dungeon-generation
 * @see https://donjon.bin.sh/code/dungeon/
 */
class DungeonGenerator {
public:
	std::vector<std::vector<int>> Generate(DungeonGenerateData data);
private:

	struct Room {
		int x, y;
		int width, height;
	};

	static int GetRandomNumber(pcg32& rng, int min, int max);
	static void SplitRoom(pcg32& rng, std::vector<Room>& rooms, int& roomLeft);
};