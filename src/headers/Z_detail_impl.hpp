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
#include <istream>
#include <ostream>
#include <string>
#include <utility>

#include "N.hpp"
#include "Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "sign_type.hpp"

// member functions of Z::detail
namespace jmaths {

/**********************************************************/
// implementation functions

inline std::ostream & Z::detail::opr_ins(std::ostream & os, const Z & z) {
    JMATHS_FUNCTION_TO_LOG;

    return os << z.conv_to_base_();
}

inline std::istream & Z::detail::opr_extr(std::istream & is, Z & z) {
    JMATHS_FUNCTION_TO_LOG;

    std::string num_str;
    is >> num_str;
    z = num_str;
    return is;
}

inline Z Z::detail::opr_add(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    using sign_type::positive, sign_type::negative;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) { return {N::detail::opr_add(lhs.abs(), rhs.abs()), positive}; }

        if (const auto difference = N::detail::opr_comp(lhs.abs(), rhs.abs()); difference == 0) {
            return Z{};
        } else if (difference > 0) {
            return {N::detail::opr_subtr(lhs.abs(), rhs.abs()), positive};
        } else {
            return {N::detail::opr_subtr(rhs.abs(), lhs.abs()), negative};
        }
    } else {
        if (rhs.is_negative()) { return {N::detail::opr_add(lhs.abs(), rhs.abs()), negative}; }

        if (const auto difference = N::detail::opr_comp(lhs.abs(), rhs.abs()); difference == 0) {
            return Z{};
        } else if (difference > 0) {
            return {N::detail::opr_subtr(lhs.abs(), rhs.abs()), negative};
        } else {
            return {N::detail::opr_subtr(rhs.abs(), lhs.abs()), positive};
        }
    }
}

inline Z Z::detail::opr_subtr(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    using sign_type::positive, sign_type::negative;

    if (lhs.is_positive()) {
        if (rhs.is_negative()) { return {N::detail::opr_add(lhs.abs(), rhs.abs()), positive}; }

        if (const auto difference = N::detail::opr_comp(lhs.abs(), rhs.abs()); difference == 0) {
            return Z{};
        } else if (difference > 0) {
            return {N::detail::opr_subtr(lhs.abs(), rhs.abs()), positive};
        } else {
            return {N::detail::opr_subtr(rhs.abs(), lhs.abs()), negative};
        }
    } else {
        if (rhs.is_positive()) { return {N::detail::opr_add(lhs.abs(), rhs.abs()), negative}; }

        if (const auto difference = N::detail::opr_comp(lhs.abs(), rhs.abs()); difference == 0) {
            return Z{};
        } else if (difference > 0) {
            return {N::detail::opr_subtr(lhs.abs(), rhs.abs()), negative};
        } else {
            return {N::detail::opr_subtr(rhs.abs(), lhs.abs()), positive};
        }
    }
}

inline Z Z::detail::opr_mult(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    N product = N::detail::opr_mult(lhs.abs(), rhs.abs());

    if (product.is_zero()) { return Z{}; }

    return {std::move(product), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)};
}

inline std::pair<Z, Z> Z::detail::opr_div(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (auto [quotient, remainder] = N::detail::opr_div(lhs.abs(), rhs.abs()); quotient.is_zero()) {
        if (remainder.is_zero()) {
            return {Z{}, Z{}};
        } else {
            return {Z{}, Z{std::move(remainder), lhs.sign_}};
        }
    } else {
        if (remainder.is_zero()) {
            return {Z{std::move(quotient), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)},
                    Z{}};
        } else {
            return {Z{std::move(quotient), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)},
                    Z{std::move(remainder), lhs.sign_}};
        }
    }
}

inline Z Z::detail::opr_and(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    N and_result = N::detail::opr_and(lhs.abs(), rhs.abs());

    if (and_result.is_zero()) { return Z{}; }

    return {std::move(and_result), static_cast<sign_type::sign_bool>(lhs.sign_ & rhs.sign_)};
}

inline Z Z::detail::opr_or(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    N or_result = N::detail::opr_or(lhs.abs(), rhs.abs());

    if (or_result.is_zero()) { return Z{}; }

    return {std::move(or_result), static_cast<sign_type::sign_bool>(lhs.sign_ | rhs.sign_)};
}

inline Z Z::detail::opr_xor(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    N xor_result = N::detail::opr_xor(lhs.abs(), rhs.abs());

    if (xor_result.is_zero()) { return Z{}; }

    return {std::move(xor_result), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)};
}

inline bool Z::detail::opr_eq(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return lhs.sign_ == rhs.sign_ && lhs.abs() == rhs.abs();
}

inline std::strong_ordering Z::detail::opr_comp(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            return N::detail::opr_comp(lhs.abs(), rhs.abs());
        } else {
            return std::strong_ordering::greater;
        }
    } else {
        if (rhs.is_positive()) {
            return std::strong_ordering::less;
        } else {
            return N::detail::opr_comp(rhs.abs(), lhs.abs());
        }
    }
}

}  // namespace jmaths
