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

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstring>
#include <span>

#include "TMP.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "uint.hpp"

namespace jmaths {

/**
 * @brief Default constructor - initializes all bits to zero
 *
 * ALGORITHM: Value-initializes the digits array, setting all elements to 0
 */
template <bitcount_t V>
    requires TMP::is_power_of_2<V>
uint<V>::uint() : digits_{} {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Construct from integral type
 * @param num Integer value to copy into the uint
 *
 * ALGORITHM: Byte-by-byte copy with endianness handling
 * - Little-endian: Direct memcpy (bytes already in correct order)
 * - Big-endian: Reverse copy to convert to little-endian internal format
 *
 * DESIGN: Stores bytes in little-endian order internally regardless of
 * platform endianness for consistency across systems.
 *
 * NOTE: Only copies min(sizeof(num), sizeof(digits_)) bytes to prevent overflow
 */
template <bitcount_t V>
    requires TMP::is_power_of_2<V>
uint<V>::uint(std::integral auto num) : digits_{} {
    JMATHS_FUNCTION_TO_LOG;

    if constexpr (std::endian::native == std::endian::little) {
        // Platform is little-endian: direct copy
        std::memcpy(static_cast<void *>(digits_), &num, std::min(sizeof(num), std::size(digits_)));
    } else if constexpr (std::endian::native == std::endian::big) {
        // Platform is big-endian: reverse copy to convert to little-endian
        std::ranges::reverse_copy(reinterpret_cast<unsigned char *>(&num) + sizeof(num) -
                                      std::min(sizeof(num), std::size(digits_)),
                                  reinterpret_cast<unsigned char *>(&num) + sizeof(num),
                                  digits_);
    }
}

}  // namespace jmaths
