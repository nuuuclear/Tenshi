#include "Tenshi/ResourceLoading.h"

#include <memory>

#include "Tenshi/Log.h"
#include "Tenshi/Sprite.h"
#include "Tenshi/Font.h"

namespace Tenshi {

bool MakeSprite(
	SDL_Renderer* renderer, AssetManager& assets,
	std::string path, 
	std::string key, 
	int width, int height
) {
	SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
	if (texture == NULL) {
		LogDebug(
			LogCategory::Resource,
			"Sprite could not be loaded: {}",
			SDL_GetError()
		);

		return false;
	}

	auto sprite = std::make_shared<Sprite>(texture, width, height);
	assets.sprite.Add(key, sprite);

	return true;
}

bool MakeFont(
	AssetManager& assets,
	std::string path, 
	std::string key, 
	float ptsize
) {
	SDL_IOStream* file = SDL_IOFromFile(path.c_str(), "r");
	if (file == NULL) {
		LogDebug(
			LogCategory::Resource,
			"Failed to open font file: {}",
			SDL_GetError()
		);

		return false;
	}

	TTF_Font* fontdata = TTF_OpenFontIO(file, true, ptsize);

	if (fontdata == NULL) {
		LogDebug(
			LogCategory::Application,
			"Failed to open font: {}",
			SDL_GetError()
		);

		return false;
	}

	auto font = std::make_shared<Font>();
	font.get()->Load_FromTTF(fontdata);
	assets.font.Add(key, font);

	return true;
}

} // namespace Tenshi