#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include <nlohmann\json.hpp>

/**
 * @brief Render layer enum
 * @var RenderLayers::ENUM_END Used to get the layer size for rendering
 */
enum RenderLayer { Background, Player, GUI, ENUM_END };
NLOHMANN_JSON_SERIALIZE_ENUM(RenderLayer, {
	{Background, "Background"},
	{Player, "Player"},
	{GUI, "GUI"},
})
/**
* MOVE_SPEED = Tiles_per_second
*/
#define MOVE_SPEED 3.f
#define TILE_SIZE 64

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

int getSign(float num);