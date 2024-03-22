#include "DungeonGenerator.h"

#include <random>
#define ROOM_SPLIT_DEVIATION 3
/**
 * @brief Generate a random number in range
 * @param rng 
 * @param min Minimum value
 * @param max Maximum value
 * @return A random number in range from min to max
 * @see https://www.pcg-random.org/posts/bounded-rands.html
 */
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

void DungeonGenerator::SplitRoom(pcg32& rng, std::vector<Room>& rooms, int& roomLeft) {
    std::vector<Room> result;
    while (!rooms.empty() && --roomLeft > 0) {
        Room currRoom = rooms.back();
        bool isVertical = rng(2);
        
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
        if ((int)currRoom.width < 5 || (int)currRoom.height < 5) {
            continue;
        }
        rooms.push_back(currRoom);      
    }
}

void DungeonGenerator::CreateRoom(pcg32& rng, std::vector<Room>& rooms, int roomWidth, int roomHeight) {
    for (int i = 0; i < rooms.size(); i++) {
        Room room = rooms[i];

        rooms[i].x = GetRandomOddNumber(rng, room.x + 1, (room.x + room.width - 6), true);
        rooms[i].y = GetRandomOddNumber(rng, room.y + 1, (room.y + room.height - 6), true);
        rooms[i].width = GetRandomOddNumber(rng, 3, room.width + room.x - rooms[i].x - 1);
        rooms[i].height = GetRandomOddNumber(rng, 3, room.height + room.y - rooms[i].y - 1);
    }
}

std::vector<std::vector<int>> DungeonGenerator::Generate(const DungeonGenerateData& data)
{
    //# Variables
    pcg32 rng(std::hash<std::string>{}(data.seed));
    std::vector<Room> rooms;

    //# Generate rooms using bsd
    //## Initialize room
    Room baseRec = {.x = 0, .y = 0, .width = data.width, .height = data.height};
    rooms.push_back(baseRec);
    int roomLeft = data.roomAmount;
    while (roomLeft > 0)
        SplitRoom(rng, rooms, roomLeft);
    CreateRoom(rng, rooms, data.width, data.height);


    //# Generate corridors

    
    //# Generate doorways


    return std::vector<std::vector<int>>();
}
