#pragma once
#include <raylib.h>
enum Direction {Right, Front, Left, Back};

struct TransformComponent{
	Vector2 position;
	Direction orientation;
};