#include "Text.h"

#include "Font.h"
#include "Log.h"

namespace Tenshi {

TextRenderer::TextRenderer() {
	x = 0;
	y = 0;

	colour = { 255, 255, 255, 255 };

	dirty = true;
}

TextRenderer::~TextRenderer() {
	if (texture != nullptr)
		SDL_DestroyTexture(texture);
}

void TextRenderer::Draw() {
	if (dirty)
		RebuildTexture();

	if (texture != nullptr)
		SDL_RenderTexture(renderer, texture, NULL, &rect);
}

void TextRenderer::RebuildTexture() {
	dirty = false;

	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	if (text.empty())
		return;

	SDL_Surface* tsurf = TTF_RenderText_Solid(font->Get(), text.c_str(), 0, colour);

	if (tsurf == NULL) {
		LogError(
			LogCategory::Renderer,
			"Could not render text to the surface: ",
			SDL_GetError()
		);

		return;
	}

	texture = SDL_CreateTextureFromSurface(renderer, tsurf);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

	rect.x = x;
	rect.y = y;
	rect.w = (float)tsurf->w;
	rect.h = (float)tsurf->h;

	SDL_DestroySurface(tsurf);
}

void TextRenderer::SubmitText(const char* t) {
	text = t;
	dirty = true;
}

void TextRenderer::SetPosition(float _x, float _y) {
	x = _x;
	y = _y;

	rect.x = x;
	rect.y = y;
}

void TextRenderer::SetColour(SDL_Color col) {
	colour = col;
	dirty = true;
}

void TextRenderer::SetFont(Font* f) {
	font = f;
	dirty = true;
}

void TextRenderer::SetRenderer(SDL_Renderer* r) {
	renderer = r;
}

void TextRenderer::ClearText() {

}

std::string TextRenderer::GetText() {
	return text;
}

TextRenderer* Angel_MakeTextRenderer(SDL_Renderer* r, Font* f) {
	TextRenderer* _rend = new TextRenderer();

	_rend->SetRenderer(r);
	_rend->SetFont(f);

	return _rend;
}

} // namespace Tenshi