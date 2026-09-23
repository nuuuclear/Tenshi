#pragma once

#include "AssetRegistry.h"

#include "Tenshi/Graphics/Sprite.h"
#include "Tenshi/Assets/Font.h"
#include "Tenshi/Audio/Sound.h"

namespace Tenshi {

class AssetManager {
public:
    AssetRegistry<Sprite> sprite;
    AssetRegistry<Font>   font;
    AssetRegistry<Sound>  sound;
};

}