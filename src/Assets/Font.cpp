#include "Tenshi/Assets/Font.h"
#include "Tenshi/Core/Log.h"

namespace Tenshi {

Font::~Font() {
	if (m_font) {
		TTF_CloseFont(m_font);
	}
}

void Font::Load_FromTTF(TTF_Font* fontdata, std::vector<uint8_t>&& sourceMemory) {
	this->m_font = fontdata;
	this->m_fontDataBuffer = std::move(sourceMemory);

	if (this->m_font == NULL) {
		LogError(
			LogCategory::Application,
			"Failed to open font: ", 
			SDL_GetError()
		);
	}
}

void Font::Load_FromMemory(const char data) {

}

TTF_Font* Font::Get() {
	return m_font;
}

void Font::SetSize(float s) {
	TTF_SetFontSize(m_font, s);
}

} // namespace Tenshi