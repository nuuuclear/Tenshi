#pragma once

#include <string>

namespace Tenshi {

class AssetManager;

enum assetType {
	SR_SPRITE,
	SR_FONT,
};

// Load resouces from the resource list in the provided directory path
// returns true on success, false on failure.
bool Angel_ResourcesLoad(AssetManager& manager, std::string resource_dir);

// Scans a resource directory for a resource list
// returns true on success, false on failure.
bool Angel_ResourcesScanDirectory(std::string list_path);

// Load an asset from a file directly
// returns true on success, false on failure.
bool Angel_ResourcesLoadAsset(std::string asset_path, assetType asset_type);

} // namespace Tenshi