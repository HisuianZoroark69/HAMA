#pragma once

#include <vector>
#include <string>
#include <PCG\pcg_random.hpp>

struct DungeonGenerateData {
	std::string seed;

	int width;
	int height;

	int roomAmount = 0;
};

/**
 * @brief 
 * @see https://www.tomstephensondeveloper.co.uk/post/creating-simple-procedural-dungeon-generation
 * @see https://donjon.bin.sh/code/dungeon/
 */
class DungeonGenerator {
public:
	std::vector<std::vector<int>> Generate(const DungeonGenerateData& data);
private:

	struct Room {
		int x, y;
		int width, height;
	};

	static int GetRandomNumber(pcg32& rng, int min, int max, bool normal = false);
	static int GetRandomOddNumber(pcg32& rng, int min, int max, bool normal = false);
	static int GetRandomEvenNumber(pcg32& rng, int min, int max, bool normal = false);
	static void SplitRoom(pcg32& rng, std::vector<Room>& rooms, int& roomLeft);
	static void CreateRoom(pcg32& rng, std::vector<Room>& rooms, int roomWidth, int roomHeight);
};