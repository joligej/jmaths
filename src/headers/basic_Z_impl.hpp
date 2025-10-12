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

#include <compare>
#include <concepts>
#include <cstddef>
#include <istream>
#include <limits>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "basic_N.hpp"
#include "basic_Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "error.hpp"
#include "sign_type.hpp"

/**
 * @file basic_Z_impl.hpp
 * @brief Implementation of basic_Z member functions and operators
 *
 * This file contains the implementation of arbitrary-precision signed integer
 * operations including construction, arithmetic, comparison, and type conversions.
 *
 * KEY ALGORITHMS:
 * - Sign-magnitude representation with sign managed separately
 * - All arithmetic operations decompose into unsigned operations plus sign logic
 * - Increment/decrement handle sign transitions across zero
 * - String conversion prepends negative sign when needed
 */

// comparison functions for Z with integral types
namespace jmaths {

/**
 * @brief Equality comparison with integral types (implementation detail)
 * @param lhs Signed arbitrary-precision integer
 * @param rhs Native integer type
 * @return true if equal
 *
 * ALGORITHM:
 * 1. Try to fit lhs into rhs's type
 * 2. If successful, compare values
 * 3. If doesn't fit, not equal
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_eq(const basic_Z & lhs,
                                                                       std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.template fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit == rhs;
    }

    return false;
}

/**
 * @brief Three-way comparison with integral types (implementation detail)
 * @param lhs Signed arbitrary-precision integer
 * @param rhs Native integer type
 * @return Ordering relationship
 *
 * ALGORITHM:
 * 1. Try to fit lhs into rhs's type
 * 2. If successful, use native comparison
 * 3. If doesn't fit: negative lhs is less, positive lhs is greater
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::strong_ordering basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_comp(
    const basic_Z & lhs,
    std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.template fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit <=> rhs;
    }

    return lhs.is_negative() ? std::strong_ordering::less : std::strong_ordering::greater;
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr bool operator==(const basic_Z_type & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_eq(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr bool operator==(std::integral auto lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_eq(rhs, lhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::strong_ordering operator<=>(const basic_Z_type & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_comp(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::strong_ordering operator<=>(std::integral auto lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return 0 <=> basic_Z_type::detail::opr_comp(rhs, lhs);
}

}  // namespace jmaths

// member function templates of Z
namespace jmaths {

/**
 * @brief Construct from integral type
 * @param num Integer value
 *
 * ALGORITHM:
 * 1. Extract sign (modifies num to make positive)
 * 2. Pass positive value to basic_N constructor
 *
 * COMPLEXITY: O(1) for sign extraction + O(log n) for value conversion
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(std::integral auto num) :
    sign_type(&num), basic_N_type(num) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Try to fit into unsigned integral type
 * @tparam T Target unsigned type
 * @return std::optional<T> containing value, or nullopt if negative or too large
 *
 * ALGORITHM:
 * 1. Check if negative (cannot fit in unsigned)
 * 2. Delegate to basic_N::fits_into for unsigned conversion
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <std::unsigned_integral T>
constexpr std::optional<T> basic_Z<BaseInt, BaseIntBig, Allocator>::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) { return std::nullopt; }

    return basic_N_type::template fits_into<T>();
}

/**
 * @brief Try to fit into signed integral type
 * @tparam T Target signed type
 * @return std::optional<T> containing value, or nullopt if too large
 *
 * ALGORITHM:
 * 1. If positive: use basic_N::fits_into (must fit in positive range)
 * 2. If negative:
 *    a. Try fitting magnitude into unsigned version of T
 *    b. Check if magnitude <= max_positive + 1 (for two's complement)
 *    c. Negate the result
 *
 * EXAMPLE: For int8_t, range is [-128, 127]
 * - Positive values must fit in [0, 127]
 * - Negative values: magnitude must fit in [0, 128]
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <std::signed_integral T>
constexpr std::optional<T> basic_Z<BaseInt, BaseIntBig, Allocator>::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    static_assert(-1 == ~0, "Two's complement architecture required.");

    if (is_positive()) { return basic_N_type::template fits_into<T>(); }

    auto fits_into_unsigned = basic_N_type::template fits_into<std::make_unsigned_t<T>>();

    if (!fits_into_unsigned.has_value()) { return std::nullopt; }

    // Check if magnitude fits in negative range (can be one larger than max positive)
    if (*fits_into_unsigned >
        static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()) + 1) {
        return std::nullopt;
    }

    return -*fits_into_unsigned;
}

/**
 * @brief Assign from integral type
 * @param rhs Integer value
 * @return Reference to *this
 *
 * ALGORITHM:
 * 1. Extract sign using handle_int_ (modifies rhs)
 * 2. Assign magnitude to basic_N part
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator=(std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(sign_type::handle_int_(&rhs));
    basic_N_type::opr_assign_(rhs);
    return *this;
}

}  // namespace jmaths

// binary operators for Z
namespace jmaths {

/**********************************************************/
// forwarding functions

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::ostream & operator<<(std::ostream & os, const basic_Z_type & z) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_ins(os, z);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::istream & operator>>(std::istream & is, basic_Z_type & z) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_extr(is, z);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator+(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_add(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator-(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_subtr(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator*(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_mult(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::pair<basic_Z_type, basic_Z_type> operator/(const basic_Z_type & lhs,
                                                          const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(rhs);
    return basic_Z_type::detail::opr_div(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator&(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_and(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator|(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_or(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator^(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_xor(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr bool operator==(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_eq(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::strong_ordering operator<=>(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_comp(lhs, rhs);
}

}  // namespace jmaths

// member functions of Z
namespace jmaths {

/**
 * @brief Private constructor with explicit sign (move version)
 * @param n Magnitude (moved)
 * @param sign Sign of the integer
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(basic_N_type && n, sign_bool sign) :
    sign_type(sign), basic_N_type(std::move(n)) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Private constructor with explicit sign (copy version)
 * @param n Magnitude (copied)
 * @param sign Sign of the integer
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(const basic_N_type & n, sign_bool sign) :
    sign_type(sign), basic_N_type(n) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Convert to string in specified base
 * @param base Numeric base (2-64)
 * @return String representation with optional negative sign
 *
 * ALGORITHM: Prepends '-' if negative, delegates to basic_N for conversion
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Z<BaseInt, BaseIntBig, Allocator>::conv_to_base_(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) {
        return negative_sign + basic_N_type::conv_to_base_(base);
    } else {
        return basic_N_type::conv_to_base_(base);
    }
}

/**
 * @brief Get size of dynamic memory allocation
 * @return Size in bytes
 *
 * ALGORITHM: Delegates to basic_N (sign doesn't require dynamic allocation)
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::size_t basic_Z<BaseInt, BaseIntBig, Allocator>::dynamic_size_() const {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::dynamic_size_();
}

/**
 * @brief Default constructor - creates zero
 * POSTCONDITION: is_zero() == true, is_positive() == true
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z() = default;

/**
 * @brief Construct from string representation
 * @param num_str String in format "[-]digits"
 * @param base Numeric base (default 10)
 *
 * ALGORITHM:
 * 1. Extract sign from beginning (via sign_type constructor)
 * 2. Parse remaining digits using basic_N
 * 3. Ensure zero is always positive
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(std::string_view num_str, unsigned base) :
    sign_type(&num_str), basic_N_type(num_str, base) {
    JMATHS_FUNCTION_TO_LOG;

    if (basic_Z::is_zero()) { set_sign_(positive); }
}

/**
 * @brief Construct from basic_N (unsigned integer)
 * @param n Unsigned value
 * POSTCONDITION: Sign is positive
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(const basic_N_type & n) :
    basic_N_type(n) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Construct from moved basic_N (unsigned integer)
 * @param n Unsigned value (moved)
 * POSTCONDITION: Sign is positive
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(basic_N_type && n) :
    basic_N_type(std::move(n)) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Get absolute value (const lvalue version)
 * @return Reference to magnitude as basic_N
 *
 * OPTIMIZATION: No copy - returns reference to base class
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Z<BaseInt, BaseIntBig, Allocator>::abs() const & -> const basic_N_type & {
    JMATHS_FUNCTION_TO_LOG;

    return static_cast<const basic_N_type &>(*this);
}

/**
 * @brief Get absolute value (rvalue version)
 * @return Moved magnitude as basic_N
 *
 * OPTIMIZATION: Moves base class without copying
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Z<BaseInt, BaseIntBig, Allocator>::abs() && -> basic_N_type && {
    JMATHS_FUNCTION_TO_LOG;

    return static_cast<basic_N_type &&>(*this);
}

/**
 * @brief Check if value is zero
 * @return true if magnitude is zero
 * COMPLEXITY: O(1)
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Z<BaseInt, BaseIntBig, Allocator>::is_zero() const {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::is_zero();
}

/**
 * @brief Check if value is positive one
 * @return true if magnitude is one and sign is positive
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Z<BaseInt, BaseIntBig, Allocator>::is_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return is_positive() && basic_N_type::is_one();
}

/**
 * @brief Check if value is negative one
 * @return true if magnitude is one and sign is negative
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Z<BaseInt, BaseIntBig, Allocator>::is_neg_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return is_negative() && basic_N_type::is_one();
}

/**
 * @brief Get total size in bytes
 * @return sizeof(*this) + dynamic allocation size
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::size_t basic_Z<BaseInt, BaseIntBig, Allocator>::size() const {
    JMATHS_FUNCTION_TO_LOG;

    return sizeof(*this) + dynamic_size_();
}

/**
 * @brief Convert to string in specified base
 * @param base Numeric base (2-64)
 * @return String with optional '-' prefix
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Z<BaseInt, BaseIntBig, Allocator>::to_str(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    error::invalid_base::check(base);

    return conv_to_base_(base);
}

/**
 * @brief Convert to hexadecimal string
 * @return Hex string with optional '-' prefix
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Z<BaseInt, BaseIntBig, Allocator>::to_hex() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) {
        return negative_sign + basic_N_type::to_hex();
    } else {
        return basic_N_type::to_hex();
    }
}

/**
 * @brief Convert to binary string
 * @return Binary string with optional '-' prefix
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Z<BaseInt, BaseIntBig, Allocator>::to_bin() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) {
        return negative_sign + basic_N_type::to_bin();
    } else {
        return basic_N_type::to_bin();
    }
}

/**
 * @brief Set value to zero
 * POSTCONDITION: is_zero() == true, is_positive() == true
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_Z<BaseInt, BaseIntBig, Allocator>::set_zero() {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::set_zero();
    set_sign_(positive);
}

/**
 * @brief Pre-increment operator (add 1)
 * @return Reference to *this
 *
 * ALGORITHM: Sign-aware increment
 * - Positive: Increment magnitude
 * - Negative: Decrement magnitude
 *   - If becomes zero, flip to positive
 *
 * EXAMPLES:
 * - 5 becomes 6
 * - -5 becomes -4
 * - -1 becomes 0 (sign flips to positive)
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator++() {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) {
        basic_N_type::opr_incr_();
    } else {
        basic_N_type::opr_decr_();

        if (basic_N_type::is_zero()) { set_sign_(positive); }
    }

    return *this;
}

/**
 * @brief Pre-decrement operator (subtract 1)
 * @return Reference to *this
 *
 * ALGORITHM: Sign-aware decrement
 * - Positive:
 *   - If zero, become -1 (flip sign, set magnitude to 1)
 *   - Otherwise, decrement magnitude
 * - Negative: Increment magnitude
 *
 * EXAMPLES:
 * - 5 becomes 4
 * - 0 becomes -1 (sign flips to negative)
 * - -5 becomes -6
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator--() {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) {
        if (basic_N_type::is_zero()) {
            set_sign_(negative);
            basic_N_type::opr_incr_();
        } else {
            basic_N_type::opr_decr_();
        }
    } else {
        basic_N_type::opr_incr_();
    }

    return *this;
}

/**
 * @brief Add and assign (compound addition)
 * @param rhs Right-hand side operand
 * @return Reference to *this
 *
 * ALGORITHM:
 * - Decomposes into cases based on signs of *this and rhs
 * - Uses basic_N operations for magnitude, with sign logic
 *
 * SIGN HANDLING:
 * - Both positive: basic_N addition
 * - Positive + Negative:
 *   - If |this| > |rhs|: Subtract rhs's magnitude from this
 *   - If |this| < |rhs|: Set this to rhs - this (flip sign)
 *   - If equal: Result is zero (set to zero)
 * - Negative + Positive: Same as positive + negative (commutative)
 * - Negative + Negative: Add magnitudes, keep negative sign
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator+=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_positive()) {
        if (rhs.is_positive()) {
            basic_N_type::opr_add_assign_(rhs);
        } else {
            if (const auto difference = basic_N_type::detail::opr_comp(this->abs(), rhs.abs());
                difference == 0) {
                basic_N_type::set_zero();
            } else if (difference > 0) {
                basic_N_type::opr_subtr_assign_(rhs.abs());
            } else {
                basic_N_type::operator=(basic_N_type::detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(negative);
            }
        }
    } else {
        if (rhs.is_positive()) {
            if (const auto difference = basic_N_type::detail::opr_comp(this->abs(), rhs.abs());
                difference == 0) {
                basic_Z::set_zero();
            } else if (difference > 0) {
                basic_N_type::opr_subtr_assign_(rhs.abs());
            } else {
                basic_N_type::operator=(basic_N_type::detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(positive);
            }
        } else {
            basic_N_type::opr_add_assign_(rhs);
        }
    }

    return *this;
}

/**
 * @brief Subtract and assign (compound subtraction)
 * @param rhs Right-hand side operand
 * @return Reference to *this
 *
 * ALGORITHM:
 * - Decomposes into cases based on signs of *this and rhs
 * - Uses basic_N operations for magnitude, with sign logic
 *
 * SIGN HANDLING:
 * - Both positive:
 *   - If |this| > |rhs|: Subtract rhs's magnitude from this
 *   - If |this| < |rhs|: Set this to rhs - this (flip sign)
 *   - If equal: Result is zero (set to zero)
 * - Positive + Negative: Add magnitudes, keep positive sign
 * - Negative + Positive: Add magnitudes, keep negative sign
 * - Both negative: basic_N subtraction with flipped sign
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator-=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_positive()) {
        if (rhs.is_positive()) {
            if (const auto difference = basic_N_type::detail::opr_comp(this->abs(), rhs.abs());
                difference == 0) {
                basic_N_type::set_zero();
            } else if (difference > 0) {
                basic_N_type::opr_subtr_assign_(rhs.abs());
            } else {
                basic_N_type::operator=(basic_N_type::detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(negative);
            }
        } else {
            basic_N_type::opr_add_assign_(rhs);
        }
    } else {
        if (rhs.is_positive()) {
            basic_N_type::opr_add_assign_(rhs);
        } else {
            if (const auto difference = basic_N_type::detail::opr_comp(this->abs(), rhs.abs());
                difference == 0) {
                basic_Z::set_zero();
            } else if (difference > 0) {
                basic_N_type::opr_subtr_assign_(rhs.abs());
            } else {
                basic_N_type::operator=(basic_N_type::detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(positive);
            }
        }
    }

    return *this;
}

/**
 * @brief Multiply and assign (compound multiplication)
 * @param rhs Right-hand side operand
 * @return Reference to *this
 *
 * ALGORITHM:
 * - Delegates to basic_N for multiplication
 * - Sets sign based on zero status and sign bits
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator*=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_mult_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
    return *this;
}

/**
 * @brief Bitwise AND and assign (compound operation)
 * @param rhs Right-hand side operand
 * @return Reference to *this
 *
 * ALGORITHM:
 * - Delegates to basic_N for bitwise AND
 * - Sets sign based on zero status and sign bits
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator&=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_and_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ & rhs.sign_);
    return *this;
}

/**
 * @brief Bitwise OR and assign (compound operation)
 * @param rhs Right-hand side operand
 * @return Reference to *this
 *
 * ALGORITHM:
 * - Delegates to basic_N for bitwise OR
 * - Sets sign based on zero status and sign bits
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator|=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_or_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ | rhs.sign_);
    return *this;
}

/**
 * @brief Bitwise XOR and assign (compound operation)
 * @param rhs Right-hand side operand
 * @return Reference to *this
 *
 * ALGORITHM:
 * - Delegates to basic_N for bitwise XOR
 * - Sets sign based on zero status and sign bits
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator^=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_xor_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
    return *this;
}

/**
 * @brief Unary negation (const version)
 * @return New signed integer with flipped sign
 *
 * SPECIAL CASE: -0 = 0 (positive)
 *
 * COMPLEXITY: O(n) for copying magnitude
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Z<BaseInt, BaseIntBig, Allocator>::operator-() const & -> basic_Z {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return basic_Z{}; }
    return {static_cast<const basic_N_type &>(*this), static_cast<sign_bool>(!sign_)};
}

/**
 * @brief Unary negation (rvalue version)
 * @return Moved *this with flipped sign
 *
 * OPTIMIZATION: Flips sign in-place, then moves
 *
 * COMPLEXITY: O(1)
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Z<BaseInt, BaseIntBig, Allocator>::operator-() && -> basic_Z && {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return std::move(*this); }
    flip_sign();
    return std::move(*this);
}

/**
 * @brief Unary bitwise complement
 * @return New basic_Z with bits flipped
 *
 * ALGORITHM:
 * 1. Compute bitwise complement of magnitude
 * 2. If result is zero, return positive zero
 * 3. Otherwise, return complemented value with sign flipped
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::operator~() const {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type complemented = basic_N_type::opr_compl_();

    if (complemented.is_zero()) { return basic_Z{}; }

    return {std::move(complemented), static_cast<sign_bool>(!sign_)};
}

/**
 * @brief Left bitwise shift
 * @param pos Number of positions to shift
 * @return New basic_Z shifted left
 *
 * ALGORITHM: Delegates to basic_N for bitwise shift
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::operator<<(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    return {basic_N_type::opr_bitshift_l_(pos), sign_};
}

/**
 * @brief Right bitwise shift
 * @param pos Number of positions to shift
 * @return New basic_Z shifted right
 *
 * ALGORITHM: Delegates to basic_N for bitwise shift
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::operator>>(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type shifted = basic_N_type::opr_bitshift_r_(pos);

    if (shifted.is_zero()) { return basic_Z{}; }

    return {std::move(shifted), sign_};
}

/**
 * @brief Left bitwise shift and assign
 * @param pos Number of positions to shift
 * @return Reference to *this
 *
 * ALGORITHM: Delegates to basic_N for bitwise shift
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator<<=(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_bitshift_l_assign_(pos);
    return *this;
}

/**
 * @brief Right bitwise shift and assign
 * @param pos Number of positions to shift
 * @return Reference to *this
 *
 * ALGORITHM: Delegates to basic_N for bitwise shift
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator>>=(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_bitshift_r_assign_(pos);

    if (basic_N_type::is_zero()) { set_sign_(positive); }

    return *this;
}

/**
 * @brief Assign from string view
 * @param num_str String in format "[-]digits"
 * @return Reference to *this
 *
 * ALGORITHM:
 * 1. Extract sign from string (via sign_type)
 * 2. Parse digits using basic_N
 * 3. Ensure zero is always positive
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator=(std::string_view num_str) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(sign_type::handle_string_(&num_str));
    basic_N_type::opr_assign_(num_str);
    if (is_zero()) { set_sign_(positive); }
    return *this;
}

/**
 * @brief Assign from basic_N (unsigned integer)
 * @param n Unsigned value
 * @return Reference to *this
 *
 * ALGORITHM:
 * 1. Copy magnitude from n
 * 2. Set sign to positive
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator=(const basic_N_type & n) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::operator=(n);
    set_sign_(positive);
    return *this;
}

/**
 * @brief Move assignment from basic_N (unsigned integer)
 * @param n Unsigned value (moved)
 * @return Reference to *this
 *
 * ALGORITHM:
 * 1. Move magnitude from n
 * 2. Set sign to positive
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator=(basic_N_type && n) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::operator=(std::move(n));
    set_sign_(positive);
    return *this;
}

}  // namespace jmaths
