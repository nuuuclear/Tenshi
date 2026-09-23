#pragma once

#include <cstdint>
#include <cstddef>

namespace Tenshi {

template <typename T, std::size_t Size>
struct vec;

// vec2
template <typename T>
struct vec<T, 2> {
    T x, y;

    constexpr vec(T x, T y) : x(x), y(y) {} 
};

// vec3
template <typename T>
struct vec<T, 3> {
    T x, y, z;

    constexpr vec(T x, T y, T z) : x(x), y(y), z(z) {}
    constexpr vec(const vec<T, 2>& v, T z) : x(v.x), y(v.y), z(z) {}
    constexpr vec(T x, const vec<T, 2>& v) : x(x), y(v.x), z(v.y) {}
};

} // namespace Tenshi
