#pragma once

#include <SDL3/SDL.h>

namespace Tenshi {

struct GameConfig;

// create a starndard window
SDL_Window* MakeWindow(GameConfig conf);

} // namespace Tenshi