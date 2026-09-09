#include "Tenshi/ResourceLoading.h"

#include <memory>

#include "Tenshi/Log.h"
#include "Tenshi/Sprite.h"
#include "Tenshi/Font.h"
#include "Tenshi/FileSystem.h"

#include "PakFile.h"

namespace Tenshi {

// Create a new sprite and put it into an AssetManager
bool MakeSprite(
    SDL_Renderer* renderer,
    AssetManager& assets,
    FileSystem& files,
    const std::string& path,
    const std::string& key,
    int width,
    int height
) {
    auto data = files.readAll(path);
    if (data.empty()) {
        LogWarning(
            Tenshi::LogCategory::Resource, 
            "Could not find image at: {}", 
            path
        );
        return false;
    }

    SDL_IOStream* io = SDL_IOFromConstMem(data.data(), data.size());
    if (!io) return false; 
    
    SDL_Texture* texture = IMG_LoadTexture_IO(renderer, io, true);
    if (!texture) {
        LogDebug(
            LogCategory::Resource,
            "Failed to load sprite '{}': {}",
            path,
            SDL_GetError()
        );

        return false;
    }

    auto sprite = std::make_unique<Sprite>(texture, width, height);
    assets.sprite.Add(key, std::move(sprite));

    return true;
}

// Create a new font and put it into an assetManager
bool MakeFont(
	AssetManager& assets, 
	FileSystem& files,
	const std::string path, 
	const std::string key, 
	float ptsize
) {
	auto data = files.readAll(path);
    if (data.empty()) return false;

    SDL_IOStream* io = SDL_IOFromConstMem(data.data(), data.size());
	if (!io) return false;

	TTF_Font* fontdata = TTF_OpenFontIO(io, true, ptsize);
	if (fontdata == NULL) {
		LogDebug(
			LogCategory::Application,
			"Failed to open font: {}",
			SDL_GetError()
		);

		return false;
	}

	auto font = std::make_unique<Font>();
	font->Load_FromTTF(fontdata);

	assets.font.Add(key, std::move(font));

	return true;
}

} // namespace Tenshi