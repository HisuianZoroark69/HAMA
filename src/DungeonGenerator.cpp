#include "DungeonGenerator.h"

#include <random>
#include <stack>
#define ROOM_SPLIT_DEVIATION 3

void printDungeon(const DungeonGrid& dungeon){
    for (int i = 0; i < dungeon.size(); i++) {
        for (int j = 0; j < dungeon[i].size(); j++) {
            if (dungeon[i][j] == C_EMPTY || dungeon[i][j] == C_WALL)
                std::cout << "  ";
            else std::cout << dungeon[i][j] << " ";
            //std::cout << dungeon[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int DungeonGenerator::GetRandomNumber(pcg32& rng, int min, int max, bool normal) {
    if (max == min) return min;

    if(!normal)
        return min + rng(max - min + 1);

    int mean = (max + min) / 2;
    std::normal_distribution<double> dist(mean, sqrt(max - mean));
    int rand = dist(rng);

    if (rand >= max) rand = max;
    else if (rand <= min) rand = min;
    return rand;
}

int DungeonGenerator::GetRandomOddNumber(pcg32& rng, int min, int max, bool normal) {
    return GetRandomNumber(rng, min / 2, max / 2, normal) * 2 + 1;
}

int DungeonGenerator::GetRandomEvenNumber(pcg32& rng, int min, int max, bool normal) {
    return GetRandomNumber(rng, min / 2, max / 2, normal) * 2;
}

void DungeonGenerator::MarkBorder(DungeonGrid& dungeon) {
    const int width = dungeon[0].size();
    const int height = dungeon.size();

    //Generate border walls
    dungeon[0] = *dungeon.rbegin() = std::vector<int>(width, C_WALL);
    for (int x = 1; x < height - 1; x++)
        dungeon[x][0] = dungeon[x][width - 1] = C_WALL;
}

void DungeonGenerator::SplitRoom(pcg32& rng, std::vector<Room>& rooms, int& roomLeft, int roomMinWidth, int roomMinHeight) {
    std::vector<Room> result;
    while (!rooms.empty() && --roomLeft > 0) {
        Room currRoom = rooms.back();
        bool isVertical = rng() % 2;
        
        //Split room into 2 parts
        if (isVertical) {
            int col = GetRandomOddNumber(rng, currRoom.width / 2 - ROOM_SPLIT_DEVIATION, currRoom.width / 2 + ROOM_SPLIT_DEVIATION, true);
            result.push_back({ currRoom.x, currRoom.y, col, currRoom.height });
            result.push_back({ currRoom.x + col - 1, currRoom.y, currRoom.width - col + 1, currRoom.height });
        }
        else {
            int row = GetRandomOddNumber(rng, currRoom.height / 2 - ROOM_SPLIT_DEVIATION, currRoom.height / 2 + ROOM_SPLIT_DEVIATION, true);
            result.push_back({ currRoom.x, currRoom.y, currRoom.width, row});
            result.push_back({ currRoom.x, currRoom.y + row - 1, currRoom.width, currRoom.height - row + 1});
        }
        rooms.pop_back();
    }
    //Add remaining rooms
    while (!rooms.empty()) {
        result.push_back(rooms.back());
        rooms.pop_back();
    }
    //rooms = result;
    //Filter rooms
    while (!result.empty()) {
        Room currRoom = result.back();  result.pop_back();
        if (currRoom.width <= roomMinWidth || currRoom.height <= roomMinHeight) {
            continue;
        }
        rooms.push_back(currRoom);      
    }
}

void DungeonGenerator::CreateRoom(pcg32& rng, std::vector<Room>& rooms, int roomWidth, int roomHeight) {
    for (int i = 0; i < rooms.size(); i++) {
        Room room = rooms[i];

        rooms[i].x = GetRandomOddNumber(rng, room.x + 1, (room.x + room.width - 2 - roomWidth), true);
        rooms[i].y = GetRandomOddNumber(rng, room.y + 1, (room.y + room.height - 2 - roomHeight), true);
        rooms[i].width = GetRandomOddNumber(rng, roomWidth, room.width + room.x - rooms[i].x - 1);
        rooms[i].height = GetRandomOddNumber(rng, roomHeight, room.height + room.y - rooms[i].y - 1);
    }
}

void DungeonGenerator::MarkRooms(const std::vector<Room>& rooms, DungeonGrid& dungeon) {
    for (const Room& room : rooms) {
        // Mark the entire room as part of the dungeon
        for (int y = room.y; y < room.y + room.height; y++) {
            for (int x = room.x; x < room.x + room.width; x++) {
                dungeon[y][x] = C_ROOM;
            }
        }

        // Mark room border as walls
        for (int x = room.x - 1; x < room.x + room.width + 1; x++) {
            dungeon[room.y - 1][x] = C_WALL;
            dungeon[room.y + room.height][x] = C_WALL;
        }
        for (int y = room.y - 1; y < room.y + room.height + 1; y++) {
            dungeon[y][room.x - 1] = C_WALL;
            dungeon[y][room.x + room.width] = C_WALL;
        }
    }
}

bool DungeonGenerator::CheckInBoundary(int x, int y, int width, int height) {
    return !(x < 1 || x > width - 2 || y < 1 || y > height - 2);
}

bool DungeonGenerator::CheckCorridorsDone(const DungeonGrid& dungeon) {
    for (int y = 1; y < dungeon.size(); y += 2) {
        for (int x = 1; x < dungeon[y].size(); x += 2) {
            if (dungeon[y][x] == C_EMPTY)
                return false;
        }
    }
    return true;
}

bool DungeonGenerator::CheckCorridorsEnd(const int& x, const int& y, const DungeonGrid& dungeon, std::pair<int, int>& currDir) {
    const std::vector<std::pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
    int count = 0;
    for (const auto& dir : directions) {
        int nx = x + dir.first;
        int ny = y + dir.second;
        if (dungeon[ny][nx] == C_CORR) {
            count++;
            currDir = { x - dir.first, y - dir.second };
        }
        else if (dungeon[ny][nx] == C_ROOM) {
            count++;
        }
    }
    return count > 1;
}

void DungeonGenerator::GenerateCorridors(DungeonGrid& dungeon, pcg32& rng, int startX, int startY) {
    const int width = dungeon[0].size();
    const int height = dungeon.size();

    std::stack<std::pair<int, int>> stack;
    stack.push({ startX, startY });

    while (!stack.empty()) {
        int x = stack.top().first;
        int y = stack.top().second;
        stack.pop();

        // Check if the current position is out of bounds or already visited
        if (!CheckInBoundary(x, y, width, height) || dungeon[y][x] != C_EMPTY) {
            continue;
        }

        // Mark the current position as part of the corridor
        dungeon[y][x] = C_CORR;

        std::vector<std::pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

        // Push all directions onto the stack in odd columns or rows
        if (x % 2 == 1 && y % 2 == 1) {
            std::shuffle(directions.begin(), directions.end(), rng);
            for (const auto& dir : directions) {
                int nx = x + dir.first;
                int ny = y + dir.second;

                stack.push({ nx, ny });
            }
        }
        else {
            // Continue in the same direction in even columns or rows
            std::pair<int, int> currDir;
            
            if (CheckCorridorsEnd(x, y, dungeon, currDir)) {
                dungeon[y][x] = C_WALL;
            }
            else {
                stack.push(currDir);
            }
        }
    }
}

DungeonGrid DungeonGenerator::Generate(const DungeonGenerateData& data)
{
    //# Variables
    pcg32 rng(std::hash<std::string>{}(data.seed));
    std::vector<Room> rooms;
    DungeonGrid dungeon(data.height, std::vector<int>(data.width, C_EMPTY));

    MarkBorder(dungeon);

    //# Generate rooms using bsd
    //## Initialize room
    Room baseRec = {.x = 0, .y = 0, .width = data.width, .height = data.height};
    rooms.push_back(baseRec);
    int roomLeft = data.roomAmount;
    while (roomLeft > 0)
        SplitRoom(rng, rooms, roomLeft, data.roomMinWidth, data.roomMinHeight);
    CreateRoom(rng, rooms, data.roomMinWidth, data.roomMinHeight);

    MarkRooms(rooms, dungeon);

    //# Generate corridors
    while (!CheckCorridorsDone(dungeon)) {
        int start_x = GetRandomOddNumber(rng, 1, data.width - 2);
        int start_y = GetRandomOddNumber(rng, 1, data.height - 2);
        while (dungeon[start_y][start_x] != C_EMPTY) {
            start_x = GetRandomOddNumber(rng, 1, data.width - 2);
            start_y = GetRandomOddNumber(rng, 1, data.height - 2);
        }
        GenerateCorridors(dungeon, rng, start_x, start_y);
    }

    //# Generate doorways

    printDungeon(dungeon);

    return dungeon;
}
