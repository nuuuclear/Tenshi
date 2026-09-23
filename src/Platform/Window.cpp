#include "Window.h"

#include "Tenshi/Core/Game.h"

namespace Tenshi {

SDL_Window* MakeWindow(GameConfig conf) {
    
    SDL_WindowFlags window_flags = 0;

    if (conf.windowResizable) window_flags |= SDL_WINDOW_RESIZABLE;
    
    SDL_Window* window = SDL_CreateWindow(
        conf.title.c_str(), 
        conf.windowWidth, 
        conf.windowHeight, 
        window_flags
    );

    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Game failed create window: %s", SDL_GetError());
        return nullptr;
    }

    return window;
}

} // namespace Tenshi