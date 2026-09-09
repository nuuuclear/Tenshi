#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>

#include "Tenshi/AssetManager.h"

namespace Tenshi {

class FileSystem;

bool MakeSprite(
    SDL_Renderer* renderer,
    AssetManager& assets,
    FileSystem& files,
    const std::string& path,
    const std::string& key,
    int width,
    int height
);

bool MakeFont(
	AssetManager& assets, 
	FileSystem& files,
	const std::string path, 
	const std::string key, 
	float ptsize
);

} // namespace Tenshi