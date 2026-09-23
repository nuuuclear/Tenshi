#pragma once

#include <string>
#include <SDL3/SDL.h>

namespace Tenshi {

// display a critical error dialog and force the application to terminate
[[noreturn]] static void FatalError(const std::string& title, const std::string& message) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR, 
        title.c_str(), 
        message.c_str(), 
        nullptr
    );
    
    std::exit(EXIT_FAILURE);
}

} // namespace Tenshi