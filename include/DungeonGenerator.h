#pragma once

#include <vector>
#include <string>
#include <PCG\pcg_random.hpp>

typedef std::vector<std::vector<int>> DungeonGrid;

const enum CELL { C_EMPTY, C_WALL, C_ROOM, C_CORR };

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
	DungeonGrid Generate(const DungeonGenerateData& data);
private:

	struct Room {
		int x, y;
		int width, height;
	};

	/**
	 * @brief Generate a random number in range
	 * @param rng
	 * @param min Minimum value
	 * @param max Maximum value
	 * @param normal Use normal distribution instead of uniform distribution
	 * @return A random number in range from min to max
	 * @see https://www.pcg-random.org/posts/bounded-rands.html
	 */
	int GetRandomNumber(pcg32& rng, int min, int max, bool normal = false);

	/**
	 * @brief Generate a random odd number in range
	 * @param rng
	 * @param min Minimum value
	 * @param max Maximum value
	 * @param normal Use normal distribution instead of uniform distribution
	 * @return A random number in range from min to max
	 * @see https://www.pcg-random.org/posts/bounded-rands.html
	 */
	int GetRandomOddNumber(pcg32& rng, int min, int max, bool normal = false);

	/**
	 * @brief Generate a random even number in range
	 * @param rng
	 * @param min Minimum value
	 * @param max Maximum value
	 * @param normal Use normal distribution instead of uniform distribution
	 * @return A random number in range from min to max
	 * @see https://www.pcg-random.org/posts/bounded-rands.html
	 */
	int GetRandomEvenNumber(pcg32& rng, int min, int max, bool normal = false);

	/**
	 * @brief Mark the border of the dungeon with walls
	 * @param dungeon The dungeon grid
	 */
	void MarkBorder(DungeonGrid& dungeon);

	/**
	 * @brief Split the dungeon into multiple parts using BSP. After splitting, this function will remove all splitted parts that
	 *        does not fit a room with the specified minimum size.
	 * @param rng Random number generator
	 * @param rooms List of rooms to split.
	 * @param roomLeft The number of rooms left. Will not split after this reached 0.
	 * @param roomMinWidth Minimum room width
	 * @param roomMinHeight Minimum room height
	 */
	void SplitRoom(pcg32& rng, std::vector<Room>& rooms, int& roomLeft, int roomMinWidth, int roomMinHeight);

	/**
	 * @brief Create rooms from BSP splits of the dungeon
	 * @param rng Random number generator
	 * @param rooms BSP splits
	 * @param roomMinWidth Minimum room width
	 * @param roomMinHeight Minimum room height
	 */
	void CreateRoom(pcg32& rng, std::vector<Room>& rooms, int roomMinWidth, int roomMinHeight);

	/**
	 * @brief Mark room cell onto dungeon grid
	 * @param rooms Room list
	 * @param dungeon The dungeon grid to mark on to
	 */
	void MarkRooms(const std::vector<Room>& rooms, DungeonGrid& dungeon);

	/**
	 * @brief Check if position in the dungeon boundaries
	 * @param x Current x position
	 * @param y Current y position
	 * @param width Dungeon's width
	 * @param height Dungeon's height
	 * @return true if position is not in boundary
	 */
	bool CheckInBoundary(int x, int y, int width, int height);

	/**
	 * @brief Check if the dungeon is filled with corridors
	 * @param dungeon The dungeon grid to check
	 * @return true if The dungeon is filled with corridors
	 */
	bool CheckCorridorsDone(const DungeonGrid& dungeon);

	/**
	 * @brief Check if current corridor position is a dead-end
	 * @param x 
	 * @param y 
	 * @param dungeon Current dungeon grid
	 * @param currDir Return the next corridor position
	 * @return true if the corridor is a dead-end
	 */
	bool CheckCorridorsEnd(const int& x, const int& y, const DungeonGrid& dungeon, std::pair<int, int>& currDir);

	/**
	 * @brief Generate corridors at odd row and columns using stack dfs algorithm
	 * @param dungeon The dungeon grid
	 * @param rng Random number generator
	 * @param x Current x-coordinate
	 * @param y Current y-coordinate
	 */
	void GenerateCorridors(DungeonGrid& dungeon, pcg32& rng, int x, int y);
};