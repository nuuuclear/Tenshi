#include "Image.h"

#include "Tenshi/Log.h"

#include <vector>
#include <memory>
#include <iostream>
#include <SDL3/SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Tenshi {

struct StbiDeleter {
    void operator()(unsigned char* pixels) const {
        stbi_image_free(pixels);
    }
};

SDL_Texture* LoadTextureIO_STB(SDL_Renderer* renderer, SDL_IOStream* src, bool closeio) {
    if (!src) {
        LogWarning(LogCategory::Resource, "Attempt to load null image data");
        return nullptr;
    }

    struct IOCloser {
        SDL_IOStream* stream;
        bool should_close;
        ~IOCloser() { if (should_close && stream) SDL_CloseIO(stream); }
    } io_closer{src, closeio};

    Sint64 total_size = SDL_GetIOSize(src);
    if (total_size <= 0) {
        return nullptr;
    }

    std::vector<uint8_t> buffer(static_cast<size_t>(total_size));
    size_t bytes_read = SDL_ReadIO(src, buffer.data(), buffer.size());
    if (bytes_read == 0) {
        LogWarning(LogCategory::Resource, "Could not read image data: {}", SDL_GetError());
        return nullptr;
    }

    int width = 0;
    int height = 0;
    int channels = 0;
    std::unique_ptr<unsigned char, StbiDeleter> pixels(
        stbi_load_from_memory(
            buffer.data(), 
            static_cast<int>(bytes_read), 
            &width, 
            &height, 
            &channels, 
            STBI_rgb_alpha
        )
    );

    if (!pixels) {
        LogWarning(LogCategory::Resource, "Could not read image data");
        return nullptr;
    }

    SDL_Surface* surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, pixels.get(), width * 4);
    if (!surface) {
        LogWarning(LogCategory::Resource, "Could not create surface: {}", SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_DestroySurface(surface);

    if (!texture) {
        LogWarning(LogCategory::Resource, "Could not create texture: {}", SDL_GetError());
    }

    return texture;
}

} // namespace Tenshi