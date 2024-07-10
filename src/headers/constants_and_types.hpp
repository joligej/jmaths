#pragma once

#include <cstdint>
#include <cstddef>
#include <climits>
#include <memory>

namespace jmaths {

#if defined(UINT64_MAX)

using base_int = std::uint32_t;
using base_int_big = std::uint64_t;
inline constexpr base_int_big base = 4'294'967'296;

#elif defined(UINT32_MAX)

using base_int = std::uint16_t;
using base_int_big = std::uint32_t;
inline constexpr base_int_big base = 65'536;

#elif defined(UINT16_MAX)

using base_int = std::uint8_t;
using base_int_big = std::uint16_t;
inline constexpr base_int_big base = 256;

#else
    #error "No support for bit architectures below 16"
#endif

inline constexpr char negative_sign = '-'; // the sign that precedes negative numbers
inline constexpr char vinculum = '/'; // the fraction bar that separates the numerator and denominator of a fraction
using bit_type = unsigned long long; // the type that should store a count of bits, preferably the largest type available
inline constexpr unsigned default_base = 10; // the default base that strings are assumed to be in
template <typename T> using allocator = std::allocator<T>; // the allocator used to store the numbers in a container on the heap

// you shouldn't edit the below code unless you know what you're doing

inline constexpr double max_ratio = 0.000125; // the ratio between sizeof(smallest type) and sizeof(base_int), preferably a lot smaller
inline constexpr bit_type bits_in_byte = CHAR_BIT;

inline constexpr base_int max_digit = base - 1;
inline constexpr std::size_t base_int_size = sizeof(base_int);
inline constexpr bit_type base_int_bits = base_int_size * bits_in_byte;

} // /namespace jmaths