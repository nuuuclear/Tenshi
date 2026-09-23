#pragma once

#include <cstdint>

namespace Tenshi {

constexpr uint16_t TMAP_VERSION = 1;

#pragma pack(push, 1)

struct TMapHeader {
    char magic[4];

    uint16_t version;

    uint16_t width;
    uint16_t height;

    uint16_t layerCount;

    uint16_t entryCount;
    uint16_t exitCount;

    uint32_t mapNameOffset;
    uint32_t mapNameLength;

    uint32_t tilesetNameOffset;
    uint32_t tilesetNameLength;

    uint32_t entriesOffset;
    uint32_t exitsOffset;

    uint32_t layersOffset;

    uint32_t collisionOffset;

    uint32_t stringsOffset;
    uint32_t stringsSize;
};

struct TMapTransition {
    uint32_t id;
    uint32_t tileIndex;
};

struct TMapLayer {
    int16_t depth;

    uint32_t tileDataOffset;
    uint32_t attributeDataOffset;

    uint32_t tileDataSize;
    uint32_t attributeDataSize;
};

#pragma pack(pop)

static_assert(sizeof(TMapHeader) == 56);
static_assert(sizeof(TMapTransition) == 8);
static_assert(sizeof(TMapLayer) == 18);

} // namespace Tenshi
