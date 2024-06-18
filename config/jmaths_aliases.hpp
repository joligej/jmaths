#pragma once

#ifndef PREPROCESSING_HEADER
    #include "../config/dependencies.hpp"
#endif

namespace jmaths {

using base_int = std::uint32_t; // or std::uint16_t on 32-bit architectures
using base_int_big = std::uint64_t; // or std::uint32_t on 32-bit architectures
using bit_type = unsigned long long; // the type that should store a number of bits, preferably the largest type available
inline constexpr base_int_big base = 4'294'967'296; // or 65'536 on 32-bit architectures
inline constexpr unsigned default_base = 10; // the default base that strings are assumed to be in
template <typename T> using allocator = std::allocator<T>; // the allocator used to store the numbers in a container on the heap

// you shouldn't edit the below code unless you know what you're doing

inline constexpr double max_ratio = 0.000125; // the ratio between sizeof(smallest type) and sizeof(base_int), preferably a lot smaller
inline constexpr bit_type bits_in_byte = 8;

inline constexpr base_int max_digit = base - 1;
inline constexpr std::size_t base_int_size = sizeof(base_int);
inline constexpr bit_type base_int_bits = base_int_size * bits_in_byte;

} // /namespace jmaths
