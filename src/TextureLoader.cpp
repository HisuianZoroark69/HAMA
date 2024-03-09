#include "Game.h"
#include <unordered_map>
#include <fstream>
#include <nlohmann\json.hpp>
#include <raylib.h>

#include "TextureLoader.h"


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Rectangle, x, y, width, height)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextureJsonData, name, file, layer, frames)

void TextureLoader::LoadTextureFromJson(const std::string jsonFile)
{
	std::ifstream f(jsonFile);
	nlohmann::json textureData = nlohmann::json::parse(f)["textures"];

	for (auto texture : textureData) {
		auto textureData = texture.get<TextureJsonData>();

		if (baseTextures.find(textureData.file) == baseTextures.end()) {
			baseTextures[textureData.file] = LoadTexture(textureData.file.c_str());
		}

		textures[textureData.name] = { baseTextures[textureData.file], textureData.layer, textureData.frames };

		TraceLog(LOG_INFO, ("Loaded texture " + textureData.name).c_str());
	}


	f.close();
}

void TextureLoader::DeleteTextures()
{
	for (auto& [name, texture] : baseTextures) {
		UnloadTexture(texture);
	}
}

TextureComponent TextureLoader::GetTexture(std::string name)
{
	return textures[name];
}



