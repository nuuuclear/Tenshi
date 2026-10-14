#pragma once

#include <SDL3/SDL_rect.h>

#include <cstdint>
#include <cstddef>

namespace Tenshi {

template <typename T>
struct Rect {
    T x, y, w, h;

    Rect() : x(0), y(0), w(0), h(0) {}
    Rect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}

    // conversion constructor for different numberic types (e.g. Rect<double> to Rect<int>)
    template <typename U>
    explicit Rect(const Rect<U>& other)
    :   x(static_cast<T>(other.x)),
        y(static_cast<T>(other.y)),
        w(static_cast<T>(other.w)),
        h(static_cast<T>(other.h)) {}

    T area() const {
        return w * h;
    }

    // conversion for sdl
    explicit operator SDL_FRect() const {
        return SDL_FRect{
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(w),
            static_cast<float>(h)
        };
    }

    explicit operator SDL_Rect() const {
        return SDL_Rect{
            static_cast<int>(x),
            static_cast<int>(y),
            static_cast<int>(w),
            static_cast<int>(h)
        };
    }
};

} // namespace Tenshi