#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>

namespace Tenshi {

struct SpriteCommand {
    SDL_Texture* texture;

    SDL_FRect src;
    SDL_FRect dst;

    float rotation;

    SDL_FPoint origin;

    SDL_FlipMode flip;

    int layer;
};

struct BatchGroup {
    SDL_Texture* texture;

    std::vector<SpriteCommand> sprites;
};

class SpriteBatch {
public:

    SpriteBatch(SDL_Renderer* r);

    void begin();
    void draw(
        SDL_Texture* texture,
        const SDL_FRect& src,
        const SDL_FRect& dst,
        float rotation = 0.0f,
        SDL_FPoint origin = { 0,0 },
        SDL_FlipMode flip = SDL_FLIP_NONE
    );

    void flush();

private:
    SDL_Renderer* renderer;

    std::vector<SpriteCommand> commands;
};

} // namespace Tenshi