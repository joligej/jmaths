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

#include <exception>
#include <format>
#include <string>
#include <string_view>

// definitions of error and associated types
namespace jmaths {

/**
 * @class error
 * @brief Base exception class for jmaths library errors
 *
 * Inherits from std::exception and provides custom error messages.
 * All jmaths-specific exceptions derive from this class.
 */
class error : public std::exception {
   public:
    static constexpr char default_message[] = "No error message provided!";

    class division_by_zero;
    class invalid_base;

    error();
    explicit error(std::string_view message);
    [[nodiscard]] const char * what() const noexcept override;

   private:
    const std::string message_;
};

/**
 * @class error::division_by_zero
 * @brief Exception thrown when attempting to divide by zero
 *
 * USAGE: Thrown by division and modulo operations when the divisor is zero.
 * Provides a static check() method for convenient validation.
 */
class error::division_by_zero : public error {
   public:
    static constexpr char default_message[] = "You tried to divide by zero!";

    division_by_zero();
    explicit division_by_zero(std::string_view message);

    /**
     * @brief Check if a number is zero and throw if true
     * @param num The number to check (any type with is_zero() or comparable to 0)
     * @param message Custom error message (optional)
     * @throws division_by_zero if num is zero
     *
     * ALGORITHM: Uses SFINAE to detect if num has is_zero() method,
     * otherwise compares with literal 0
     */
    static constexpr void check(const auto & num, std::string_view message = default_message);
};

/**
 * @class error::invalid_base
 * @brief Exception thrown when an invalid number base is specified
 *
 * CONSTRAINTS: Base must be between 2 and 64 inclusive.
 * - Base 2-10: Uses digits 0-9
 * - Base 11-36: Uses 0-9, A-Z
 * - Base 37-62: Uses 0-9, A-Z, a-z
 * - Base 63-64: Uses 0-9, A-Z, a-z, +, ~
 */
class error::invalid_base : public error {
   public:
    static constexpr char default_message[] = "You need to enter a base between 2 and 64!";

    invalid_base();
    explicit invalid_base(std::string_view message);

    /**
     * @brief Check if base is valid and throw if not
     * @param base The base to validate
     * @throws invalid_base if base < 2 or base > 64
     */
    static constexpr void check(unsigned base);

    static constexpr unsigned minimum_base = 2U;
    static constexpr unsigned maximum_base = 64U;
};

}  // namespace jmaths

#include "error_impl.hpp"
