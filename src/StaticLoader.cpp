#include "StaticLoader.h"

#include "AssetManager.h"
#include "AssetRegistry.h"
#include "ResourceLoading.h"

namespace Tenshi {

bool ResourcesLoad(AssetManager& manager, std::string resource_dir) {

	return true;
}

bool ResourcesScanDirectory(std::string list_path) {

	return true;
}

bool ResourcesLoadAsset(std::string asset_path, assetType asset_type) {
	switch (asset_type) {
	case SR_SPRITE:
		// MakeSprite();
		break;
	case SR_FONT:

		break;
	}
	return true;
}

} // namespace Tenshi