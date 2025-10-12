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

#include "basic_N.hpp"
#include "basic_Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "sign_type.hpp"

// member functions of Z::detail
namespace jmaths {

/**********************************************************/
// implementation functions

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::ostream & basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_ins(
    std::ostream & os,
    const basic_Z & z) {
    JMATHS_FUNCTION_TO_LOG;

    return os << z.conv_to_base_();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::istream & basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_extr(std::istream & is,
                                                                                   basic_Z & z) {
    JMATHS_FUNCTION_TO_LOG;

    std::string num_str;
    is >> num_str;
    z = num_str;
    return is;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_add(const basic_Z & lhs, const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    using sign_type::positive, sign_type::negative;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            return {basic_N_type::detail::opr_add(lhs.abs(), rhs.abs()), positive};
        }

        if (const auto difference = basic_N_type::detail::opr_comp(lhs.abs(), rhs.abs());
            difference == 0) {
            return basic_Z{};
        } else if (difference > 0) {
            return {basic_N_type::detail::opr_subtr(lhs.abs(), rhs.abs()), positive};
        } else {
            return {basic_N_type::detail::opr_subtr(rhs.abs(), lhs.abs()), negative};
        }
    } else {
        if (rhs.is_negative()) {
            return {basic_N_type::detail::opr_add(lhs.abs(), rhs.abs()), negative};
        }

        if (const auto difference = basic_N_type::detail::opr_comp(lhs.abs(), rhs.abs());
            difference == 0) {
            return basic_Z{};
        } else if (difference > 0) {
            return {basic_N_type::detail::opr_subtr(lhs.abs(), rhs.abs()), negative};
        } else {
            return {basic_N_type::detail::opr_subtr(rhs.abs(), lhs.abs()), positive};
        }
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_subtr(const basic_Z & lhs,
                                                           const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    using sign_type::positive, sign_type::negative;

    if (lhs.is_positive()) {
        if (rhs.is_negative()) {
            return {basic_N_type::detail::opr_add(lhs.abs(), rhs.abs()), positive};
        }

        if (const auto difference = basic_N_type::detail::opr_comp(lhs.abs(), rhs.abs());
            difference == 0) {
            return basic_Z{};
        } else if (difference > 0) {
            return {basic_N_type::detail::opr_subtr(lhs.abs(), rhs.abs()), positive};
        } else {
            return {basic_N_type::detail::opr_subtr(rhs.abs(), lhs.abs()), negative};
        }
    } else {
        if (rhs.is_positive()) {
            return {basic_N_type::detail::opr_add(lhs.abs(), rhs.abs()), negative};
        }

        if (const auto difference = basic_N_type::detail::opr_comp(lhs.abs(), rhs.abs());
            difference == 0) {
            return basic_Z{};
        } else if (difference > 0) {
            return {basic_N_type::detail::opr_subtr(lhs.abs(), rhs.abs()), negative};
        } else {
            return {basic_N_type::detail::opr_subtr(rhs.abs(), lhs.abs()), positive};
        }
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_mult(const basic_Z & lhs, const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type product = basic_N_type::detail::opr_mult(lhs.abs(), rhs.abs());

    if (product.is_zero()) { return basic_Z{}; }

    return {std::move(product), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::pair<basic_Z<BaseInt, BaseIntBig, Allocator>, basic_Z<BaseInt, BaseIntBig, Allocator>>
basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_div(const basic_Z & lhs, const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (auto [quotient, remainder] = basic_N_type::detail::opr_div(lhs.abs(), rhs.abs());
        quotient.is_zero()) {
        if (remainder.is_zero()) {
            return {basic_Z{}, basic_Z{}};
        } else {
            return {basic_Z{}, basic_Z{std::move(remainder), lhs.sign_}};
        }
    } else {
        if (remainder.is_zero()) {
            return {basic_Z{std::move(quotient),
                            static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)},
                    basic_Z{}};
        } else {
            return {basic_Z{std::move(quotient),
                            static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)},
                    basic_Z{std::move(remainder), lhs.sign_}};
        }
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_and(const basic_Z & lhs, const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type and_result = basic_N_type::detail::opr_and(lhs.abs(), rhs.abs());

    if (and_result.is_zero()) { return basic_Z{}; }

    return {std::move(and_result), static_cast<sign_type::sign_bool>(lhs.sign_ & rhs.sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_or(const basic_Z & lhs, const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type or_result = basic_N_type::detail::opr_or(lhs.abs(), rhs.abs());

    if (or_result.is_zero()) { return basic_Z{}; }

    return {std::move(or_result), static_cast<sign_type::sign_bool>(lhs.sign_ | rhs.sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_xor(const basic_Z & lhs, const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type xor_result = basic_N_type::detail::opr_xor(lhs.abs(), rhs.abs());

    if (xor_result.is_zero()) { return basic_Z{}; }

    return {std::move(xor_result), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_eq(const basic_Z & lhs,
                                                                       const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return lhs.sign_ == rhs.sign_ && lhs.abs() == rhs.abs();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::strong_ordering basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_comp(
    const basic_Z & lhs,
    const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            return basic_N_type::detail::opr_comp(lhs.abs(), rhs.abs());
        } else {
            return std::strong_ordering::greater;
        }
    } else {
        if (rhs.is_positive()) {
            return std::strong_ordering::less;
        } else {
            return basic_N_type::detail::opr_comp(rhs.abs(), lhs.abs());
        }
    }
}

}  // namespace jmaths
