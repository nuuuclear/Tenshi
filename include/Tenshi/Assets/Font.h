#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <vector>
#include <cstdint>

namespace Tenshi {

class Font {
public:
	~Font();

	void Load_FromTTF(TTF_Font* fontdata, std::vector<uint8_t>&& sourceMemory);
	void Load_FromMemory(const char data);

	void SetSize(float s);

	TTF_Font* Get();
private:
	TTF_Font* m_font = nullptr;
	std::vector<uint8_t> m_fontDataBuffer;
};

} // namespace Tenshi