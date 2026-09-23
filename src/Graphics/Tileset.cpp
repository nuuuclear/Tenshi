#include "Tenshi/Graphics/Tileset.h"

#include <cstdlib>

namespace Tenshi {

Tileset::Tileset() {
    tileWidth = 16;
    tileHeight = 16;
}

Tileset::~Tileset() = default;

bool Tileset::SetSprite(Sprite* spr) {
    sprite = spr;
    return sprite != nullptr;
}

bool Tileset::LoadFromData(const char* dataPath) {
    if (dataPath == nullptr)
        return false;

    // TODO: load from file the tileset data

    constexpr int tilesWide = 32;
    constexpr int tilesHigh = 32;

    tiles.clear();
    tiles.resize(tilesWide * tilesHigh);

    for (int y = 0; y < tilesHigh; ++y) {
        for (int x = 0; x < tilesWide; ++x) {
            const size_t index 
			=	static_cast<size_t>(y) 
			* 	tilesWide + x;

            tiles[index].x = static_cast<uint8_t>(x);
            tiles[index].y = static_cast<uint8_t>(y);
            tiles[index].flags = 0;
            tiles[index].defaultCollider 
			=	TileColliderType::NONE;
        }
    }

    return true;
}

uint8_t Tileset::GetTileWidth() const {
    return tileWidth;
}

uint8_t Tileset::GetTileHeight() const {
    return tileHeight;
}

Sprite* Tileset::GetSprite() {
    return sprite;
}

const TilesetTile* Tileset::GetTile(TileId tileid) const {
    if (tileid >= tiles.size())
        return nullptr;

    return &tiles[tileid];
}

size_t Tileset::GetTileCount() const {
    return tiles.size();
}

} // namespace Tenshi