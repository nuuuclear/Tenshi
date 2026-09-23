#pragma once

#include <cstdint>

namespace Tenshi {

using TileId = uint16_t;

constexpr TileId AIR_TILE = 0xFFFF;

enum class TileColliderType : uint8_t {
    NONE = 0,
    FULL,
    HALF,
    SLOPE_UP,
    SLOPE_DOWN,
    HALF_SLOPE_UP,
    HALF_SLOPE_DOWN
};

struct TilesetTile {
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t flags = 0;
    TileColliderType defaultCollider = TileColliderType::NONE;
};

} // namespace Tenshi
