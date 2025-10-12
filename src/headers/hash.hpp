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

#include <cstddef>
#include <functional>

#include "TMP.hpp"
#include "declarations.hpp"

// declarations of std::hash specialisations

/**
 * @struct std::hash<basic_N_type>
 * @brief Hash function specialization for arbitrary-precision unsigned integers
 *
 * DESIGN: Allows basic_N to be used as key in std::unordered_map, std::unordered_set, etc.
 * The hash treats the internal digit representation as a byte sequence and hashes it
 * using std::hash<std::string_view> for good distribution properties.
 *
 * PROPERTIES:
 * - Consistent: Same value always produces same hash
 * - Well-distributed: Values are spread across hash space
 * - Fast: O(n) where n is number of digits
 */
template <jmaths::TMP::instance_of<jmaths::basic_N> basic_N_type> struct std::hash<basic_N_type> {
    /**
     * @brief Compute hash value for arbitrary-precision unsigned integer
     * @param n The number to hash
     * @return Hash value (std::size_t)
     *
     * See hash_impl.hpp for implementation details.
     */
    static std::size_t operator()(const basic_N_type & n);
};

/**
 * @struct std::hash<basic_Z_type>
 * @brief Hash function specialization for arbitrary-precision signed integers
 *
 * DESIGN: Similar to basic_N hash but incorporates the sign bit.
 * Ensures that positive and negative versions of the same magnitude have different hashes.
 *
 * FORMULA: hash(magnitude) XOR (sign << (first_digit % bits_in_size_t))
 */
template <jmaths::TMP::instance_of<jmaths::basic_Z> basic_Z_type> struct std::hash<basic_Z_type> {
    /**
     * @brief Compute hash value for arbitrary-precision signed integer
     * @param z The number to hash
     * @return Hash value (std::size_t)
     */
    static std::size_t operator()(const basic_Z_type & z);
};

/**
 * @struct std::hash<basic_Q_type>
 * @brief Hash function specialization for arbitrary-precision rational numbers
 *
 * DESIGN: Combines hashes of numerator, denominator, and sign using XOR operations.
 * Rational numbers in reduced form will have consistent hashes.
 *
 * FORMULA: (hash(num) XOR hash(denom)) XOR (sign << ((num_first XOR denom_first) % bits))
 */
template <jmaths::TMP::instance_of<jmaths::basic_Q> basic_Q_type> struct std::hash<basic_Q_type> {
    /**
     * @brief Compute hash value for arbitrary-precision rational number
     * @param q The number to hash
     * @return Hash value (std::size_t)
     */
    static std::size_t operator()(const basic_Q_type & q);
};

#include "hash_impl.hpp"
