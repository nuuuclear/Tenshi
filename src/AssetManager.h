#pragma once

#include "AssetRegistry.h"

#include "Sprite.h"
#include "Font.h"

namespace Tenshi {

class AssetManager {
public:
    AssetRegistry<Sprite> sprite;
    AssetRegistry<Font>   font;
};

}