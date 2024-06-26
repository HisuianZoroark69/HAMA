#pragma once

#include "Auxiliaries.h"
#include <unordered_map>

struct Frame {
	Rectangle rect;
	int duration = 1;
};

struct TextureJsonData {
	std::string name;
	std::string file;
	RenderLayer layer = ENUM_END;
	std::vector<Frame> frames;
};

class TextureLoader {
public:
	static void LoadTextureFromJson(const std::string jsonFile);
	static void DeleteTextures();
	
	static TextureComponent GetTexture(std::string name);

private:
	inline static std::unordered_map<std::string, TextureComponent> textures;
	inline static std::unordered_map<std::string, Texture2D> baseTextures;
};