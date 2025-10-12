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
#include <string_view>
#include <type_traits>

#include "declarations.hpp"

// declarations of sign_type and associated functions and types
namespace jmaths {

/**
 * @class sign_type
 * @brief Base class for signed number types (Z and Q)
 *
 * This class provides sign management functionality for arbitrary-precision signed integers
 * and rational numbers. The sign is stored as a single bit (bool) for memory efficiency.
 *
 * DESIGN: Uses the sign-magnitude representation where:
 * - sign_ = 0 (positive) means the number is >= 0
 * - sign_ = 1 (negative) means the number is < 0
 *
 * NOTE: Zero is always represented as positive (sign_ = 0) for canonical form.
 */
class sign_type {
    friend struct calc;

    template <TMP::instance_of<basic_N, basic_Z> T> friend struct rand;

   public:
    /**
     * @brief Check if the number is positive (>= 0)
     * @return true if sign_ == positive (0)
     */
    [[nodiscard]] constexpr bool is_positive() const;

    /**
     * @brief Check if the number is negative (< 0)
     * @return true if sign_ == negative (1)
     */
    [[nodiscard]] constexpr bool is_negative() const;

    /**
     * @brief Pure virtual function to check if the number is zero
     * @return true if magnitude is zero
     * NOTE: Must be implemented by derived classes (basic_Z, basic_Q)
     */
    [[nodiscard]] virtual constexpr bool is_zero() const = 0;

    /**
     * @brief Flip the sign of the number (positive <-> negative)
     * NOTE: Does nothing if the number is zero (maintains canonical form)
     */
    constexpr void flip_sign();

   protected:
    /**
     * @enum sign_bool
     * @brief Enumeration for sign representation
     * Uses bool as underlying type for memory efficiency (1 bit storage)
     */
    enum sign_bool : bool { positive = 0, negative = 1 } sign_;

    // Constructors (protected - only derived classes can instantiate)
    constexpr sign_type();
    constexpr sign_type(sign_bool sign);

    /**
     * @brief Construct from string, extracting sign prefix
     * @param num_str Pointer to string view (modified to remove sign prefix)
     * ALGORITHM: Checks for leading '-' character and removes it
     */
    constexpr sign_type(std::string_view * num_str);

    /**
     * @brief Construct from integer, extracting and removing sign
     * @param num Pointer to integer (modified to make positive)
     * ALGORITHM: For signed types, checks sign bit and negates if needed
     */
    constexpr sign_type(std::integral auto * num);

    /**
     * @brief Static helper to extract sign from string
     * @param num_str Pointer to string (modified to remove sign)
     * @return sign_bool indicating the extracted sign
     */
    [[nodiscard]] static constexpr sign_bool handle_string_(std::string_view * num_str);

    /**
     * @brief Static helper to extract sign from integer
     * @param num Pointer to integer (modified to make positive)
     * @return sign_bool indicating the extracted sign
     * NOTE: For unsigned types, always returns positive
     */
    [[nodiscard]] static constexpr sign_bool handle_int_(std::integral auto * num);

    constexpr void set_sign_(std::convertible_to<std::underlying_type_t<sign_bool>> auto val);
};

}  // namespace jmaths

#include "sign_type_impl.hpp"
