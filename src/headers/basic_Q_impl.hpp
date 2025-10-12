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

#include <algorithm>
#include <bit>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <limits>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "basic_N.hpp"
#include "basic_Q.hpp"
#include "basic_Z.hpp"
#include "calc.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "error.hpp"
#include "sign_type.hpp"

/**
 * @file basic_Q_impl.hpp
 * @brief Implementation of basic_Q member functions and operators
 *
 * This file contains the implementation of arbitrary-precision rational number
 * operations including construction, arithmetic, comparison, and type conversions.
 *
 * KEY ALGORITHMS:
 * - Floating-point conversion using frexp/scalbn for accurate fraction extraction
 * - Automatic fraction reduction using GCD after each operation
 * - Sign management following rational arithmetic rules
 * - Compound assignment operators with in-place optimization
 */

// comparison functions for Q with floating point types
namespace jmaths {

/**
 * @brief Equality comparison with floating-point (implementation detail)
 * @param lhs Rational number
 * @param rhs Floating-point number
 * @return true if equal after converting rhs to Q
 *
 * ALGORITHM: Converts floating-point to Q, then compares
 * This ensures exact comparison using rational arithmetic
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_eq(
    const basic_Q & lhs,
    std::floating_point auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return lhs == basic_Q{rhs};
}

/**
 * @brief Three-way comparison with floating-point (implementation detail)
 * @param lhs Rational number
 * @param rhs Floating-point number
 * @return Ordering relationship
 *
 * ALGORITHM: Converts floating-point to Q, then compares
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::strong_ordering basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_comp(
    const basic_Q & lhs,
    std::floating_point auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return lhs <=> basic_Q{rhs};
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr bool operator==(const basic_Q_type & lhs, std::floating_point auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_eq(lhs, rhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr bool operator==(std::floating_point auto lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_eq(rhs, lhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr std::strong_ordering operator<=>(const basic_Q_type & lhs, std::floating_point auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_comp(lhs, rhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr std::strong_ordering operator<=>(std::floating_point auto lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return 0 <=> basic_Q_type::detail::opr_comp(rhs, lhs);
}

}  // namespace jmaths

// member function templates of Q
namespace jmaths {

/**
 * @brief Convert floating-point number to rational representation
 * @param num Floating-point value (float or double)
 * @return Tuple of (numerator, denominator, sign)
 *
 * ALGORITHM: IEEE 754 decomposition
 * 1. Use std::frexp to extract mantissa and exponent
 * 2. Scale mantissa to integer by multiplying with 2^(mantissa_bits)
 * 3. Compute denominator as 2^(-adjusted_exponent)
 * 4. Extract sign and make numerator positive
 *
 * EXAMPLE: 3.5 = 7/2 in base 2
 * - frexp(3.5) = (0.875, 2)
 * - mantissa = 0.875 * 2^53 = integer
 * - exponent adjusted = 2 - 53 = -51
 * - denominator = 2^51
 *
 * PRECISION: Maintains exact representation for values that fit in mantissa
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::handle_float_(std::floating_point auto num)
    -> std::tuple<basic_N_type, basic_N_type, sign_bool> {
    JMATHS_FUNCTION_TO_LOG;

    using floating_point_type = decltype(num);

    // Extract mantissa and exponent using IEEE 754 decomposition
    int exponent;
    floating_point_type significant_part = std::frexp(num, &exponent);

    // Scale mantissa to full integer precision
    significant_part =
        std::scalbn(significant_part, std::numeric_limits<floating_point_type>::digits);
    exponent -= std::numeric_limits<floating_point_type>::digits;

    // Convert to integers
    auto numerator = std::llrint(significant_part);
    auto denominator = std::llrint(std::exp2(-exponent));

    // Extract and handle sign
    const auto sign = (numerator < 0) ? sign_type::negative : sign_type::positive;

    if (sign == sign_type::negative) numerator *= -1;

    // it is assumed here that exponent is negative at this point

    return {static_cast<std::make_unsigned_t<decltype(numerator)>>(numerator),
            static_cast<std::make_unsigned_t<decltype(denominator)>>(denominator),
            sign};
}

/**
 * @brief Construct from floating-point number
 * @param num Float or double to convert
 *
 * ALGORITHM: Uses handle_float_ to decompose IEEE 754 representation
 * The result is automatically reduced to lowest terms by canonicalise_()
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(std::floating_point auto num) :
    basic_Q(handle_float_(num)) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Convert rational to floating-point if possible
 * @tparam T Target floating-point type (float or double)
 * @return std::optional<T> containing value, or nullopt if overflow/underflow
 *
 * ALGORITHM: Direct rational-to-float conversion with exponent adjustment
 * 1. Convert numerator and denominator to floating-point (up to mantissa bits)
 * 2. Compute initial ratio: result = numerator / denominator
 * 3. Adjust exponent based on difference in digit counts
 * 4. Check for overflow (infinity) and underflow (subnormals/zero)
 * 5. Set sign bit appropriately
 *
 * COMPLEXITY: O(n) where n = min(num_digits, mantissa_bits)
 *
 * EDGE CASES:
 * - Overflow: Returns infinity if available, else nullopt
 * - Underflow: Returns nullopt (subnormals not fully supported yet)
 * - Exact values: Preserved when within mantissa precision
 *
 * NOTE: Marked FIXME - subnormal handling incomplete
 */
// FIXME:
template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <std::floating_point T>
    requires std::numeric_limits<T>::is_iec559
constexpr std::optional<T> basic_Q<BaseInt, BaseIntBig, Allocator>::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    using nlf = std::numeric_limits<T>;

    static constexpr bool is_float = std::same_as<T, float>;
    static constexpr bool is_double = std::same_as<T, double>;

#if 0
    static constexpr bool is_long_double = std::same_as<T, long double>;
#endif

    static constexpr bool is_allowed_type = is_float || is_double;

    static_assert(is_allowed_type,
                  "Template type parameter is not one of the allowed types: "
                  "float and double.");
    static_assert(nlf::radix == 2,
                  "The radix of the floating point type is currently not supported. Please make "
                  "sure it is equal to 2.");

    static constexpr bool is_big_endian = std::endian::native == std::endian::big;
    static constexpr bool is_little_endian = std::endian::native == std::endian::little;

    static_assert(
        is_big_endian || is_little_endian,
        "Mixed endianness is not supported. Unclear how to implement floating point manipulation.");

    struct float_sizes {
        enum : bitcount_t { sign = 1, exponent = 8, mantissa = 23 };

        struct big_endian {
            std::uint32_t sign : float_sizes::sign;
            std::uint32_t exponent : float_sizes::exponent;
            std::uint32_t mantissa : float_sizes::mantissa;
        };

        struct little_endian {
            std::uint32_t mantissa : float_sizes::mantissa;
            std::uint32_t exponent : float_sizes::exponent;
            std::uint32_t sign : float_sizes::sign;
        };
    };

    struct double_sizes {
        enum : bitcount_t { sign = 1, exponent = 11, mantissa = 52 };

        struct big_endian {
            std::uint64_t sign : double_sizes::sign;
            std::uint64_t exponent : double_sizes::exponent;
            std::uint64_t mantissa : double_sizes::mantissa;
        };

        struct little_endian {
            std::uint64_t mantissa : double_sizes::mantissa;
            std::uint64_t exponent : double_sizes::exponent;
            std::uint64_t sign : double_sizes::sign;
        };
    };

    union float_access {
        float val;

        std::conditional_t<is_big_endian,
                           typename float_sizes::big_endian,
                           typename float_sizes::little_endian>
            fields;
    };

    union double_access {
        double val;

        std::conditional_t<is_big_endian,
                           typename double_sizes::big_endian,
                           typename double_sizes::little_endian>
            fields;
    };

#if 0
  union long_double_access {
  long double val;
  struct {
    #ifdef NATIVELY_BIG_ENDIAN
  std::uint64_t sign : 1;
  std::uint64_t exponent : 15;
  std::uint64_t mantissa : 112;
    #else
  std::uint64_t mantissa : 112;
  std::uint64_t exponent : 15;
  std::uint64_t sign : 1;
    #endif
  } fields;
  };
#endif

    static_assert(
        sizeof(float_access) == sizeof(float) && sizeof(float_access) == sizeof(std::uint32_t[1]),
        "There seems to be a problem with the padding bits for type: "
        "float_access.");
    static_assert(
        sizeof(double_access) == sizeof(double) && sizeof(double_access) == sizeof(std::uint64_t[1]),
        "There seems to be a problem with the padding bits for type: "
        "double_access.");

#if 0
  static_assert(sizeof(long_double_access) == sizeof(long double) && sizeof(long_double_access) == sizeof(std::uint64_t[2]), "There seems to be a problem with the padding bits for type: long_double_access.");
#endif

    using access_type = std::conditional_t<is_float, float_access, double_access>;
    using sizes_type = std::conditional_t<is_float, float_sizes, double_sizes>;

    // Handle special cases for efficiency
    if (num_.is_zero()) return 0;
    if (is_one()) return 1;
    if (is_neg_one()) return -1;

    // Convert numerator: extract up to sizeof(T)/base_int_type_size digits
    T numerator{};

    {
        std::size_t i = 0U;

        // Process from most significant digit
        for (auto crit = num_.digits_.crbegin();
             crit != num_.digits_.crend() && i < sizeof(T) / base_int_type_size;
             ++crit, ++i) {
            numerator = numerator * radix + *crit;
        }

        // Fill remaining positions with zeros (scale up)
        for (; i < sizeof(T) / base_int_type_size; ++i) {
            numerator *= radix;
        }
    }

    // Convert denominator similarly
    T denominator{};

    {
        std::size_t j = 0;

        for (auto crit = denom_.digits_.crbegin();
             crit != denom_.digits_.crend() && j < sizeof(T) / base_int_type_size;
             ++crit, ++j) {
            denominator = denominator * radix + *crit;
        }

        for (; j < sizeof(T) / base_int_type_size; ++j) {
            denominator *= radix;
        }
    }

    // Compute initial floating-point result
    access_type result = {.val = numerator / denominator};

    // Adjust exponent based on digit count difference
    if (num_.digits_.size() < denom_.digits_.size()) {
        // Numerator smaller: decrease exponent (smaller result)
        static constexpr std::uint16_t min_exponent = 1U;

        // Check for underflow
        if (result.fields.exponent <
            min_exponent + (denom_.digits_.size() - num_.digits_.size()) * base_int_type_bits) {
#if 0
  if (*this >= Q{nlf::min()}) {
  return nlf::min();
  } else {
  return std::nullopt;
  }
#endif
            // ^^^ doesn't yet take care of subnormals
            return std::nullopt;
        }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
        result.fields.exponent -= (denom_.digits_.size() - num_.digits_.size()) * base_int_type_bits;
#pragma GCC diagnostic pop

    } else {
        // Numerator larger or equal: increase exponent (larger result)
        static constexpr std::uint32_t max_exponent =
            ~(~static_cast<std::uint32_t>(0) << sizes_type::exponent) - 1;

        // Check for overflow
        if ((num_.digits_.size() - denom_.digits_.size()) * base_int_type_bits >
            max_exponent - result.fields.exponent) {
            if constexpr (nlf::has_infinity) {
                return nlf::infinity();
            } else {
                return std::nullopt;
            }
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
        result.fields.exponent += (num_.digits_.size() - denom_.digits_.size()) * base_int_type_bits;
#pragma GCC diagnostic pop
    }

    // Set sign bit
    result.fields.sign = is_negative();

    return result.val;
}

/**
 * @brief Assign from floating-point number
 * @param rhs Float or double to assign
 * @return Reference to *this
 *
 * ALGORITHM:
 * 1. Convert floating-point to (numerator, denominator, sign) tuple
 * 2. Assign components
 * 3. Reduce to lowest terms via canonicalise_()
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator=(std::floating_point auto rhs)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    auto fraction_info = handle_float_(rhs);
    num_ = std::move(std::get<0>(fraction_info));
    denom_ = std::move(std::get<1>(fraction_info));
    set_sign_(std::get<2>(fraction_info));

    canonicalise_();

    return *this;
}

}  // namespace jmaths

// binary operators for Q
namespace jmaths {
/**********************************************************/
// forwarding functions

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr std::ostream & operator<<(std::ostream & os, const basic_Q_type & q) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_ins(os, q);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr std::istream & operator>>(std::istream & is, basic_Q_type & q) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_extr(is, q);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator+(const basic_Q_type & lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_add(lhs, rhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator-(const basic_Q_type & lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_subtr(lhs, rhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator*(const basic_Q_type & lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_mult(lhs, rhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator/(const basic_Q_type & lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(rhs);
    return basic_Q_type::detail::opr_div(lhs, rhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator&(const basic_Q_type & lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_and(lhs, rhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator|(const basic_Q_type & lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_or(lhs, rhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr basic_Q_type operator^(const basic_Q_type & lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_xor(lhs, rhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr bool operator==(const basic_Q_type & lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_eq(lhs, rhs);
}

template <TMP::instance_of<basic_Q> basic_Q_type>
constexpr std::strong_ordering operator<=>(const basic_Q_type & lhs, const basic_Q_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Q_type::detail::opr_comp(lhs, rhs);
}

}  // namespace jmaths

// member functions of Q
namespace jmaths {

/**
 * @brief Private constructor with explicit sign
 * @param num Numerator (moved)
 * @param denom Denominator (moved)
 * @param sign Sign of the rational
 *
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(basic_N_type && num,
                                                           basic_N_type && denom,
                                                           sign_bool sign) :
    sign_type(sign), num_(std::move(num)), denom_(std::move(denom)) {
    JMATHS_FUNCTION_TO_LOG;

    canonicalise_();
}

/**
 * @brief Construct from numerator, denominator, and sign
 * @param num Numerator (const reference)
 * @param denom Denominator (const reference)
 * @param sign Sign of the rational
 *
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(const basic_N_type & num,
                                                           const basic_N_type & denom,
                                                           sign_bool sign) :
    sign_type(sign), num_(num), denom_(denom) {
    JMATHS_FUNCTION_TO_LOG;

    canonicalise_();
}

/**
 * @brief Construct from fraction info tuple
 * @param fraction_info Tuple of (numerator, denominator, sign)
 *
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(
    std::tuple<basic_N_type, basic_N_type, sign_bool> && fraction_info) :
    basic_Q(std::move(std::get<0>(fraction_info)),
            std::move(std::get<1>(fraction_info)),
            std::get<2>(fraction_info)) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Extract numerator from fraction string
 * @param num_str Pointer to string being parsed (modified)
 * @return String view of numerator part
 *
 * ALGORITHM:
 * 1. Find position of vinculum (fraction bar '/')
 * 2. Extract substring before vinculum as numerator
 * 3. Remove prefix from num_str, leaving denominator
 *
 * EXAMPLE: "3/4" -> returns "3", num_str becomes "4"
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string_view basic_Q<BaseInt, BaseIntBig, Allocator>::handle_fraction_string_(
    std::string_view * num_str) {
    JMATHS_FUNCTION_TO_LOG;

    const auto fraction_bar = num_str->find(vinculum);
    const std::string_view numerator = num_str->substr(0U, fraction_bar);
    num_str->remove_prefix(fraction_bar + 1U);
    return numerator;
}

/**
 * @brief Reduce fraction to lowest terms
 *
 * ALGORITHM: Euclidean reduction
 * 1. Compute g = gcd(numerator, denominator)
 * 2. Divide both by g: num = num/g, denom = denom/g
 *
 * POSTCONDITION: gcd(num_, denom_) = 1 (coprime)
 *
 * COMPLEXITY: O(n log n) from GCD calculation
 *
 * INVARIANT MAINTAINED: All Q operations call this to keep fractions reduced
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_Q<BaseInt, BaseIntBig, Allocator>::canonicalise_() {
    JMATHS_FUNCTION_TO_LOG;

    const basic_N_type gcd = calc::gcd(num_, denom_);
    num_ = basic_N_type::detail::opr_div(num_, gcd).first;
    denom_ = basic_N_type::detail::opr_div(denom_, gcd).first;
}

/**
 * @brief Convert to string in specified base
 * @param base Numeric base (2-64)
 * @return String representation "[-]numerator/denominator"
 *
 * FORMAT:
 * - Positive: "3/4"
 * - Negative: "-3/4"
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Q<BaseInt, BaseIntBig, Allocator>::conv_to_base_(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) { return num_.conv_to_base_(base) + vinculum + denom_.conv_to_base_(base); }

    return negative_sign + num_.conv_to_base_(base) + vinculum + denom_.conv_to_base_(base);
}

/**
 * @brief Get size in bytes for dynamic memory allocation
 * @return Total size in bytes
 *
 * ALGORITHM: Sum sizes of numerator and denominator components
 *
 * COMPLEXITY: O(n) where n = num_.digits_.size() + denom_.digits_.size()
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::size_t basic_Q<BaseInt, BaseIntBig, Allocator>::dynamic_size_() const {
    JMATHS_FUNCTION_TO_LOG;

    return num_.dynamic_size_() + denom_.dynamic_size_();
}

/**
 * @brief Default constructor - creates zero (0/1)
 * POSTCONDITION: is_zero() == true, denom_ == 1
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q() : denom_(basic_N_type::one_) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Construct from string in format "[-]numerator/denominator"
 * @param num_str String representation
 * @param base Numeric base (default 10)
 * @throws error::division_by_zero if denominator is zero
 *
 * ALGORITHM:
 * 1. Extract sign from beginning of string
 * 2. Parse numerator (up to '/')
 * 3. Parse denominator (after '/')
 * 4. Reduce to lowest terms
 * 5. Ensure zero is always positive
 *
 * EXAMPLE: "-3/4" creates rational -3/4
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(std::string_view num_str, unsigned base) :
    sign_type(&num_str), num_(handle_fraction_string_(&num_str), base), denom_(num_str, base) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(denom_, "Denominator cannot be zero!");

    canonicalise_();
    if (basic_Q::is_zero()) { set_sign_(positive); }
}

/**
 * @brief Construct from basic_N (integer) value
 * @param n Integer value
 *
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(const basic_N_type & n) :
    num_(n), denom_(basic_N_type::one_) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Construct from moved basic_N (integer) value
 * @param n Integer value (moved)
 *
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(basic_N_type && n) :
    num_(std::move(n)), denom_(basic_N_type::one_) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Construct from basic_Z (integer with sign) value
 * @param z Integer value with sign
 *
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(const basic_Z_type & z) :
    sign_type(z.sign_), num_(z.abs()), denom_(basic_N_type::one_) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Construct from moved basic_Z (integer with sign) value
 * @param z Integer value with sign (moved)
 *
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(basic_Z_type && z) :
    sign_type(z.sign_), num_(std::move(std::move(z).abs())), denom_(basic_N_type::one_) {
    JMATHS_FUNCTION_TO_LOG;
}

/**
 * @brief Construct from numerator and denominator
 * @param num Numerator
 * @param denom Denominator
 *
 * ALGORITHM: Checks denominator for zero, then assigns num_ and denom_
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(const basic_N_type & num,
                                                           const basic_N_type & denom) :
    num_(num), denom_(denom) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(denom_, "Denominator cannot be zero!");

    canonicalise_();
}

/**
 * @brief Construct from numerator and moved denominator
 * @param num Numerator
 * @param denom Denominator (moved)
 *
 * ALGORITHM: Checks denominator for zero, then assigns num_ and denom_
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(const basic_N_type & num,
                                                           basic_N_type && denom) :
    num_(num), denom_(std::move(denom)) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(denom_, "Denominator cannot be zero!");

    canonicalise_();
}

/**
 * @brief Construct from moved numerator and denominator
 * @param num Numerator (moved)
 * @param denom Denominator
 *
 * ALGORITHM: Checks denominator for zero, then assigns num_ and denom_
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(basic_N_type && num,
                                                           const basic_N_type & denom) :
    num_(std::move(num)), denom_(denom) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(denom_, "Denominator cannot be zero!");

    canonicalise_();
}

/**
 * @brief Construct from moved numerator and denominator
 * @param num Numerator (moved)
 * @param denom Denominator (moved)
 *
 * ALGORITHM: Checks denominator for zero, then assigns num_ and denom_
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(basic_N_type && num,
                                                           basic_N_type && denom) :
    num_(std::move(num)), denom_(std::move(denom)) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(denom_, "Denominator cannot be zero!");

    canonicalise_();
}

/**
 * @brief Construct from two basic_Z (signed integers)
 * @param num Numerator (signed)
 * @param denom Denominator (signed)
 *
 * ALGORITHM: Extracts absolute values and computes sign from XOR
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(const basic_Z_type & num,
                                                           const basic_Z_type & denom) :
    sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)),
    num_(num.abs()),
    denom_(denom.abs()) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(denom_, "Denominator cannot be zero!");

    canonicalise_();
}

/**
 * @brief Construct from basic_Z (signed integer) and moved denominator
 * @param num Numerator (signed)
 * @param denom Denominator (signed, moved)
 *
 * ALGORITHM: Extracts absolute value of num, and computes sign from XOR
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(const basic_Z_type & num,
                                                           basic_Z_type && denom) :
    sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)),
    num_(num.abs()),
    denom_(std::move(std::move(denom).abs())) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(denom_, "Denominator cannot be zero!");

    canonicalise_();
}

/**
 * @brief Construct from moved numerator (signed) and denominator (signed)
 * @param num Numerator (signed, moved)
 * @param denom Denominator (signed)
 *
 * ALGORITHM: Extracts absolute value of num, and computes sign from XOR
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(basic_Z_type && num,
                                                           const basic_Z_type & denom) :
    sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)),
    num_(std::move(std::move(num).abs())),
    denom_(denom.abs()) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(denom_, "Denominator cannot be zero!");

    canonicalise_();
}

/**
 * @brief Construct from moved numerator and denominator (signed)
 * @param num Numerator (signed, moved)
 * @param denom Denominator (signed, moved)
 *
 * ALGORITHM: Extracts absolute values of num and denom, and computes sign from XOR
 * POSTCONDITION: Fraction is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::basic_Q(basic_Z_type && num,
                                                           basic_Z_type && denom) :
    sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)),
    num_(std::move(std::move(num).abs())),
    denom_(std::move(std::move(denom).abs())) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(denom_, "Denominator cannot be zero!");

    canonicalise_();
}

/**
 * @brief Check if the rational number is zero
 * @return true if numerator is zero
 *
 * ALGORITHM: Simple check on num_.is_zero()
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Q<BaseInt, BaseIntBig, Allocator>::is_zero() const {
    JMATHS_FUNCTION_TO_LOG;

    return num_.is_zero();
}

/**
 * @brief Check if the rational number is one
 * @return true if numerator and denominator are both one
 *
 * ALGORITHM: Checks num_.is_one() and denom_.is_one()
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Q<BaseInt, BaseIntBig, Allocator>::is_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return is_positive() && num_.is_one() && denom_.is_one();
}

/**
 * @brief Check if the rational number is negative one
 * @return true if numerator and denominator are both one, and sign is negative
 *
 * ALGORITHM: Checks num_.is_one(), denom_.is_one(), and is_negative()
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Q<BaseInt, BaseIntBig, Allocator>::is_neg_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return is_negative() && num_.is_one() && denom_.is_one();
}

/**
 * @brief Get absolute value (const lvalue reference)
 * @return Positive basic_Q with same value
 *
 * ALGORITHM: Returns new basic_Q with sign set to positive
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::abs() const & -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    return {num_, denom_, positive};
}

/**
 * @brief Get absolute value (rvalue reference)
 * @return Moved basic_Q with sign set to positive
 *
 * ALGORITHM: Sets sign to positive, then moves *this
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::abs() && -> basic_Q && {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(positive);
    return std::move(*this);
}

/**
 * @brief Get multiplicative inverse (const lvalue reference)
 * @return basic_Q representing 1/this
 *
 * ALGORITHM: Returns new basic_Q with numerator and denominator swapped
 * Checks for zero to prevent division by zero
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::inverse() const & -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(num_, "Cannot take the inverse of zero!");

    return {denom_, num_, sign_};
}

/**
 * @brief Get multiplicative inverse (rvalue reference)
 * @return Moved basic_Q with numerator and denominator swapped
 *
 * ALGORITHM: Swaps num_ and denom_, then moves *this
 * Checks for zero to prevent division by zero
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::inverse() && -> basic_Q && {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(num_, "Cannot take the inverse of zero!");

    std::swap(num_, denom_);
    return std::move(*this);
}

/**
 * @brief Get size of the basic_Q object in bytes
 * @return Size in bytes
 *
 * ALGORITHM: Adds up sizes of num_, denom_, and sign_
 *
 * COMPLEXITY: O(1)
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::size_t basic_Q<BaseInt, BaseIntBig, Allocator>::size() const {
    JMATHS_FUNCTION_TO_LOG;

    return sizeof(*this) + dynamic_size_();
}

/**
 * @brief Convert to string in specified base
 * @param base Numeric base (2-64)
 * @return String representation "[-]numerator/denominator"
 *
 * FORMAT:
 * - Positive: "3/4"
 * - Negative: "-3/4"
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Q<BaseInt, BaseIntBig, Allocator>::to_str(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    error::invalid_base::check(base);

    return conv_to_base_(base);
}

/**
 * @brief Convert to hexadecimal string
 * @return Hexadecimal representation "[-]numerator/denominator"
 *
 * ALGORITHM: Uses to_base_ with base 16
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Q<BaseInt, BaseIntBig, Allocator>::to_hex() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) { return num_.to_hex() + vinculum + denom_.to_hex(); }

    return negative_sign + num_.to_hex() + vinculum + denom_.to_hex();
}

/**
 * @brief Convert to binary string
 * @return Binary representation "[-]numerator/denominator"
 *
 * ALGORITHM: Uses to_base_ with base 2
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Q<BaseInt, BaseIntBig, Allocator>::to_bin() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) { return num_.to_bin() + vinculum + denom_.to_bin(); }

    return negative_sign + num_.to_bin() + vinculum + denom_.to_bin();
}

/**
 * @brief Conversion to bool
 * @return true if rational is non-zero
 *
 * ALGORITHM: Returns !is_zero()
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Q<BaseInt, BaseIntBig, Allocator>::operator bool() const {
    JMATHS_FUNCTION_TO_LOG;

    return !is_zero();
}

/**
 * @brief Pre-increment operator (add 1)
 * @return Reference to *this
 *
 * ALGORITHM: Adds denominator to numerator
 * - Positive: num += denom (makes larger)
 * - Negative: |num| -= denom (makes closer to zero)
 * - Handles sign flip when crossing zero
 *
 * EXAMPLE: 3/4 becomes 7/4, -1/4 becomes 3/4
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator++() -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) {
        num_.opr_add_assign_(denom_);
    } else {
        if (const auto difference = basic_N_type::detail::opr_comp(num_, denom_); difference == 0) {
            num_.set_zero();
            set_sign_(positive);
        } else if (difference > 0) {
            num_.opr_subtr_assign_(denom_);
        } else {
            num_ = basic_N_type::detail::opr_subtr(denom_, num_);
            set_sign_(positive);
        }
    }

    return *this;
}

/**
 * @brief Pre-decrement operator (subtract 1)
 * @return Reference to *this
 *
 * ALGORITHM: Subtracts denominator from numerator
 * - Positive: num -= denom (makes smaller)
 * - Negative: |num| += denom (makes more negative)
 * - Handles sign flip when crossing zero
 *
 * EXAMPLE: 3/4 becomes -1/4, -3/4 becomes -7/4
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator--() -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) {
        if (const auto difference = basic_N_type::detail::opr_comp(num_, denom_); difference == 0) {
            num_.set_zero();
        } else if (difference > 0) {
            num_.opr_subtr_assign_(denom_);
        } else {
            num_ = basic_N_type::detail::opr_subtr(denom_, num_);
            set_sign_(negative);
        }

    } else {
        num_.opr_add_assign_(denom_);
    }

    return *this;
}

/**
 * @brief Compound addition assignment
 * @param rhs Rational to add
 * @return Reference to *this
 *
 * ALGORITHM: In-place rational addition
 * Formula: this = (num*rhs.denom ± denom*rhs.num) / (denom*rhs.denom)
 *
 * FOUR CASES based on signs (±):
 * 1. (+) += (+): Add cross products
 * 2. (+) += (-): Subtract, keep sign of larger magnitude
 * 3. (-) += (+): Subtract, keep sign of larger magnitude
 * 4. (-) += (-): Add cross products (both negative)
 *
 * OPTIMIZATION: Modifies in-place to avoid temporary allocations
 * POSTCONDITION: Result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator+=(const basic_Q & rhs)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_positive()) {
        if (rhs.is_positive()) {
            // Case 1: positive + positive
            basic_N_type first_product = basic_N_type::detail::opr_mult(num_, rhs.denom_);
            first_product.opr_add_assign_(basic_N_type::detail::opr_mult(denom_, rhs.num_));
            num_ = std::move(first_product);
        } else {
            // Case 2: positive + negative = positive - positive
            basic_N_type first_product = basic_N_type::detail::opr_mult(num_, rhs.denom_);
            basic_N_type second_product = basic_N_type::detail::opr_mult(denom_, rhs.num_);

            if (const auto difference =
                    basic_N_type::detail::opr_comp(first_product, second_product);
                difference == 0) {
                // Results cancel to zero
                num_.set_zero();
                denom_ = basic_N_type::one_;
                return *this;
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                num_ = std::move(first_product);
            } else {
                second_product.opr_subtr_assign_(first_product);
                num_ = std::move(second_product);
                set_sign_(negative);
            }
        }
    } else {
        if (rhs.is_positive()) {
            // Case 3: negative + positive = -(|lhs| - |rhs|)
            basic_N_type first_product = basic_N_type::detail::opr_mult(num_, rhs.denom_);
            basic_N_type second_product = basic_N_type::detail::opr_mult(denom_, rhs.num_);

            if (const auto difference =
                    basic_N_type::detail::opr_comp(first_product, second_product);
                difference == 0) {
                num_.set_zero();
                denom_ = basic_N_type::one_;
                set_sign_(positive);
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                num_ = std::move(first_product);
            } else {
                second_product.opr_subtr_assign_(first_product);
                num_ = std::move(second_product);
                set_sign_(positive);
            }
        } else {
            // Case 4: negative + negative
            basic_N_type first_product = basic_N_type::detail::opr_mult(num_, rhs.denom_);
            first_product.opr_add_assign_(basic_N_type::detail::opr_mult(denom_, rhs.num_));
            num_ = std::move(first_product);
        }
    }

    denom_.opr_mult_assign_(rhs.denom_);

    canonicalise_();

    return *this;
}

/**
 * @brief Compound subtraction assignment
 * @param rhs Rational to subtract
 * @return Reference to *this
 *
 * ALGORITHM: Similar to +=, but with negated rhs
 * Formula: this = (num*rhs.denom - denom*rhs.num) / (denom*rhs.denom)
 *
 * OPTIMIZATION: In-place modification
 * POSTCONDITION: Result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator-=(const basic_Q & rhs)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_positive()) {
        if (rhs.is_positive()) {
            // Case 1: positive - positive
            basic_N_type first_product = basic_N_type::detail::opr_mult(num_, rhs.denom_);
            basic_N_type second_product = basic_N_type::detail::opr_mult(denom_, rhs.num_);

            if (const auto difference =
                    basic_N_type::detail::opr_comp(first_product, second_product);
                difference == 0) {
                num_.set_zero();
                denom_ = basic_N_type::one_;
                return *this;
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                num_ = std::move(first_product);
            } else {
                second_product.opr_subtr_assign_(first_product);
                num_ = std::move(second_product);
                set_sign_(negative);
            }
        } else {
            // Case 2: positive - negative = positive + positive
            basic_N_type first_product = basic_N_type::detail::opr_mult(num_, rhs.denom_);
            first_product.opr_add_assign_(basic_N_type::detail::opr_mult(denom_, rhs.num_));
            num_ = std::move(first_product);
        }
    } else {
        if (rhs.is_positive()) {
            // Case 3: negative - positive = -(|lhs| + |rhs|)
            basic_N_type first_product = basic_N_type::detail::opr_mult(num_, rhs.denom_);
            basic_N_type second_product = basic_N_type::detail::opr_mult(denom_, rhs.num_);

            if (const auto difference =
                    basic_N_type::detail::opr_comp(first_product, second_product);
                difference == 0) {
                num_.set_zero();
                denom_ = basic_N_type::one_;
                set_sign_(positive);
            } else if (difference > 0) {
                first_product.opr_add_assign_(second_product);
                num_ = std::move(first_product);
            } else {
                second_product.opr_add_assign_(first_product);
                num_ = std::move(second_product);
                set_sign_(positive);
            }
        } else {
            // Case 4: negative - negative
            basic_N_type first_product = basic_N_type::detail::opr_mult(num_, rhs.denom_);
            first_product.opr_subtr_assign_(basic_N_type::detail::opr_mult(denom_, rhs.num_));
            num_ = std::move(first_product);
        }
    }

    denom_.opr_mult_assign_(rhs.denom_);

    canonicalise_();

    return *this;
}

/**
 * @brief Compound multiplication assignment
 * @param rhs Rational to multiply by
 * @return Reference to *this
 *
 * ALGORITHM: Rational multiplication
 * Formula: (a/b) *= (c/d) => (a*c) / (b*d)
 * Sign: XOR of signs
 *
 * COMPLEXITY: O(n²) for multiplications + O(n log n) for GCD
 * POSTCONDITION: Result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator*=(const basic_Q & rhs)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    num_.opr_mult_assign_(rhs.num_);
    denom_.opr_mult_assign_(rhs.denom_);

    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);

    canonicalise_();

    return *this;
}

/**
 * @brief Compound division assignment
 * @param rhs Rational to divide by
 * @return Reference to *this
 * @throws error::division_by_zero if rhs is zero
 *
 * ALGORITHM: Invert and multiply
 * Formula: (a/b) /= (c/d) => (a*d) / (b*c)
 *
 * POSTCONDITION: Result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator/=(const basic_Q & rhs)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(rhs);

    num_.opr_mult_assign_(rhs.denom_);
    denom_.opr_mult_assign_(rhs.num_);

    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);

    canonicalise_();

    return *this;
}

/**
 * @brief Compound bitwise AND assignment
 * @param rhs Rational to AND with
 * @return Reference to *this
 *
 * ALGORITHM: Bitwise AND on numerator and denominator
 *
 * POSTCONDITION: Result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator&=(const basic_Q & rhs)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    denom_.opr_and_assign_(rhs.denom_);

    error::division_by_zero::check(denom_);

    num_.opr_and_assign_(rhs.num_);

    set_sign_(is_zero() ? positive : this->sign_ & rhs.sign_);

    canonicalise_();

    return *this;
}

/**
 * @brief Compound bitwise OR assignment
 * @param rhs Rational to OR with
 * @return Reference to *this
 *
 * ALGORITHM: Bitwise OR on numerator and denominator
 *
 * POSTCONDITION: Result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator|=(const basic_Q & rhs)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    num_.opr_or_assign_(rhs.num_);
    denom_.opr_or_assign_(rhs.denom_);

    set_sign_(is_zero() ? positive : this->sign_ | rhs.sign_);

    canonicalise_();

    return *this;
}

/**
 * @brief Compound bitwise XOR assignment
 * @param rhs Rational to XOR with
 * @return Reference to *this
 *
 * ALGORITHM: Bitwise XOR on numerator and denominator
 *
 * POSTCONDITION: Result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator^=(const basic_Q & rhs)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    denom_.opr_xor_assign_(rhs.denom_);

    error::division_by_zero::check(denom_);

    num_.opr_xor_assign_(rhs.num_);

    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);

    canonicalise_();

    return *this;
}

/**
 * @brief Unary negation (const version)
 * @return New rational with flipped sign
 *
 * SPECIAL CASE: -0 = 0 (positive)
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator-() const & -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return *this; }
    return {num_, denom_, static_cast<sign_bool>(!sign_)};
}

/**
 * @brief Unary negation (rvalue version)
 * @return Moved *this with flipped sign
 *
 * OPTIMIZATION: Modifies and moves existing object instead of copying
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator-() && -> basic_Q && {
    JMATHS_FUNCTION_TO_LOG;

    flip_sign();
    return std::move(*this);
}

/**
 * @brief Bitwise complement
 * @return New rational with complemented numerator and denominator
 * @throws error::division_by_zero if complemented denominator is zero
 *
 * NOTE: Bitwise operations on rationals have limited mathematical meaning
 * Included for completeness and potential bit manipulation use cases
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator~() const -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type num_complemented = num_.opr_compl_();

    if (num_complemented.is_zero()) { return basic_Q{}; }

    basic_N_type denom_complemented = denom_.opr_compl_();

    error::division_by_zero::check(denom_complemented,
                                   "Denominator of complemented fraction cannot be zero!");

    return {std::move(num_complemented),
            std::move(denom_complemented),
            static_cast<sign_bool>(!sign_)};
}

/**
 * @brief Left bit shift (multiply numerator by 2^pos)
 * @param pos Number of positions to shift
 * @return New rational = this * 2^pos
 *
 * ALGORITHM: Shifts numerator left, keeping denominator unchanged
 * Equivalent to multiplying by 2^pos
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator<<(bitcount_t pos) const -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    return {num_.opr_bitshift_l_(pos), denom_, sign_};
}

/**
 * @brief Right bit shift (divide by 2^pos)
 * @param pos Number of positions to shift
 * @return New rational = this / 2^pos
 *
 * ALGORITHM: Shifts denominator left, keeping numerator unchanged
 * Equivalent to dividing by 2^pos
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator>>(bitcount_t pos) const -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return basic_Q{}; }
    return {num_, denom_.opr_bitshift_l_(pos), sign_};
}

/**
 * @brief Left bit shift assignment
 * @param pos Number of positions to shift
 * @return Reference to *this
 *
 * POSTCONDITION: Result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator<<=(bitcount_t pos) -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    num_.opr_bitshift_l_assign_(pos);
    canonicalise_();
    return *this;
}

/**
 * @brief Right bit shift assignment
 * @param pos Number of positions to shift
 * @return Reference to *this
 *
 * POSTCONDITION: Result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator>>=(bitcount_t pos) -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return *this; }
    denom_.opr_bitshift_l_assign_(pos);
    canonicalise_();
    return *this;
}

/**
 * @brief Assign from string in format "[-]numerator/denominator"
 * @param num_str String representation
 * @return Reference to *this
 *
 * ALGORITHM:
 * 1. Extract sign from beginning of string
 * 2. Parse numerator (up to '/')
 * 3. Parse denominator (after '/')
 * 4. Reduce to lowest terms
 * 5. Ensure zero is always positive
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator=(std::string_view num_str)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(sign_type::handle_string_(&num_str));

    num_.opr_assign_(handle_fraction_string_(&num_str));
    denom_.opr_assign_(num_str);

    canonicalise_();

    if (is_zero()) { set_sign_(positive); }

    return *this;
}

/**
 * @brief Assign from basic_N (integer) value
 * @param n Integer value
 * @return Reference to *this
 *
 * ALGORITHM: Sets num_ to n, denom_ to 1
 * Ensures that the result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator=(const basic_N_type & n)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(positive);
    num_ = n;
    denom_ = basic_N_type::one_;
    return *this;
}

/**
 * @brief Assign from moved basic_N (integer) value
 * @param n Integer value (moved)
 * @return Reference to *this
 *
 * ALGORITHM: Sets num_ to n, denom_ to 1
 * Ensures that the result is reduced to lowest terms
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator=(basic_N_type && n) -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(positive);
    num_ = std::move(n);
    denom_ = basic_N_type::one_;
    return *this;
}

/**
 * @brief Assign from basic_Z (integer with sign) value
 * @param z Integer value with sign
 * @return Reference to *this
 *
 * ALGORITHM: Sets num_ to |z|, denom_ to 1, sign_ to z.sign_
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator=(const basic_Z_type & z)
    -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(z.sign_);
    num_ = z.abs();
    denom_ = basic_N_type::one_;
    return *this;
}

/**
 * @brief Assign from moved basic_Z (integer with sign) value
 * @param z Integer value with sign (moved)
 * @return Reference to *this
 *
 * ALGORITHM: Sets num_ to |z|, denom_ to 1, sign_ to z.sign_
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::operator=(basic_Z_type && z) -> basic_Q & {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(z.sign_);
    num_ = std::move(std::move(z).abs());
    denom_ = basic_N_type::one_;
    return *this;
}

}  // namespace jmaths
