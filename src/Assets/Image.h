#pragma once

#include <SDL3/SDL.h>

namespace Tenshi {

// SDL_Texture* LoadTexture_STB(SDL_Renderer* renderer, const char* filename);
SDL_Texture* LoadTextureIO_STB(SDL_Renderer* renderer, SDL_IOStream* src, bool closeio);

} // namespace Tenshi