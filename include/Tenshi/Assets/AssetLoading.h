#pragma once

#include "Tenshi/Files/FileSystem.h"
#include "Tenshi/Assets/AssetManager.h"

#include <string>

namespace Tenshi {

bool MakeAllAssetsYaml(SDL_Renderer* renderer, AssetManager& assets, FileSystem& files, AudioSystem& audio);
bool MakeSpriteYaml(SDL_Renderer* renderer, AssetManager& assets, FileSystem& files, const std::string& path);
bool MakeSoundYaml(AssetManager& assets, FileSystem& files, AudioSystem& audio, const std::string& path);
    
} // namespace Tenshi
