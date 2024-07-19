// The jmaths library for C++
// Copyright (C) 2024  Jasper de Smaele

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

#include <bit>
#include <cmath>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>

#include "Q.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "detail.hpp"
#include "sign_type.hpp"

// comparison functions for Q with floating point types
namespace jmaths {

template <typename FLOAT>
    requires std::is_floating_point_v<FLOAT>
bool detail::opr_eq(const Q & lhs, FLOAT rhs) {
    FUNCTION_TO_LOG;

    return (lhs == Q(rhs));
}

template <typename FLOAT>
    requires std::is_floating_point_v<FLOAT>
std::strong_ordering detail::opr_comp(const Q & lhs, FLOAT rhs) {
    FUNCTION_TO_LOG;

    return (lhs <=> Q(rhs));
}

template <typename FLOAT>
    requires std::is_floating_point_v<FLOAT>
bool operator==(const Q & lhs, FLOAT rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_eq(lhs, rhs);
}

template <typename FLOAT>
    requires std::is_floating_point_v<FLOAT>
bool operator==(FLOAT lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_eq(rhs, lhs);
}

template <typename FLOAT>
    requires std::is_floating_point_v<FLOAT>
std::strong_ordering operator<=>(const Q & lhs, FLOAT rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_comp(lhs, rhs);
}

template <typename FLOAT>
    requires std::is_floating_point_v<FLOAT>
std::strong_ordering operator<=>(FLOAT lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    return (0 <=> detail::opr_comp(rhs, lhs));
}

}  // namespace jmaths

// member function templates of Q
namespace jmaths {

template <typename FLOAT>
    requires std::is_floating_point_v<FLOAT>
std::tuple<N, N, sign_type::sign_bool> Q::handle_float_(FLOAT num) {
    FUNCTION_TO_LOG;

    int exponent;
    FLOAT significant_part = std::frexp(num, &exponent);
    significant_part =
        std::scalbn(significant_part, std::numeric_limits<FLOAT>::digits);
    exponent -= std::numeric_limits<FLOAT>::digits;

    auto numerator = std::llrint(significant_part);
    auto denominator = std::llrint(std::exp2(-exponent));

    const auto sign =
        (numerator < 0) ? sign_type::negative : sign_type::positive;

    if (sign == sign_type::negative) numerator *= -1;

    // it is assumed here that exponent is negative at this point

    return {(std::make_unsigned_t<decltype(numerator)>)numerator,
            (std::make_unsigned_t<decltype(denominator)>)denominator,
            sign};
}

template <typename FLOAT>
    requires std::is_floating_point_v<FLOAT>
Q::Q(FLOAT num) : Q(handle_float_(num)) {
    FUNCTION_TO_LOG;
}

// FIXME:
template <typename FLOAT>
    requires std::is_floating_point_v<FLOAT> &&
             std::numeric_limits<FLOAT>::is_iec559
std::optional<FLOAT> Q::fits_into() const {
    FUNCTION_TO_LOG;

    using nlf = std::numeric_limits<FLOAT>;

    static constexpr bool is_float = std::is_same_v<FLOAT, float>;
    static constexpr bool is_double = std::is_same_v<FLOAT, double>;

    static constexpr bool is_allowed_type = is_float || is_double;

    static_assert(is_allowed_type,
                  "Template type parameter is not one of the allowed types: "
                  "float and double.");
    static_assert(nlf::radix == 2,
                  "The radix of the floating point type is currently not "
                  "supported. Please make sure it is equal to 2.");

    static constexpr bool is_big_endian =
        std::endian::native == std::endian::big;
    static constexpr bool is_little_endian =
        std::endian::native == std::endian::little;

    static_assert(is_big_endian || is_little_endian,
                  "Mixed endianness is not supported. Unclear how to implement "
                  "floating point manipulation.");

    struct float_sizes {
        enum : bit_type { sign = 1, exponent = 8, mantissa = 23 };

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
        enum : bit_type { sign = 1, exponent = 11, mantissa = 52 };

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

    static_assert(sizeof(float_access) == sizeof(float) &&
                      sizeof(float_access) == sizeof(std::uint32_t[1]),
                  "There seems to be a problem with the padding bits for type: "
                  "float_access.");
    static_assert(sizeof(double_access) == sizeof(double) &&
                      sizeof(double_access) == sizeof(std::uint64_t[1]),
                  "There seems to be a problem with the padding bits for type: "
                  "double_access.");

#if 0
  static_assert(sizeof(long_double_access) == sizeof(long double) && sizeof(long_double_access) == sizeof(std::uint64_t[2]), "There seems to be a problem with the padding bits for type: long_double_access.");
#endif

    using access_type =
        std::conditional_t<is_float, float_access, double_access>;
    using sizes_type = std::conditional_t<is_float, float_sizes, double_sizes>;

    if (num_.is_zero()) return 0;
    if (is_one()) return 1;
    if (is_neg_one()) return -1;

    FLOAT numerator{};

    {
        std::size_t i = 0;

        for (auto crit = num_.digits_.crbegin();
             crit != num_.digits_.crend() && i < sizeof(FLOAT) / base_int_size;
             ++crit, ++i) {
            numerator = numerator * radix + *crit;
        }

        for (; i < sizeof(FLOAT) / base_int_size; ++i) {
            numerator *= radix;
        }
    }

    FLOAT denominator{};

    {
        std::size_t j = 0;

        for (auto crit = denom_.digits_.crbegin();
             crit != denom_.digits_.crend() &&
             j < sizeof(FLOAT) / base_int_size;
             ++crit, ++j) {
            denominator = denominator * radix + *crit;
        }

        for (; j < sizeof(FLOAT) / base_int_size; ++j) {
            denominator *= radix;
        }
    }

    access_type result = {.val = numerator / denominator};

    if (num_.digits_.size() < denom_.digits_.size()) {
        static constexpr std::uint16_t min_exponent = (std::uint16_t)1;

        if (result.fields.exponent <
            min_exponent +
                (denom_.digits_.size() - num_.digits_.size()) * base_int_bits) {
#if 0
  if (*this >= Q(nlf::min())) {
  return nlf::min();
  } else {
  return std::nullopt;
  }
#endif
            // ^^^ doesn't yet take care of subnormals
            return std::nullopt;
        }

        result.fields.exponent -=
            (denom_.digits_.size() - num_.digits_.size()) * base_int_bits;
    } else {
        static constexpr std::uint16_t max_exponent =
            ~(~(std::uint16_t)0 << sizes_type::exponent) - 1;

        if ((num_.digits_.size() - denom_.digits_.size()) * base_int_bits >
            max_exponent - result.fields.exponent) {
            if constexpr (nlf::has_infinity) {
                return nlf::infinity();
            } else {
                return std::nullopt;
            }
        }

        result.fields.exponent +=
            (num_.digits_.size() - denom_.digits_.size()) * base_int_bits;
    }

    result.fields.sign = is_negative();

    return result.val;
}

template <typename FLOAT>
    requires std::is_floating_point_v<FLOAT>
Q & Q::operator=(FLOAT rhs) {
    FUNCTION_TO_LOG;

    auto fraction_info = handle_float_(rhs);
    num_ = std::move(std::get<0>(fraction_info));
    denom_ = std::move(std::get<1>(fraction_info));
    set_sign_(std::get<2>(fraction_info));

    canonicalise_();

    return *this;
}

}  // namespace jmaths

#include "undef.hh"
