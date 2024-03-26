#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include <nlohmann\json.hpp>
#include <PCG\pcg_random.hpp>


/**
 * @brief Render layer enum
 * @var RenderLayers::ENUM_END Used to get the layer size for rendering
 */
enum RenderLayer { Background, Dungeon, Player, GUI, ENUM_END };
NLOHMANN_JSON_SERIALIZE_ENUM(RenderLayer, {
	{Background, "Background"},
	{Dungeon, "Dungeon"},
	{Player, "Player"},
	{GUI, "GUI"},
})
/**
* MOVE_SPEED = Tiles_per_second
*/
#define MOVE_SPEED 4.f
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
	std::string name;
	Texture2D texture; //The texture sheet
	RenderLayer targetLayer; //The layer to render to
	std::vector<Rectangle> frames;
	int currentFrame = 0;
};

int getSign(float num);
/**
	 * @brief Generate a random number in range
	 * @param rng
	 * @param min Minimum value
	 * @param max Maximum value
	 * @param normal Use normal distribution instead of uniform distribution
	 * @return A random number in range from min to max
	 * @see https://www.pcg-random.org/posts/bounded-rands.html
	 */
int GetRandomNumber(pcg32& rng, int min, int max, bool normal = false);

/**
 * @brief Generate a random odd number in range
 * @param rng
 * @param min Minimum value
 * @param max Maximum value
 * @param normal Use normal distribution instead of uniform distribution
 * @return A random number in range from min to max
 * @see https://www.pcg-random.org/posts/bounded-rands.html
 */
int GetRandomOddNumber(pcg32& rng, int min, int max, bool normal = false);

/**
 * @brief Generate a random even number in range
 * @param rng
 * @param min Minimum value
 * @param max Maximum value
 * @param normal Use normal distribution instead of uniform distribution
 * @return A random number in range from min to max
 * @see https://www.pcg-random.org/posts/bounded-rands.html
 */
int GetRandomEvenNumber(pcg32& rng, int min, int max, bool normal = false);

const int GetRandomSeed();
