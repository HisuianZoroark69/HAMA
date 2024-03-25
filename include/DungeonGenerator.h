#pragma once

#include <vector>
#include <string>
#include <PCG\pcg_random.hpp>

struct DungeonGenerateData {
	std::string seed;

	int width;
	int height;

	int roomAmount = 0;

	int roomMinWidth = 3;
	int roomMinHeight = 3;
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
	static void SplitRoom(pcg32& rng, std::vector<Room>& rooms, int& roomLeft, int roomMinWidth, int roomMinHeight);
	static void CreateRoom(pcg32& rng, std::vector<Room>& rooms, int roomMinWidth, int roomMinHeight);
	static bool CheckNotInBoundary(int x, int y, int width, int height);
	static bool CheckCorridorsDone(const std::vector<std::vector<int>>& dungeon);
	/**
	 * @brief Generate corridors at odd row and columns using stack dfs algorithm
	 * @param dungeon The dungeon grid
	 * @param rng Random number generator
	 * @param x Current x-coordinate
	 * @param y Current y-coordinate
	 */
	static void GenerateCorridors(std::vector<std::vector<int>>& dungeon, pcg32& rng, int x, int y);
};