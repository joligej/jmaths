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
#include "basic_Z.hpp"
#include "def.hh"
#include "literals.hpp"

namespace jmaths::inline literals {

/**
 * @brief User-defined literal implementation for N (unsigned)
 * @param num_str C-string containing the number
 * @return N constructed from the string
 *
 * ALGORITHM: Directly constructs N from string using N's string constructor,
 * which parses the string in the default base (10).
 */
#if defined(JMATHS_TYPEDEF_N)
constexpr N operator""_N(const char * num_str) {
    JMATHS_FUNCTION_TO_LOG;

    return N{num_str};
}
#endif

/**
 * @brief User-defined literal implementation for Z (signed)
 * @param num_str C-string containing the number (may include '-' prefix)
 * @return Z constructed from the string
 *
 * ALGORITHM: Constructs Z from string, which automatically handles
 * sign extraction and magnitude parsing.
 */
#if defined(JMATHS_TYPEDEF_Z)
constexpr Z operator""_Z(const char * num_str) {
    JMATHS_FUNCTION_TO_LOG;

    return Z{num_str};
}
#endif

/**
 * @brief User-defined literal implementation for Q (rational)
 * @param num_str C-string containing the numerator
 * @return Q constructed with given numerator and denominator = 1
 *
 * ALGORITHM: First creates N from string, then constructs Q from that N.
 * This creates a rational number with denominator 1.
 *
 * NOTE: Requires both JMATHS_TYPEDEF_Q and JMATHS_TYPEDEF_N to be defined
 */
#if defined(JMATHS_TYPEDEF_Q) && defined(JMATHS_TYPEDEF_N)
constexpr Q operator""_Q(const char * num_str) {
    JMATHS_FUNCTION_TO_LOG;

    return Q{N{num_str}};
}
#endif

}  // namespace jmaths::inline literals
