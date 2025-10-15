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

#include "constants_and_types.hpp"
#include "def.hh"
#include "sign_type.hpp"

// member function templates of sign_type
namespace jmaths {

/**
 * @brief Template constructor for sign_type from integral types
 * @param num Pointer to integer value (will be modified to make positive)
 *
 * ALGORITHM: Extracts sign information from the integer
 * - For unsigned types: always positive
 * - For signed types: checks if negative, makes positive, stores sign
 */
constexpr sign_type::sign_type(std::integral auto * num) : sign_{handle_int_(num)} {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Static helper to handle sign extraction from integers
 * @param num Pointer to integer (modified in-place to make positive)
 * @return sign_bool indicating whether the original value was negative
 *
 * ALGORITHM:
 * - Unsigned integers: always return positive
 * - Signed integers: if negative, multiply by -1 (make positive) and return negative
 *
 * NOTE: Suppresses -Wsign-conversion warning because we're intentionally
 * converting from signed to unsigned after ensuring the value is positive
 */
constexpr sign_type::sign_bool sign_type::handle_int_(std::integral auto * num) {
    JMATHS_FUNCTION_TO_LOG;

    if constexpr (std::unsigned_integral<decltype(*num)>) {
        return positive;
    } else {
        if (*num < 0) {
            // Handle negation safely to avoid undefined behavior with INT_MIN
            // Cast to unsigned, negate, then cast back to signed
            using value_type = std::remove_reference_t<decltype(*num)>;
            using unsigned_type = std::make_unsigned_t<value_type>;
            *num = static_cast<value_type>(-static_cast<unsigned_type>(*num));
            return negative;
        } else {
            return positive;
        }
    }
}

/**
 * @brief Set the sign value (helper function)
 * @param val Value to set as sign (convertible to bool)
 */
constexpr void sign_type::set_sign_(
    std::convertible_to<std::underlying_type_t<sign_bool>> auto val) {
    JMATHS_FUNCTION_TO_LOG;

    sign_ = static_cast<sign_bool>(val);
}

}  // namespace jmaths

// member functions of sign_type
namespace jmaths {

/**
 * @brief Default constructor - initializes to positive
 */
constexpr sign_type::sign_type() : sign_{positive} {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Constructor with explicit sign value
 * @param sign The sign_bool value to initialize with
 */
constexpr sign_type::sign_type(sign_bool sign) : sign_{sign} {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Constructor from string view (extracts sign prefix)
 * @param num_str Pointer to string view (will be modified to remove sign)
 */
constexpr sign_type::sign_type(std::string_view * num_str) : sign_{handle_string_(num_str)} {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Static helper to extract sign from string
 * @param num_str Pointer to string view (modified to remove sign prefix)
 * @return sign_bool indicating the sign
 *
 * ALGORITHM:
 * 1. Check if string starts with '-' (negative_sign constant)
 * 2. If yes, remove the prefix and return negative
 * 3. If string becomes empty after removal, treat as positive (invalid input)
 * 4. Otherwise return positive
 *
 * NOTE: This modifies the input string by removing the sign prefix
 */
constexpr sign_type::sign_bool sign_type::handle_string_(std::string_view * num_str) {
    JMATHS_FUNCTION_TO_LOG;

    if (!num_str->empty() && num_str->front() == negative_sign) {
        num_str->remove_prefix(1U);
        return num_str->empty() ? positive : negative;
    } else {
        return positive;
    }
}

/**
 * @brief Check if number is positive (>= 0)
 * @return true if positive or zero
 */
constexpr bool sign_type::is_positive() const {
    JMATHS_FUNCTION_TO_LOG;

    return sign_ == positive;
}

/**
 * @brief Check if number is negative (< 0)
 * @return true if negative
 */
constexpr bool sign_type::is_negative() const {
    JMATHS_FUNCTION_TO_LOG;

    return sign_ == negative;
}

/**
 * @brief Flip the sign (positive <-> negative)
 */
constexpr void sign_type::flip_sign() {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return; }
    set_sign_(!sign_);
}

}  // namespace jmaths
