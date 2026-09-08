#include "Tenshi/Tilemap.h"

#include "Tenshi/Game.h"
#include "Tenshi/Tileset.h"
#include "Tenshi/Camera.h"
#include "Tenshi/SpriteBatch.h"
#include "Tenshi/Sprite.h"

namespace Tenshi {

Tilemap::Tilemap(Game* game, int w, int h) 
    : width(w), height(h)
{
	batch = new SpriteBatch(game->getRenderer());
}

Tilemap::~Tilemap() {
	batch = nullptr;
}

// Load tile map from .ena file.
	// TODO: Actually make that format, and it's editor!!!
bool Tilemap::LoadFromData(const char* dataPath) {
	// for now: fill with junk data

	for (size_t i = 0; i < height * width; i++) {
		map.push_back(rand() % 1024);
	}

    return true;
}

void Tilemap::Step(double deltaTime) {
	// step animation here, when added.
}

void Tilemap::SetTileset(Tileset *ts) {
    tileset = ts;
}

void Tilemap::Draw(const Camera& cam) {
    int tw = tileset->GetTileWidth();
    int th = tileset->GetTileHeight();

    int camX = cam.getViewX();
    int camY = cam.getViewY();

    // visible tile range
    int startX = camX / tw;
    int startY = camY / th;

    int endX = (camX + cam.viewW) / tw + 1;
    int endY = (camY + cam.viewH) / th + 1;

    Sprite* spr = tileset->GetSprite();

    batch->begin();

    for (int j = startY; j < endY; j++) {
        for (int i = startX; i < endX; i++) {

            // bounds check
            if (i < 0 || j < 0 || i >= width || j >= height)
                continue;

            uint16_t tileid = map[j * width + i];
            Tile tile = tileset->Index(tileid);

            SDL_FRect dst;
            dst.x = (i * tw) - camX;
            dst.y = (j * th) - camY;
            dst.w = tw;
            dst.h = th;

            SDL_FRect src;
            src.x = tile.x * tw;
            src.y = tile.y * th;
            src.w = tw;
            src.h = th;

            batch->draw(
                spr->GetTexture(),
                src,
                dst,
                0.0f,
                { 0, 0 },
                SDL_FLIP_NONE
            );
        }
    }

    batch->flush();
}

} // namespace Tenshi