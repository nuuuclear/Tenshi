#pragma once

#include <cstdint>
#include <cstddef>

namespace Tenshi {

template <typename T, std::size_t Size>
struct Vec;

// Vec2
template <typename T>
struct Vec<T, 2> {
    T x, y;

    constexpr Vec(T x, T y) : x(x), y(y) {} 
};

// Vec3
template <typename T>
struct Vec<T, 3> {
    T x, y, z;

    constexpr Vec(T x, T y, T z) : x(x), y(y), z(z) {}
    constexpr Vec(const Vec<T, 2>& v, T z) : x(v.x), y(v.y), z(z) {}
    constexpr Vec(T x, const Vec<T, 2>& v) : x(x), y(v.x), z(v.y) {}
};

} // namespace Tenshi
