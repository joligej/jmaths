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

#include "basic_N.hpp"
#include "basic_Q.hpp"
#include "basic_Z.hpp"

// declarations of literals
namespace jmaths::inline literals {

/**
 * @brief User-defined literal for creating arbitrary-precision unsigned integers
 * @param num_str String representation of the number
 * @return N type (arbitrary-precision unsigned integer)
 *
 * USAGE: auto num = 12345678901234567890_N;
 *
 * NOTE: Only available if JMATHS_TYPEDEF_N is defined (creates N typedef)
 * This allows convenient syntax for creating large numbers from literals.
 */
#if defined(JMATHS_TYPEDEF_N)
constexpr N operator""_N(const char * num_str);
#endif

/**
 * @brief User-defined literal for creating arbitrary-precision signed integers
 * @param num_str String representation of the number (may start with '-')
 * @return Z type (arbitrary-precision signed integer)
 *
 * USAGE: auto num = -12345678901234567890_Z;
 *
 * NOTE: Only available if JMATHS_TYPEDEF_Z is defined (creates Z typedef)
 */
#if defined(JMATHS_TYPEDEF_Z)
constexpr Z operator""_Z(const char * num_str);
#endif

/**
 * @brief User-defined literal for creating arbitrary-precision rational numbers
 * @param num_str String representation of numerator
 * @return Q type (arbitrary-precision rational number with denominator = 1)
 *
 * USAGE: auto num = 123_Q;
 * Creates rational number 123/1
 *
 * NOTE: Only available if both JMATHS_TYPEDEF_Q and JMATHS_TYPEDEF_N are defined
 * For fractions, use constructor: Q{numerator, denominator}
 */
#if defined(JMATHS_TYPEDEF_Q)
constexpr Q operator""_Q(const char * num_str);
#endif

}  // namespace jmaths::inline literals

#include "literals_impl.hpp"
