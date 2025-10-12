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

/**
 * @file all.hpp
 * @brief Convenience header that includes all jmaths library components
 *
 * This single header includes everything you need to use the jmaths library.
 * Include this for convenience, or include individual headers for faster
 * compilation when you only need specific components.
 *
 * INCLUDED COMPONENTS:
 * - TMP.hpp: Template metaprogramming utilities
 * - basic_N.hpp: Arbitrary-precision unsigned integers
 * - basic_Z.hpp: Arbitrary-precision signed integers
 * - basic_Q.hpp: Arbitrary-precision rational numbers
 * - calc.hpp: Mathematical calculation functions (GCD, sqrt, pow, etc.)
 * - error.hpp: Exception types
 * - hash.hpp: std::hash specializations for unordered containers
 * - literals.hpp: User-defined literals (_N, _Z, _Q)
 * - rand.hpp: Random number generation
 * - uint.hpp: Fixed-size unsigned integers
 * - constants_and_types.hpp: Type aliases and constants
 * - declarations.hpp: Forward declarations
 *
 * USAGE:
 * ```cpp
 * #include <jmaths/all.hpp>
 *
 * int main() {
 *     jmaths::N large_num = "123456789012345678901234567890"_N;
 *     jmaths::Z signed_num = -42_Z;
 *     jmaths::Q rational = jmaths::Q{3, 4};  // 3/4
 *     // ... use all jmaths features ...
 * }
 * ```
 */

#pragma once

#include "TMP.hpp"
#include "basic_N.hpp"
#include "basic_Q.hpp"
#include "basic_Z.hpp"
#include "calc.hpp"
#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "error.hpp"
#include "hash.hpp"
#include "literals.hpp"
#include "rand.hpp"
#include "uint.hpp"
