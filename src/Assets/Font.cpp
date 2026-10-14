#include "Tenshi/Assets/Font.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <algorithm>
#include <cmath>
#include <utility>

namespace Tenshi {

struct Font::Data {
    stbtt_fontinfo info {};
    std::vector<uint8_t> bytes;

    float pixelHeight = 0.0f;
    float scale = 0.0f;

    int ascent = 0;
    int descent = 0;
    int lineGap = 0;
};

Font::Font() : m_data(std::make_unique<Data>()) 
{
}

Font::~Font() = default;

bool Font::LoadFromMemory(std::vector<uint8_t>&& sourceMemory, float pixelHeight) {
    if (sourceMemory.empty() || pixelHeight <= 0.0f) return false;

    m_data = std::make_unique<Data>();
    m_data->bytes = std::move(sourceMemory);

    const int offset = stbtt_GetFontOffsetForIndex(m_data->bytes.data(), 0);
    if (offset < 0 
    ||  !stbtt_InitFont(&m_data->info, m_data->bytes.data(), offset)
    ) {
        m_data = std::make_unique<Data>();
        return false;
    }

    stbtt_GetFontVMetrics(
        &m_data->info,
        &m_data->ascent,
        &m_data->descent,
        &m_data->lineGap
    );

    SetSize(pixelHeight);

    return true;
}

bool Font::IsLoaded() const {
    return m_data 
        && !m_data->bytes.empty()
        && m_data->scale > 0.0f;
}

void Font::SetSize(float pixelHeight) {
    if (!m_data || m_data->bytes.empty() || pixelHeight <= 0.0f) {
        return;
    }

    m_data->pixelHeight = pixelHeight;
    m_data->scale = stbtt_ScaleForMappingEmToPixels(&m_data->info, pixelHeight);
}

float Font::GetSize() const {
    return m_data ? m_data->pixelHeight : 0.0f;
}

int Font::GetAscent() const {
    return IsLoaded() ? static_cast<int>(
        std::ceil(m_data->ascent * m_data->scale)
    ) : 0;
}

int Font::GetDescent() const {
    return IsLoaded() ? static_cast<int>(
        std::floor(m_data->descent * m_data->scale)
    ) : 0;
}

int Font::GetLineSkip() const {
    if (!IsLoaded()) {
        return 0;
    }

    return std::max(1, static_cast<int>(std::ceil(
        (m_data->ascent - m_data->descent + m_data->lineGap) * m_data->scale
    )));
}

float Font::GetAdvance(uint32_t codepoint) const {
    if (!IsLoaded() || codepoint > 0x10FFFF) {
        return 0.0f;
    }

    int advance = 0;
    stbtt_GetCodepointHMetrics(
        &m_data->info,
        static_cast<int>(codepoint),
        &advance,
        nullptr
    );

    return advance * m_data->scale;
}

float Font::GetKerningAdvance(uint32_t left, uint32_t right) const {
    if (!IsLoaded() || left > 0x10FFFF || right > 0x10FFFF) {
        return 0.0f;
    }
    return stbtt_GetCodepointKernAdvance(
        &m_data->info,
        static_cast<int>(left),
        static_cast<int>(right)
    ) * m_data->scale;
}

bool Font::RasterizeGlyph(uint32_t codepoint, GlyphBitmap& glyph) const {
    glyph = {};
    if (!IsLoaded() || codepoint > 0x10FFFF) return false;

    int width = 0;
    int height = 0;
    unsigned char* bitmap = stbtt_GetCodepointBitmap(
        &m_data->info,
        m_data->scale,
        m_data->scale,
        static_cast<int>(codepoint),
        &width, &height,
        &glyph.xOffset, &glyph.yOffset
    );

    glyph.width = width;
    glyph.height = height;

    if (bitmap && width > 0 && height > 0) {
        glyph.pixels.assign(bitmap, bitmap + (width * height));
    }

    stbtt_FreeBitmap(bitmap, nullptr);

    return true;
}

} // namespace Tenshi