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

#include <string_view>

#include "def.hh"
#include "error.hpp"

// member functions of error and derived types
namespace jmaths {

/**
 * @brief Default constructor using default message
 */
inline error::error() : error(default_message) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Constructor with custom error message
 * @param message The error message to store
 */
inline error::error(std::string_view message) : message_(message) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Get the error message (override from std::exception)
 * @return C-string containing the error message
 */
inline const char * error::what() const noexcept {
    JMATHS_FUNCTION_TO_LOG;

    return message_.c_str();
}

/**
 * @brief Default constructor for division_by_zero
 */
inline error::division_by_zero::division_by_zero() : error(default_message) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Constructor with custom message for division_by_zero
 */
inline error::division_by_zero::division_by_zero(std::string_view message) : error(message) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Check if a value is zero and throw exception if true
 * @param num Value to check
 * @param message Custom error message
 * @throws division_by_zero if num is zero
 *
 * ALGORITHM: Uses compile-time detection to check for is_zero() method
 * - If num has is_zero() method: use it (for arbitrary-precision types)
 * - Otherwise: compare with 0 (for built-in types)
 */
constexpr void error::division_by_zero::check(const auto & num, std::string_view message) {
    JMATHS_FUNCTION_TO_LOG;

    if constexpr (requires { !num.is_zero(); }) {
        // For types with is_zero() method (N, Z, Q)
        if (!num.is_zero()) { return; }
    } else {
        // For built-in numeric types
        if (num != 0) { return; }
    }

    throw division_by_zero(message);
}

/**
 * @brief Default constructor for invalid_base
 */
inline error::invalid_base::invalid_base() : error(default_message) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Constructor with custom message for invalid_base
 */
inline error::invalid_base::invalid_base(std::string_view message) : error(message) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Validate that base is within valid range [2, 64]
 * @param base The base to validate
 * @throws invalid_base if base is out of range
 *
 * NOTE: The range is limited because:
 * - Minimum 2: Binary is the smallest meaningful base
 * - Maximum 64: Limited by available printable characters (0-9, A-Z, a-z, +, ~)
 */
constexpr void error::invalid_base::check(unsigned base) {
    if (base >= minimum_base && base <= maximum_base) { return; }

    throw invalid_base(std::format("{} The base you entered was: {}.", default_message, base));
}

}  // namespace jmaths
