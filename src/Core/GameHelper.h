#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace Tenshi {
namespace INTERNAL {

// Initialize backends
bool Initialize() {
    SDL_SetHint(SDL_HINT_WINDOWS_ENABLE_MESSAGELOOP, "1");

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Video failed to init: %s", SDL_GetError());
        return false;
    }

    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Audio failed to init: %s", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF failed to init: %s", SDL_GetError());
        return false;
    }

    return true;
}

const char* getBasePath() {
    const char* basePath = SDL_GetBasePath();

    if (!basePath) {
        throw std::runtime_error("Could not determine application path");
    }

    return basePath;
}

void buildConfig(YamlDocument document, GameConfig& config) {
    const auto& yamlRoot = document.GetRoot();

    config.title
    =   yamlRoot["game"]["title"].get_value<std::string>();

    config.windowWidth
    =   yamlRoot["window"]["width"].get_value<int>();

    config.windowHeight
    =   yamlRoot["window"]["height"].get_value<int>();

    config.windowScale
    =   yamlRoot["window"]["scale"].get_value<int>();

    config.windowResizable
    =   yamlRoot["window"]["resizable"].get_value<bool>();
}

} // namespace INTERNAL
} // namespace Tenshi