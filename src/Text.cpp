#include "Tenshi/Text.h"

#include "Tenshi/Font.h"
#include "Tenshi/Log.h"

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
	if (renderer == nullptr)
        return;

	if (dirty)
		RebuildTexture();

	if (texture != nullptr)
		SDL_RenderTexture(renderer, texture, NULL, &rect);
}

void TextRenderer::RebuildTexture() {
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
	
	if (texture == nullptr) {
		LogError(
			LogCategory::Renderer,
			"Could not create text texture: ",
			SDL_GetError()
		);
		
		SDL_DestroySurface(tsurf);
		return;
	}

	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

	rect.x = x;
	rect.y = y;
	rect.w = (float)tsurf->w;
	rect.h = (float)tsurf->h;

	SDL_DestroySurface(tsurf);

	dirty = false;
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
    if (font == f)
        return;

    font = f;
    dirty = true;
}

void TextRenderer::SetRenderer(SDL_Renderer* r) {
    if (renderer == r)
        return;

    renderer = r;
    dirty = true;

    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void TextRenderer::ClearText() {
 	text.clear();

    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    rect.w = 0.0f;
    rect.h = 0.0f;

    dirty = false;
}

std::string TextRenderer::GetText() {
	return text;
}

std::unique_ptr<TextRenderer> MakeTextRenderer(SDL_Renderer* renderer, Font* font) {
	auto new_TextRenderer = std::make_unique<TextRenderer>();

	new_TextRenderer->SetRenderer(renderer);
	new_TextRenderer->SetFont(font);

	return new_TextRenderer;
}

} // namespace Tenshi