#include "DungeonGenerator.h"

#include <random>

int DungeonGenerator::GetRandomNumber(pcg32& rng, int min, int max) {
    std::uniform_int_distribution<> distribute(min, max);
    return distribute(rng);
}

void DungeonGenerator::SplitRoom(pcg32& rng, std::vector<Room>& rooms, int& roomLeft) {
    std::vector<Room> result;
    while (!rooms.empty() && --roomLeft > 0) {
        Room currRoom = rooms.back(); rooms.pop_back();
        //If room cannot be divided, skip
        bool isVertical;
        if (currRoom.width % 2 == 0 && currRoom.height % 2 == 0) {
            continue;
        }
        else if (currRoom.width % 2 == 0) {
            isVertical = false;
        }
        else if (currRoom.height % 2 == 0) {
            isVertical = true;
        }
        else
            isVertical = GetRandomNumber(rng, false, true);
        
        //Split room into 2 parts
        if (isVertical) {
            int col = GetRandomNumber(rng, 1, currRoom.width / 2) * 2 + 1;
            result.push_back({ currRoom.x, currRoom.y, col, currRoom.height });
            result.push_back({ currRoom.x + col, currRoom.y, currRoom.width - col, currRoom.height });
        }
        else {
            int row = GetRandomNumber(rng, 1, currRoom.height / 2) * 2 + 1;
            result.push_back({ currRoom.x, currRoom.y, currRoom.height, row });
            result.push_back({ currRoom.x, currRoom.y + row, currRoom.width, currRoom.height - row});
        }
    }
    //Add remaining rooms
    while (!rooms.empty()) {
        result.push_back(rooms.back());
        rooms.pop_back();
    }
    //Filter rooms
    while (!result.empty()) {
        Room currRoom = result.back();  result.pop_back();
        if ((int)currRoom.width % 2 == 0 || (int)currRoom.height % 2 == 0) {
            continue;
        }
        rooms.push_back(currRoom);      
    }
}

std::vector<std::vector<int>> DungeonGenerator::Generate(DungeonGenerateData data)
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

    //# Generate corridors
    
    //# Generate doorways

    return std::vector<std::vector<int>>();
}
