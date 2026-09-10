#pragma once

#include <SDL3/SDL.h>

namespace Tenshi {

struct GameConfig;

SDL_Window* MakeWindow(GameConfig conf);

} // namespace Tenshi