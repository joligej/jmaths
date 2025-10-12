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

#include <utility>

#include "TMP.hpp"
#include "declarations.hpp"

namespace jmaths {

/**
 * @struct calc
 * @brief Collection of mathematical calculation functions for arbitrary-precision numbers
 *
 * This struct provides static member functions for various mathematical operations
 * that go beyond basic arithmetic. All functions are constexpr-compatible.
 *
 * DESIGN: Uses a struct instead of namespace to allow friending for private access
 * to internal number representations when needed for optimization.
 */
struct calc {
    /**
     * @brief Calculate Greatest Common Divisor using binary GCD algorithm
     * @tparam basic_N_type_1 First arbitrary-precision unsigned integer type
     * @tparam basic_N_type_2 Second type (must decay to same as first)
     * @param a First number
     * @param b Second number
     * @return GCD(a, b)
     *
     * ALGORITHM: Binary GCD (Stein's algorithm) - O(n log n)
     * More efficient than Euclidean algorithm for binary computers.
     * See calc_impl.hpp for detailed implementation.
     */
    template <TMP::instance_of<basic_N> basic_N_type_1,
              TMP::decays_to<basic_N_type_1> basic_N_type_2>
    static constexpr auto gcd(basic_N_type_1 && a, basic_N_type_2 && b)
        -> std::decay_t<basic_N_type_1>;

    /**
     * @brief Calculate integer square root with remainder
     * @param num Number to take square root of
     * @return Pair of (sqrt, remainder) where num = sqrt² + remainder
     *
     * ALGORITHM: Binary search - O(log n) iterations × O(n²) per multiplication
     * Finds largest integer x such that x² ≤ num
     */
    template <TMP::instance_of<basic_N> basic_N_type>
    static constexpr auto sqrt(basic_N_type && num)
        -> std::pair<std::decay_t<basic_N_type>, std::decay_t<basic_N_type>>;

    /**
     * @brief Calculate integer square root (floor only, no remainder)
     * @param num Number to take square root of
     * @return Floor of square root
     *
     * ALGORITHM: Similar to sqrt() but doesn't compute remainder
     * Slightly more efficient when remainder is not needed
     */
    template <TMP::instance_of<basic_N> basic_N_type>
    static constexpr auto sqrt_whole(basic_N_type && num) -> std::decay_t<basic_N_type>;

    /**
     * @brief Calculate power (exponentiation) for unsigned integers
     * @param base The base number
     * @param exponent The exponent (must be non-negative)
     * @return base^exponent
     *
     * ALGORITHM: Exponentiation by squaring - O(log exponent)
     * Much faster than repeated multiplication for large exponents
     */
    template <TMP::instance_of<basic_N> basic_N_type_1,
              TMP::decays_to<basic_N_type_1> basic_N_type_2>
    static constexpr auto pow(basic_N_type_1 && base, basic_N_type_2 && exponent)
        -> std::decay_t<basic_N_type_1>;

    /**
     * @brief Calculate modular exponentiation
     * @param base The base number
     * @param exponent The exponent
     * @param mod The modulus
     * @return (base^exponent) mod mod
     *
     * ALGORITHM: Modular exponentiation by squaring - O(log exponent)
     * Crucial for cryptographic applications (RSA, Diffie-Hellman, etc.)
     * Keeps intermediate results small to prevent overflow
     */
    template <TMP::instance_of<basic_N> basic_N_type_1,
              TMP::decays_to<basic_N_type_1> basic_N_type_2>
    static constexpr auto pow_mod(basic_N_type_1 && base,
                                  basic_N_type_2 && exponent,
                                  const std::decay_t<basic_N_type_1> & mod)
        -> std::decay_t<basic_N_type_1>;

    /**
     * @brief Calculate power for signed integers
     * @param base The signed base number
     * @param exponent The exponent (unsigned)
     * @return base^exponent with appropriate sign
     *
     * SIGN RULE: Result is negative if base is negative and exponent is odd
     */
    template <TMP::instance_of<basic_Z> basic_Z_type, TMP::instance_of<basic_N> basic_N_type>
    static constexpr auto pow(basic_Z_type && base, basic_N_type && exponent)
        -> std::decay_t<basic_Z_type>;
};  // struct calc

}  // namespace jmaths

#include "calc_impl.hpp"
