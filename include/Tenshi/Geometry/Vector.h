#pragma once

#include <cstdint>

namespace Tenshi {

template <typename T, std::size_t Size>
struct vec;

// vec2
template <typename T>
struct vec<T, 2> {
    T x, y;
};

// vec3
template <typename T>
struct vec<T, 3> {
    T x, y, z;
};


// vec2
template <typename T>
vec(T, T) -> vec<T, 2>;

// vec3
template <typename T>
vec(T, T, T) -> vec<T, 3>;

template <typename T>
vec(const vec<T, 2>&, T) -> vec<T, 3>;

template <typename T>
vec(T, const vec<T, 2>&) -> vec<T, 3>;


} // namespace Tenshi


