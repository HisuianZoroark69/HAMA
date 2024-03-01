#pragma once

#include "Game.h"
#include <raylib.h>

/**
* MOVE_SPEED = FPS / Tiles_per_second
*/
#define MOVE_SPEED FPS/2
#define TILE_SIZE 64

/**
 * @brief Directions
 */
enum Direction {Right, FrontRight, Front, FrontLeft, Left, BackLeft, Back, BackRight};

/**
 * @brief The transform component
 */
struct TransformComponent{
	Vector2 position;
	Direction orientation;
};

/**
 * @brief The texture component
 */
struct TextureComponent {
	Texture2D texture;
	int drawRectY;
	int currentFrame;
};