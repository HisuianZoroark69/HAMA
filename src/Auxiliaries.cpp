#include "Auxiliaries.h"

void generateFrames(std::vector<Frame>& frames, float y, float xStart, float xEnd)
{
	for (float x = xStart; x <= xEnd; x += TILE_SIZE) {
		frames.push_back(Frame{ x, y, TILE_SIZE, TILE_SIZE });
	}
}
