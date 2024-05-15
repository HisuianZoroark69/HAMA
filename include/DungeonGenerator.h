#pragma once

#include <vector>
#include <string>
#include <PCG\pcg_random.hpp>

constexpr auto MINIMUM_DUNGEON_SIZE = 23;
constexpr auto MINIMUM_ROOM_AMOUNT = 4;

using DungeonGrid = std::vector<std::vector<int>>;

const enum CELL { 
	C_EMPTY, 
	C_WALL,
	C_WALL_INVIS,
	C_ROOM, 
	C_ROOM_INVIS,
	C_CORR,
	C_DOOR,
	C_STAIR
};

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
	DungeonGrid Generate(const DungeonGenerateData& data, std::pair<int, int>& spawnPosition);
	std::vector<std::pair<int, int>> GenerateApplesPosition(pcg32& rng, const DungeonGrid& dg, const float rate);
private:

	struct Room {
		int x, y;
		int width, height;
	};

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

	/**
	 * @brief Fill the empty cells in the dungeon with walls
	 * @param dungeon The dungeon grid
	 */
	void FillDungeon(DungeonGrid& dungeon);

	/**
	 * @brief Generate doorways
	 * @param rng Random number generator
	 * @param dungeon The dungeon grid
	 * @param rooms List of rooms
	 * @return List of doorway positions suitable to generate corridors
	 */
	std::vector<std::pair<int, int>> GenerateDoorways(pcg32& rng, DungeonGrid& dungeon, const std::vector<Room>& rooms);

	void GenerateSpawn(pcg32& rng, const std::vector<Room>& rooms, std::pair<int, int>& spawnPosition);

	std::vector<std::pair<int, int>> GetDeadends(const DungeonGrid& dg);

	void GenerateStair(pcg32& rng, DungeonGrid& dg);
};