#pragma once

#include "AssetRegistry.h"

#include "Tenshi/Sprite.h"
#include "Tenshi/Font.h"
#include "Tenshi/Sound.h"

namespace Tenshi {

class AssetManager {
public:
    AssetRegistry<Sprite> sprite;
    AssetRegistry<Font>   font;
    AssetRegistry<Sound>  sound;
};

}