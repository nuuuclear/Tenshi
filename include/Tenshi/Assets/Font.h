#pragma once

#include <vector>
#include <cstdint>
#include <memory>

namespace Tenshi {

class Font {
public:
	struct GlyphBitmap {
		int width = 0;
		int height = 0;
		
		int xOffset = 0;
		int yOffset = 0;

		std::vector<uint8_t> pixels;
	};

	Font();
	~Font();
	
	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;

	bool LoadFromMemory(std::vector<uint8_t>&& sourceMemory, float pixelHeight);
	bool IsLoaded() const;

	void SetSize(float s);

	float GetSize() const;
	int GetAscent() const;
	int GetDescent() const;
	int GetLineSkip() const;
	float GetAdvance(uint32_t codepoint) const;
	float GetKerningAdvance(uint32_t left, uint32_t right) const;

	bool RasterizeGlyph(uint32_t codepoint, GlyphBitmap& glyph) const;

private:
	struct Data;
	std::unique_ptr<Data> m_data;
};

} // namespace Tenshi