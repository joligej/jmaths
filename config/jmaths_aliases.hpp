#pragma once

#ifndef PREPROCESSING_HEADER
#include "../src/dependencies.hpp"
#endif

namespace jmaths {

using BASE_INT = std::uint32_t; // or std::uint16_t on 32-bit architectures
using BASE_INT_BIG = std::uint64_t; // or std::uint32_t on 32-bit architectures
using BIT_TYPE = unsigned long long; // the type that should store a number of bits, preferably the largest type available
inline constexpr BASE_INT_BIG BASE = 4'294'967'296; // or 65'536 on 32-bit architectures

// you shouldn't edit the below code unless you know what you're doing

inline constexpr unsigned DEFAULT_BASE = 10;
inline constexpr double MAX_RATIO = 0.000125; // the ratio between sizeof(smallest type) and sizeof(BASE_INT), preferably a lot smaller
inline constexpr BIT_TYPE BITS_IN_BYTE = 8;
template <typename T> using ALLOCATOR = std::allocator<T>;

inline constexpr BASE_INT MAX_DIGIT = BASE - 1;
inline constexpr std::size_t BASE_INT_SIZE = sizeof(BASE_INT);
inline constexpr BIT_TYPE BASE_INT_BITS = BASE_INT_SIZE * BITS_IN_BYTE;

} // /namespace jmaths
