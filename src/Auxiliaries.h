#pragma once

#include "Game.h"
#include <raylib.h>

/**
* MOVE_SPEED = FPS / Tiles_per_second
*/
#define MOVE_SPEED FPS/2
#define TILE_SIZE 64

enum Direction {Right, FrontRight, Front, FrontLeft, Left, BackLeft, Back, BackRight};

struct TransformComponent{
	Vector2 position;
	Direction orientation;
};
/// <summary>
/// Texture component
/// </summary>
/// texture: The texture file
/// drawRectY: The Y component of the draw frame
/// currentFrame: Current animation frame in drawRect
struct TextureComponent {
	Texture2D texture;
	int drawRectY;
	int currentFrame;
};