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

#include <compare>
#include <cstddef>
#include <limits>
#include <optional>
#include <type_traits>

#include "Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "detail.hpp"
#include "sign_type.hpp"

// comparison functions for Z with integral types
namespace jmaths {

template <typename INT>
    requires std::is_integral_v<INT>
bool detail::opr_eq(const Z & lhs, INT rhs) {
    FUNCTION_TO_LOG;

    if (rhs < 0) {
        if (lhs.is_positive()) return false;
        return (lhs.abs() == -rhs);
    }

    return (lhs.abs() == rhs);
}

template <typename INT>
    requires std::is_integral_v<INT>
std::strong_ordering detail::opr_comp(const Z & lhs, INT rhs) {
    FUNCTION_TO_LOG;

    if (lhs.is_positive()) {
        if (rhs >= 0) {
            return (lhs.abs() <=> rhs);
        } else {
            return std::strong_ordering::greater;
        }
    } else {
        if (rhs >= 0) {
            return std::strong_ordering::less;
        } else {
            return (-rhs <=> lhs.abs());
        }
    }
}

template <typename INT>
    requires std::is_integral_v<INT>
bool operator==(const Z & lhs, INT rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_eq(lhs, rhs);
}

template <typename INT>
    requires std::is_integral_v<INT>
bool operator==(INT lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_eq(rhs, lhs);
}

template <typename INT>
    requires std::is_integral_v<INT>
std::strong_ordering operator<=>(const Z & lhs, INT rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_comp(lhs, rhs);
}

template <typename INT>
    requires std::is_integral_v<INT>
std::strong_ordering operator<=>(INT lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    return (0 <=> detail::opr_comp(rhs, lhs));
}

}  // namespace jmaths

// member function templates of Z
namespace jmaths {

template <typename INT>
    requires std::is_integral_v<INT>
Z::Z(INT num) : sign_type(num), N(num) {
    FUNCTION_TO_LOG;
}

template <typename INT>
    requires std::is_integral_v<INT>
std::optional<INT> Z::fits_into() const {
    FUNCTION_TO_LOG;

    if constexpr (std::is_unsigned_v<INT>) {
        if (is_positive())
            return N::fits_into<INT>();
        else
            return std::nullopt;
    }

    if (is_zero()) return 0;

    if constexpr (sizeof(INT) < base_int_size) {
        if (digits_.front() > (is_negative() ?
                                   -std::numeric_limits<INT>::min() :
                                   std::numeric_limits<INT>::max()) ||
            digits_.size() > 1)
            return std::nullopt;
        return (is_negative() ? -digits_.front() : digits_.front());
    } else {
        if (digits_.size() * base_int_size > sizeof(INT)) return std::nullopt;

        if (detail::opr_comp(
                *this,
                (is_negative() ? -std::numeric_limits<INT>::min() :
                                 std::numeric_limits<INT>::max())) > 0)
            return std::nullopt;

#if 0
  INT converted (digits_.back());
  for (std::size_t i = digits_.size() - 1; i --> 0;) {
  converted <<= base_int_bits;
  converted |= digits_[i];
  }
#endif

        INT converted(digits_.front());

        for (std::size_t i = 1; i < digits_.size(); ++i) {
            converted += (INT)((std::make_unsigned_t<INT>)digits_[i]
                               << (base_int_bits * i));
        }

        if (is_negative())
            return -converted;
        else
            return converted;
    }
}

template <typename INT>
    requires std::is_integral_v<INT>
Z & Z::operator=(INT rhs) {
    FUNCTION_TO_LOG;

    set_sign_(sign_type::handle_int_(rhs));
    N::opr_assign_(rhs);
    return *this;
}

}  // namespace jmaths

#include "undef.hh"
