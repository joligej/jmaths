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
#include <cstdint>
#include <format>
#include <istream>
#include <limits>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>

#include "TMP.hpp"
#include "basic_N.hpp"
#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "formatter.hpp"
#include "sign_type.hpp"

/**
 * @file basic_Q.hpp
 * @brief Arbitrary-precision rational number type (basic_Q class template)
 *
 * This file defines the basic_Q class template, which implements arbitrary-precision
 * rational numbers (fractions). Each rational is represented as numerator/denominator
 * with a separate sign.
 *
 * KEY FEATURES:
 * - Arbitrary-precision: Both numerator and denominator can be any size
 * - Automatic reduction: Fractions reduced to lowest terms using GCD
 * - Sign-magnitude: Sign stored separately, denominator always positive
 * - Floating-point interop: Can construct from and convert to float/double
 *
 * DESIGN INVARIANTS:
 * - Denominator is always positive (sign stored separately)
 * - Fractions are always in reduced form: gcd(numerator, denominator) = 1
 * - Zero is represented as 0/1 with positive sign
 *
 * TEMPLATE PARAMETERS:
 * - BaseInt: Base digit type (e.g., uint32_t)
 * - BaseIntBig: Wider type for intermediate calculations (e.g., uint64_t)
 * - Allocator: Memory allocator for digit storage
 */

// declarations of Q and associated functions and types
namespace jmaths {

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr std::ostream & operator<<(std::ostream & os, const basic_Q_type & q);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr std::istream & operator>>(std::istream & is, basic_Q_type & q);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator+(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator-(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator*(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator/(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator&(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator|(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator^(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr bool operator==(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr bool operator==(const basic_Q_type & lhs, std::floating_point auto rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr bool operator==(std::floating_point auto lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr std::strong_ordering operator<=>(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr std::strong_ordering operator<=>(const basic_Q_type & lhs, std::floating_point auto rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr std::strong_ordering operator<=>(std::floating_point auto lhs, const basic_Q_type & rhs);

/**
 * @class basic_Q
 * @brief Arbitrary-precision rational number (fraction)
 *
 * Implements rational numbers as fractions with arbitrary-precision numerator and
 * denominator. All fractions are automatically reduced to lowest terms.
 *
 * REPRESENTATION:
 * - numerator (num_): Absolute value of numerator
 * - denominator (denom_): Always positive, never zero
 * - sign (sign_): Positive or negative
 *
 * EXAMPLE: The number -3/4 is represented as:
 * - num_ = 3
 * - denom_ = 4
 * - sign_ = negative
 *
 * ARITHMETIC:
 * - Addition: (a/b) + (c/d) = (a×d + b×c) / (b×d), then reduce
 * - Multiplication: (a/b) × (c/d) = (a×c) / (b×d), then reduce
 * - Division: (a/b) / (c/d) = (a×d) / (b×c), then reduce
 *
 * COMPLEXITY: Operations involve multiple N multiplications plus GCD for reduction
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator = allocator<BaseInt>>
class basic_Q final : public sign_type {
    using basic_N_type = basic_N<BaseInt, BaseIntBig, Allocator>;
    using basic_Z_type = basic_Z<BaseInt, BaseIntBig, Allocator>;

    using base_int_type = BaseInt;
    using base_int_big_type = BaseIntBig;
    using allocator_type = Allocator;
    static constexpr base_int_type max_digit = std::numeric_limits<base_int_type>::max();
    static constexpr base_int_big_type radix = static_cast<base_int_big_type>(max_digit) + 1;
    static constexpr std::size_t base_int_type_size = sizeof(base_int_type);
    static constexpr bitcount_t base_int_type_bits = base_int_type_size * bits_in_byte;

    friend struct calc;
    friend struct std::hash<basic_Q>;

    friend struct format_output<basic_Q>;

   public:
    struct detail;

    constexpr basic_Q(const basic_Q &) = default;
    constexpr basic_Q(basic_Q &&) = default;
    constexpr basic_Q & operator=(const basic_Q &) = default;
    constexpr basic_Q & operator=(basic_Q &&) = default;

    constexpr basic_Q();
    explicit constexpr basic_Q(std::string_view num_str, unsigned base = default_base);
    constexpr basic_Q(std::floating_point auto num);

    explicit constexpr basic_Q(const basic_N_type & n);
    explicit constexpr basic_Q(basic_N_type && n);

    explicit constexpr basic_Q(const basic_Z_type & z);
    explicit constexpr basic_Q(basic_Z_type && z);

    constexpr basic_Q(const basic_N_type & num, const basic_N_type & denom);
    constexpr basic_Q(const basic_N_type & num, basic_N_type && denom);
    constexpr basic_Q(basic_N_type && num, const basic_N_type & denom);
    constexpr basic_Q(basic_N_type && num, basic_N_type && denom);

    constexpr basic_Q(const basic_Z_type & num, const basic_Z_type & denom);
    constexpr basic_Q(const basic_Z_type & num, basic_Z_type && denom);
    constexpr basic_Q(basic_Z_type && num, const basic_Z_type & denom);
    constexpr basic_Q(basic_Z_type && num, basic_Z_type && denom);

    [[nodiscard]] constexpr bool is_zero() const override;
    [[nodiscard]] constexpr bool is_one() const;
    [[nodiscard]] constexpr bool is_neg_one() const;

    [[nodiscard]] constexpr basic_Q abs() const &;
    [[nodiscard]] constexpr basic_Q && abs() &&;

    [[nodiscard]] constexpr basic_Q inverse() const &;
    [[nodiscard]] constexpr basic_Q && inverse() &&;

    [[nodiscard]] constexpr std::size_t size() const;  // size of this object in bytes

    [[nodiscard]] constexpr std::string to_str(
        unsigned base = default_base) const;  // convert to string in any base >= 2 and <= 64
    [[nodiscard]] constexpr std::string to_hex() const;  // convert to string in base 16 (assumes
                                                         // base is an integer power of 2)
    [[nodiscard]] constexpr std::string to_bin() const;  // convert to string in base 2

    explicit constexpr operator bool() const;
    template <std::floating_point T>
        requires std::numeric_limits<T>::is_iec559
    [[nodiscard]] constexpr std::optional<T> fits_into() const;

    constexpr basic_Q & operator++();
    constexpr basic_Q & operator--();

    constexpr basic_Q & operator+=(const basic_Q & rhs);
    constexpr basic_Q & operator-=(const basic_Q & rhs);
    constexpr basic_Q & operator*=(const basic_Q & rhs);
    constexpr basic_Q & operator/=(const basic_Q & rhs);

    constexpr basic_Q & operator&=(const basic_Q & rhs);
    constexpr basic_Q & operator|=(const basic_Q & rhs);
    constexpr basic_Q & operator^=(const basic_Q & rhs);

    [[nodiscard]] constexpr basic_Q operator-() const &;
    [[nodiscard]] constexpr basic_Q && operator-() &&;
    [[nodiscard]] constexpr basic_Q operator~() const;
    [[nodiscard]] constexpr basic_Q operator<<(bitcount_t pos) const;
    [[nodiscard]] constexpr basic_Q operator>>(bitcount_t pos) const;

    constexpr basic_Q & operator<<=(bitcount_t pos);
    constexpr basic_Q & operator>>=(bitcount_t pos);

    constexpr basic_Q & operator=(std::string_view num_str);
    constexpr basic_Q & operator=(std::floating_point auto rhs);

    constexpr basic_Q & operator=(const basic_N_type & n);
    constexpr basic_Q & operator=(basic_N_type && n);

    constexpr basic_Q & operator=(const basic_Z_type & z);
    constexpr basic_Q & operator=(basic_Z_type && z);

   private:
    basic_N_type num_, denom_;

    constexpr basic_Q(basic_N_type && num, basic_N_type && denom, sign_bool sign);
    constexpr basic_Q(const basic_N_type & num, const basic_N_type & denom, sign_bool sign);
    constexpr basic_Q(std::tuple<basic_N_type, basic_N_type, sign_bool> && fraction_info);

    [[nodiscard]] static constexpr std::string_view handle_fraction_string_(
        std::string_view * num_str);

    constexpr void canonicalise_();
    [[nodiscard]] constexpr std::string conv_to_base_(unsigned base = default_base) const;

    [[nodiscard]] static constexpr std::tuple<basic_N_type, basic_N_type, sign_bool> handle_float_(
        std::floating_point auto num);

    [[nodiscard]] constexpr std::size_t dynamic_size_() const;
};

}  // namespace jmaths

namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct basic_Q<BaseInt, BaseIntBig, Allocator>::detail {
    static constexpr std::ostream & opr_ins(std::ostream & os, const basic_Q & q);
    static constexpr std::istream & opr_extr(std::istream & is, basic_Q & q);

    static constexpr basic_Q opr_add(const basic_Q & lhs, const basic_Q & rhs);
    static constexpr basic_Q opr_subtr(const basic_Q & lhs, const basic_Q & rhs);
    static constexpr basic_Q opr_mult(const basic_Q & lhs, const basic_Q & rhs);
    static constexpr basic_Q opr_div(const basic_Q & lhs, const basic_Q & rhs);

    static constexpr basic_Q opr_and(const basic_Q & lhs, const basic_Q & rhs);
    static constexpr basic_Q opr_or(const basic_Q & lhs, const basic_Q & rhs);
    static constexpr basic_Q opr_xor(const basic_Q & lhs, const basic_Q & rhs);

    static constexpr bool opr_eq(const basic_Q & lhs, const basic_Q & rhs);
    static constexpr bool opr_eq(const basic_Q & lhs, std::floating_point auto rhs);

    static constexpr std::strong_ordering opr_comp(const basic_Q & lhs, const basic_Q & rhs);
    static constexpr std::strong_ordering opr_comp(const basic_Q & lhs,
                                                   std::floating_point auto rhs);
};

}  // namespace jmaths

namespace jmaths {

#if defined(UINT64_MAX) && defined(UINT32_MAX)

    #define JMATHS_TYPEDEF_Q 32
using Q = basic_Q<std::uint32_t, std::uint64_t>;

#elif defined(UINT32_MAX) && defined(UINT16_MAX)

    #define JMATHS_TYPEDEF_Q 16
using Q = basic_Q<std::uint16_t, std::uint32_t>;

#elif defined(UINT16_MAX) && defined(UINT8_MAX)

    #define JMATHS_TYPEDEF_Q 8
using Q = basic_Q<std::uint8_t, std::uint16_t>;

#endif

}  // namespace jmaths

template <jmaths::TMP::instance_of<jmaths::basic_Q> basic_Q_type>
struct std::formatter<basic_Q_type> : jmaths::format_output<basic_Q_type> {};

#include "basic_Q_detail_impl.hpp"
#include "basic_Q_impl.hpp"
