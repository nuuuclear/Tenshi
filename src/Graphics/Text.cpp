#include "Tenshi/Graphics/Text.h"

#include "Tenshi/Assets/Font.h"
#include "Tenshi/Core/Log.h"

#include <cmath>

namespace Tenshi {

TextRenderer::TextRenderer() {
    x = 0.0f;
    y = 0.0f;
    
    rect = { 0.0f, 0.0f, 0.0f, 0.0f };
    colour = { 255, 255, 255, 255 };

    dirty = true;
}

TextRenderer::~TextRenderer() {
	if (texture != nullptr)
		SDL_DestroyTexture(texture);

	SDL_DestroySurface(text_surface);
}

void TextRenderer::Draw() {
	if (renderer == nullptr)
        return;

	if (dirty)
		RebuildTexture();

	if (texture != nullptr) {
        SDL_FRect srcRect = { 0.0f, 0.0f, rect.w, rect.h };
        SDL_RenderTexture(renderer, texture, &srcRect, &rect);
    }
}

void TextRenderer::RebuildTexture() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	if (text.empty()) {
		rect.w = 0.0f;
        rect.h = 0.0f;

		dirty = false;
		return;
	}

	if (font == nullptr || font->Get() == nullptr) {
        LogError(LogCategory::Renderer, "TextRenderer has a null font reference pointer");
        dirty = false;
        return;
    }

	text_surface = TTF_RenderText_Blended(font->Get(), text.c_str(), text.length(), colour);

	if (text_surface == NULL) {
		LogError(
			LogCategory::Renderer,
			"Could not render text to the surface: ",
			SDL_GetError()
		);
		return;
	}

	texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	
	if (texture == nullptr) {
		LogError(
			LogCategory::Renderer,
			"Could not create text texture: ",
			SDL_GetError()
		);
		
		SDL_DestroySurface(text_surface);
		text_surface = nullptr;
		return;
	}

	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

	rect.x = std::trunc(x);
	rect.y = std::trunc(y);
	rect.w = (float)text_surface->w;
	rect.h = (float)text_surface->h;

	SDL_DestroySurface(text_surface);
	text_surface = nullptr;

	dirty = false;
}

void TextRenderer::SubmitText(const std::string& t) {
	if (this->text == t) return;

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