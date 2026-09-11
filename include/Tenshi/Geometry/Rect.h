#pragma once

#include <SDL3/SDL_rect.h>

namespace Tenshi {

struct Rect_f {
    float x;
    float y;
    float w;
    float h;

    // implicit conversion operator to SDL_FRect pointer
    operator const SDL_FRect*() const {
        return reinterpret_cast<const SDL_FRect*>(this);
    }
};

struct Rect_d {
    double x;
    double y;
    double w;
    double h;
};

} // namespace Tenshi