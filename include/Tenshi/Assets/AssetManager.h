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
public:
    std::string getYamlPath() {
        return yamlPath;
    }

    void setYamlPath(std::string path) {
        yamlPath = path;
    }
private: 
    std::string yamlPath = "";
};

} // namespace Tenshi