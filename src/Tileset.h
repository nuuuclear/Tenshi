#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdint.h>
#include <string>

#include "Sprite.h"

namespace Tenshi {

enum tileCollider {
	// full tile
	TC_FULL,

	// half tile (TODO: add slopes for these too...)
	TC_HALF_TOP,
	TC_HALF_BOTTOM,
	TC_HALF_LEFT,
	TC_HALF_RIGHT,

	// standard sloped corners
	TC_CORNER_UL, 
	TC_CORNER_UR,
	TC_CORNER_BL,
	TC_CORNER_BR,

	// 2x1 slopes
	TC_VU_CORN_UL, // Vertical upper
	TC_VU_CORN_UR,
	TC_VU_CORN_BL,
	TC_VU_CORN_BR,
	TC_VL_CORN_UL, // Vertical lower
	TC_VL_CORN_UR,
	TC_VL_CORN_BL,
	TC_VL_CORN_BR,

	TC_HU_CORN_UL, // Horizontal upper
	TC_HU_CORN_UR,
	TC_HU_CORN_BL,
	TC_HU_CORN_BR,
	TC_HL_CORN_UL, // Horizontal lower
	TC_HL_CORN_UR,
	TC_HL_CORN_BL,
	TC_HL_CORN_BR,
};

struct Tile {
	uint8_t x, y;
	uint8_t flags;

	bool collision;
	uint8_t collider_type;
};

class Tileset {
public:
	Tileset();
	~Tileset();

	bool SetSprite(Sprite* spr);

	bool LoadFromData(const char* dataPath);

	uint8_t GetTileWidth() const;
	uint8_t GetTileHeight() const;

	Sprite* GetSprite();
	Tile Index(uint16_t tileid);
private:
	// 32x32 array of 16x16 tiles from a 512x512 image
	Tile index[32 * 32]; // TODO: make this resizable

	Sprite* sprite;
	uint8_t tileWidth, tileHeight;

	bool animated;
};

} // namespace Tenshi