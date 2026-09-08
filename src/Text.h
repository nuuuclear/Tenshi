#pragma once

#include <SDL3/SDL.h>

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>

#include <string>

namespace Tenshi {

class Font;

class TextRenderer {
public:
	TextRenderer();
	~TextRenderer();

	void Draw();
	void RebuildTexture();
	void SubmitText(const char* t);

	void SetPosition(float x, float y);

	void SetColour(SDL_Color col);
	void SetFont(Font* f);
	void SetRenderer(SDL_Renderer* r);

	void ClearText();

	std::string GetText();
	SDL_Color GetColour();
private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	SDL_FRect rect;
	
	Font* font;
	SDL_Color colour;

	float x;
	float y;
	
	std::string text;

	bool dirty = true;
};

TextRenderer* Angel_MakeTextRenderer(SDL_Renderer* r, Font* f);

} // namespace Tenshi