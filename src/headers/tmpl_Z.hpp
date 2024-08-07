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
#include <concepts>
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

bool detail::opr_eq(const Z & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit == rhs;
    }

    return false;
}

std::strong_ordering detail::opr_comp(const Z & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit <=> rhs;
    }

    return lhs.is_negative() ? std::strong_ordering::less : std::strong_ordering::greater;
}

bool operator==(const Z & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_eq(lhs, rhs);
}

bool operator==(std::integral auto lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_eq(rhs, lhs);
}

std::strong_ordering operator<=>(const Z & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_comp(lhs, rhs);
}

std::strong_ordering operator<=>(std::integral auto lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return 0 <=> detail::opr_comp(rhs, lhs);
}

}  // namespace jmaths

// member function templates of Z
namespace jmaths {

Z::Z(std::integral auto num) : sign_type(&num), N(num) {
    JMATHS_FUNCTION_TO_LOG;
}

template <std::unsigned_integral T> std::optional<T> Z::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) { return std::nullopt; }

    return N::fits_into<T>();
}

template <std::signed_integral T> std::optional<T> Z::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    static_assert(-1 == ~0, "Two's complement architecture required.");

    if (is_positive()) { return N::fits_into<T>(); }

    auto fits_into_unsigned = N::fits_into<std::make_unsigned_t<T>>();

    if (!fits_into_unsigned.has_value()) { return std::nullopt; }

    if (*fits_into_unsigned >
        static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()) + 1) {
        return std::nullopt;
    }

    return -*fits_into_unsigned;
}

Z & Z::operator=(std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(sign_type::handle_int_(&rhs));
    N::opr_assign_(rhs);
    return *this;
}

}  // namespace jmaths
