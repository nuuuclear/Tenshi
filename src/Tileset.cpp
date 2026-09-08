#include "Tenshi/Tileset.h"

#include <random>
#include <SDL3/SDL.h>

namespace Tenshi {

Tileset::Tileset() {
	tileWidth = 16;
	tileHeight = 16;
}
Tileset::~Tileset() {

}

// get sprite from the global asset registry
bool Tileset::SetSprite(Sprite* spr) {
	sprite = spr;
	return sprite != nullptr;
}

// Load tileset data from .ena file
bool Tileset::LoadFromData(const char* dataPath) {
	// for now: fill with junk data
	int length = sizeof(index) / sizeof(index[0]);

	for (int i = 0; i < length; i++) {
		index[i].x = rand() % 32;
		index[i].y = rand() % 32;
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

Tile Tileset::Index(uint16_t tileid) {
	return index[tileid];
}

} // namespace Tenshi