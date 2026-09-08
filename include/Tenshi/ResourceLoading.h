#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>

#include "AssetManager.h"

namespace Tenshi {

bool MakeSprite(
	SDL_Renderer* renderer, AssetManager& assets, 
	std::string path, std::string handle, 
	int width, int height
);

bool MakeFont(
	AssetManager& assets, 
	std::string path, std::string key, 
	float ptsize
);

} // namespace Tenshi