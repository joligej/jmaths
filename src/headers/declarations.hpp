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

#include "TMP.hpp"
#include "constants_and_types.hpp"

/**
 * @file declarations.hpp
 * @brief Forward declarations for all jmaths types
 *
 * This header provides forward declarations to break circular dependencies
 * between headers. Include this when you only need to declare pointers or
 * references to jmaths types without needing their full definitions.
 *
 * DESIGN PATTERN: Forward declaration header
 * - Allows faster compilation by reducing header dependencies
 * - Enables circular references between types
 * - Types are fully defined in their respective headers
 */

// all the types
namespace jmaths {

/**
 * @struct calc
 * @brief Mathematical calculation functions (forward declaration)
 * Full definition in calc.hpp
 */
struct calc;

/**
 * @class basic_N
 * @brief Arbitrary-precision unsigned integer (forward declaration)
 * @tparam BaseInt Base integer type for digits
 * @tparam BaseIntBig Wider type for intermediate calculations
 * @tparam Allocator Allocator for internal storage
 * Full definition in basic_N.hpp
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator> class basic_N;

/**
 * @class sign_type
 * @brief Base class for signed number types (forward declaration)
 * Full definition in sign_type.hpp
 */
class sign_type;

/**
 * @class basic_Z
 * @brief Arbitrary-precision signed integer (forward declaration)
 * @tparam BaseInt Base integer type for digits
 * @tparam BaseIntBig Wider type for intermediate calculations
 * @tparam Allocator Allocator for internal storage
 * Full definition in basic_Z.hpp
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator> class basic_Z;

/**
 * @class basic_Q
 * @brief Arbitrary-precision rational number (forward declaration)
 * @tparam BaseInt Base integer type for digits
 * @tparam BaseIntBig Wider type for intermediate calculations
 * @tparam Allocator Allocator for internal storage
 * Full definition in basic_Q.hpp
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator> class basic_Q;

/**
 * @class error
 * @brief Base exception class (forward declaration)
 * Full definition in error.hpp
 */
class error;

/**
 * @struct rand
 * @brief Random number generator (forward declaration)
 * @tparam T Type to generate (basic_N or basic_Z)
 * Full definition in rand.hpp
 */
template <TMP::instance_of<basic_N, basic_Z> T> struct rand;

/**
 * @class uint
 * @brief Fixed-size unsigned integer (forward declaration)
 * @tparam V Number of bits (must be power of 2)
 * Full definition in uint.hpp
 */
template <bitcount_t V>
    requires TMP::is_power_of_2<V>
class uint;

}  // namespace jmaths
