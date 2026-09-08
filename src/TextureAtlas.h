#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>

namespace Tenshi {

struct AtlasRegion {
    SDL_Texture* texture;

    SDL_FRect region;

    int width;
    int height;
};

class TextureAtlas {
public:

    SDL_Texture* texture;

    std::unordered_map<std::string, AtlasRegion> regions;

    AtlasRegion* get(const std::string& name) {
        return &regions[name];
    }
};

} // namespace Tenshi