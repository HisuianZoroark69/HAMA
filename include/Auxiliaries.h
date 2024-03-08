#pragma once

#include "Game.h"
#include <raylib.h>
#include <string>

/**
* MOVE_SPEED = Tiles_per_second
*/
#define MOVE_SPEED 3.f
#define TILE_SIZE 64

using Frame = Rectangle;

/**
 * @brief Direction -> vector2
 * x: negative for left, positive for right, 0 for neutral
 * y: negative for down, positive for up, 0 for neutral
 */
using Direction = Vector2;

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
	Texture2D texture; //The texture sheet
	RenderLayer targetLayer; //The layer to render to
	std::vector<Rectangle> frames;
	int currentFrame = 0;
};

void generateFrames(std::vector<Frame>& frames, float y, float xStart, float xEnd);
int getSign(float num);