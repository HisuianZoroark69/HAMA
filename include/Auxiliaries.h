#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include <nlohmann\json.hpp>
#include <PCG\pcg_random.hpp>
#include <set>
#include <entt/entt.hpp>
#include <random>

using entt::operator"" _hs;

/**
 * @brief Render layer enum
 * @var RenderLayers::ENUM_END Used to get the layer size for rendering
 */
enum RenderLayer {Background, Dungeon, Item, Player, GUI, Minimap, ENUM_END };
NLOHMANN_JSON_SERIALIZE_ENUM(RenderLayer, {
	{Background, "Background"},
	{Dungeon, "Dungeon"},
	{Player, "Player"},
	{GUI, "GUI"},
	{Minimap, "Minimap"},
	{Item, "Item"}
})
	
const std::set<int> GUILayers = { GUI };
/**
* MOVE_SPEED = Tiles_per_second
*/
constexpr auto MOVE_SPEED = 4.f;
constexpr auto TILE_SIZE = 64;
constexpr auto FPS = 60;
constexpr auto SCREEN_SIZE = 640;
constexpr auto CAMERA_BORDER_SIZE = 3;
constexpr auto MINIMAP_RADIUS = 8; //Tiles

constexpr auto DUNGEON_REG_NAME = "dungeon"_hs;
constexpr auto MINIMAP_REG_NAME = "minimap"_hs;
constexpr auto ITEM_LIST_REG_NAME = "itemList"_hs;

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

template <class T>
T GetRandomElement(pcg32& rng, const std::vector<T>& arr, bool normal = false)
{
	return arr[GetRandomNumber(rng, 0, arr.size() - 1, normal)];
	//return *(begin + GetRandomNumber(rng, 0, (end - begin, normal));
}

/**
 * @brief Check if position in the dungeon boundaries
 * @param x Current x position
 * @param y Current y position
 * @param width Dungeon's width
 * @param height Dungeon's height
 * @return true if position is not in boundary
 */
bool CheckInBoundary(int x, int y, uint64_t width, uint64_t height);

const int GetRandomSeed();
