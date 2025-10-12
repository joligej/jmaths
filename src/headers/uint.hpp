// The jmaths library for C++
// Copyright (C) 2025  Jasper de Smaele

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <concepts>
#include <cstddef>

#include "TMP.hpp"
#include "constants_and_types.hpp"

namespace jmaths {

/**
 * @class uint
 * @brief Fixed-size unsigned integer with compile-time specified bit width
 * @tparam V Number of bits (must be a power of 2)
 *
 * DESIGN: Provides fixed-width unsigned integers of arbitrary size (e.g., uint<256>, uint<512>)
 * Unlike basic_N which is dynamically-sized, uint<V> always uses exactly V bits.
 *
 * USE CASES:
 * - Cryptographic operations requiring specific bit widths
 * - Hash digests (uint<256> for SHA-256, uint<512> for SHA-512)
 * - Fixed-precision arithmetic where exact bit width matters
 *
 * CONSTRAINTS: V must be a power of 2 (enforced by requires clause)
 *
 * TODO: This class is currently minimal. Consider expanding with arithmetic operations.
 */
template <bitcount_t V>
    requires TMP::is_power_of_2<V>
class uint {
    // TODO: Consider if element_type should be configurable or always unsigned char
    // using element_type = unsigned char;  // base_int_big ???

   public:
    /**
     * @brief Element type used for internal storage
     *
     * ALGORITHM: Selects the largest unsigned integer type that fits within V/8 bytes
     * using template metaprogramming. This optimizes memory layout and access patterns.
     * For example:
     * - uint<256> might use uint64_t elements (4 elements)
     * - uint<128> might use uint64_t elements (2 elements)
     * - uint<64> would use uint64_t (1 element)
     */
    using element_type = TMP::matches_which_t<TMP::type_size<V / bits_in_byte>,
                                              TMP::leq_size,
                                              TMP::unsigned_integral_types>;

   public:
    /**
     * @brief Default constructor - initializes to zero
     */
    uint();

    /**
     * @brief Construct from standard integral type
     * @param num The integer value to initialize from
     *
     * ALGORITHM: Copies bytes from num into internal storage using memcpy,
     * handling endianness appropriately. See uint_impl.hpp for details.
     */
    uint(std::integral auto num);

   private:
    /**
     * @brief Internal storage array
     * Size is calculated as V / (element_size_in_bits)
     * Stored in native byte order
     */
    element_type digits_[V / (sizeof(element_type) * bits_in_byte)];
};

}  // namespace jmaths

#include "uint_impl.hpp"
