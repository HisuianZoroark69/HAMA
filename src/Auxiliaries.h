#pragma once

#include "Game.h"
#include <raylib.h>

/**
* MOVE_SPEED = FPS / Tiles_per_second
*/
#define MOVE_SPEED FPS/2

enum Direction {Right, FrontRight, Front, FrontLeft, Left, BackLeft, Back, BackRight};

struct TransformComponent{
	Vector2 position;
	Direction orientation;
};