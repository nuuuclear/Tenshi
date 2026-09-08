#include "Tenshi/SpriteBatch.h"

namespace Tenshi {

SpriteBatch::SpriteBatch(SDL_Renderer* r) {
    renderer = r;
}

void SpriteBatch::begin() {
    commands.clear();
}

void SpriteBatch::draw(
    SDL_Texture* texture,
    const SDL_FRect& src,
    const SDL_FRect& dst,
    float rotation,
    SDL_FPoint origin,
    SDL_FlipMode flip
) {
    SpriteCommand cmd{};

    cmd.texture = texture;
    cmd.src = src;
    cmd.dst = dst;
    cmd.rotation = rotation;
    cmd.origin = origin;
    cmd.flip = flip;

    commands.push_back(cmd);
}

void SpriteBatch::flush() {
    std::sort(commands.begin(), commands.end(),
        [](const SpriteCommand& a, const SpriteCommand& b) {
            if (a.layer != b.layer)
                return a.layer < b.layer;

            return a.texture < b.texture;
        });

    for (const SpriteCommand& cmd : commands) {
        SDL_RenderTextureRotated(
            renderer,
            cmd.texture,
            &cmd.src,
            &cmd.dst,
            cmd.rotation,
            &cmd.origin,
            cmd.flip
        );
    }
}

} // namespace Tenshi