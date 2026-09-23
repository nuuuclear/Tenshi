#pragma once

#include <SDL3/SDL.h>

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>

#include <string>
#include <memory>

namespace Tenshi {

class Font;

class TextRenderer {
public:
	TextRenderer();
	~TextRenderer();

	void Draw();
	void RebuildTexture();
	void SubmitText(const std::string& t);

	void SetPosition(float x, float y);

	void SetColour(SDL_Color col);
	void SetFont(Font* f);
	void SetRenderer(SDL_Renderer* r);

	void ClearText();

	std::string GetText();
	SDL_Color GetColour();
private:
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_Surface* text_surface = nullptr;

	Font* font = nullptr;
	
	SDL_Color colour;
	SDL_FRect rect;

	std::string text = "";
	
	float x;
	float y;

	bool dirty = true;
};

std::unique_ptr<TextRenderer> MakeTextRenderer(SDL_Renderer* renderer, Font* font);

} // namespace Tenshi