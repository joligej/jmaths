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
#include <istream>
#include <ostream>
#include <string>
#include <utility>

#include "basic_N.hpp"
#include "basic_Q.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "error.hpp"
#include "sign_type.hpp"

// member functions of Q::detail
namespace jmaths {

/**********************************************************/
// implementation functions

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::ostream & basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_ins(
    std::ostream & os,
    const basic_Q & q) {
    JMATHS_FUNCTION_TO_LOG;

    return os << q.conv_to_base_();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::istream & basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_extr(std::istream & is,
                                                                                   basic_Q & q) {
    JMATHS_FUNCTION_TO_LOG;

    std::string num_str;
    is >> num_str;
    q = num_str;
    return is;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_add(const basic_Q & lhs,
                                                                        const basic_Q & rhs)
    -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Rational number addition
    // To add two fractions a/b + c/d, we use the formula:
    // a/b + c/d = (a*d + b*c) / (b*d)
    //
    // However, we need to handle signs separately:
    // - If both have same sign: add numerators (scaled appropriately)
    // - If different signs: subtract numerators, result takes sign of larger magnitude
    //
    // The result should be automatically simplified later by the constructor or
    // other normalization functions to maintain the fraction in lowest terms.
    //
    // Example: 1/2 + 1/3 = (1*3 + 2*1) / (2*3) = 5/6

    using sign_type::positive, sign_type::negative;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            // Both positive: (a/b) + (c/d) = (a*d + b*c) / (b*d)
            basic_N_type first_product = basic_N_type::detail::opr_mult(lhs.num_, rhs.denom_);
            first_product.opr_add_assign_(basic_N_type::detail::opr_mult(lhs.denom_, rhs.num_));
            return {std::move(first_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    positive};
        }

        // Different signs: need to subtract and determine result sign
        basic_N_type first_product = basic_N_type::detail::opr_mult(lhs.num_, rhs.denom_);
        basic_N_type second_product = basic_N_type::detail::opr_mult(lhs.denom_, rhs.num_);

        if (const auto difference = basic_N_type::detail::opr_comp(first_product, second_product);
            difference == 0) {
            return basic_Q{};  // Results cancel to zero
        } else if (difference > 0) {
            first_product.opr_subtr_assign_(second_product);
            return {std::move(first_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    positive};
        } else {
            second_product.opr_subtr_assign_(first_product);
            return {std::move(second_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    negative};
        }
    } else {
        if (rhs.is_negative()) {
            // Both negative: -(|a|/b) + -(|c|/d) = -((|a|*d + b*|c|) / (b*d))
            basic_N_type first_product = basic_N_type::detail::opr_mult(lhs.num_, rhs.denom_);
            first_product.opr_add_assign_(basic_N_type::detail::opr_mult(lhs.denom_, rhs.num_));
            return {std::move(first_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    negative};
        }

        // Different signs: need to subtract and determine result sign
        basic_N_type first_product = basic_N_type::detail::opr_mult(lhs.num_, rhs.denom_);
        basic_N_type second_product = basic_N_type::detail::opr_mult(lhs.denom_, rhs.num_);

        if (const auto difference = basic_N_type::detail::opr_comp(first_product, second_product);
            difference == 0) {
            return basic_Q{};
        } else if (difference > 0) {
            first_product.opr_subtr_assign_(second_product);
            return {std::move(first_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    negative};
        } else {
            second_product.opr_subtr_assign_(first_product);
            return {std::move(second_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    positive};
        }
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_subtr(const basic_Q & lhs,
                                                                          const basic_Q & rhs)
    -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    using sign_type::positive, sign_type::negative;

    if (lhs.is_positive()) {
        if (rhs.is_negative()) {
            basic_N_type first_product = basic_N_type::detail::opr_mult(lhs.num_, rhs.denom_);
            first_product.opr_add_assign_(basic_N_type::detail::opr_mult(lhs.denom_, rhs.num_));
            return {std::move(first_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    positive};
        }

        basic_N_type first_product = basic_N_type::detail::opr_mult(lhs.num_, rhs.denom_);
        basic_N_type second_product = basic_N_type::detail::opr_mult(lhs.denom_, rhs.num_);

        if (const auto difference = basic_N_type::detail::opr_comp(first_product, second_product);
            difference == 0) {
            return basic_Q{};
        } else if (difference > 0) {
            first_product.opr_subtr_assign_(second_product);
            return {std::move(first_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    positive};
        } else {
            second_product.opr_subtr_assign_(first_product);
            return {std::move(second_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    negative};
        }
    } else {
        if (rhs.is_positive()) {
            basic_N_type first_product = basic_N_type::detail::opr_mult(lhs.num_, rhs.denom_);
            first_product.opr_add_assign_(basic_N_type::detail::opr_mult(lhs.denom_, rhs.num_));
            return {std::move(first_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    negative};
        }

        basic_N_type first_product = basic_N_type::detail::opr_mult(lhs.num_, rhs.denom_);
        basic_N_type second_product = basic_N_type::detail::opr_mult(lhs.denom_, rhs.num_);

        if (const auto difference = basic_N_type::detail::opr_comp(first_product, second_product);
            difference == 0) {
            return basic_Q{};
        } else if (difference > 0) {
            first_product.opr_subtr_assign_(second_product);
            return {std::move(first_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    negative};
        } else {
            second_product.opr_subtr_assign_(first_product);
            return {std::move(second_product),
                    basic_N_type::detail::opr_mult(lhs.denom_, rhs.denom_),
                    positive};
        }
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_mult(const basic_Q & lhs,
                                                                         const basic_Q & rhs)
    -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Rational number multiplication
    // Multiplying fractions is straightforward:
    // (a/b) * (c/d) = (a*c) / (b*d)
    //
    // Sign rule: same as integer multiplication (XOR of signs)
    // - positive * positive = positive
    // - negative * negative = positive
    // - positive * negative = negative
    // - negative * positive = negative
    //
    // The result should be reduced to lowest terms by dividing both numerator
    // and denominator by their GCD (done elsewhere in the constructor).

    basic_N_type numerator = lhs.num_ * rhs.num_;

    if (numerator.is_zero()) { return basic_Q{}; }

    return {std::move(numerator),
            lhs.denom_ * rhs.denom_,
            static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_div(const basic_Q & lhs,
                                                                        const basic_Q & rhs)
    -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Rational number division
    // Division of fractions uses the "invert and multiply" rule:
    // (a/b) / (c/d) = (a/b) * (d/c) = (a*d) / (b*c)
    //
    // In other words, we multiply the first fraction by the reciprocal of the second.
    // Sign handling is the same as multiplication.
    //
    // Example: (2/3) / (4/5) = (2/3) * (5/4) = 10/12 = 5/6 (after reduction)
    //
    // Note: Division by zero is checked before this function is called.

    basic_N_type numerator = lhs.num_ * rhs.denom_;

    if (numerator.is_zero()) { return basic_Q{}; }

    return {std::move(numerator),
            lhs.denom_ * rhs.num_,
            static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_and(const basic_Q & lhs,
                                                                        const basic_Q & rhs)
    -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type denominator = lhs.denom_ & rhs.denom_;

    error::division_by_zero::check(denominator);

    basic_N_type numerator = lhs.num_ & rhs.num_;

    if (numerator.is_zero()) { return basic_Q{}; }

    return {std::move(numerator),
            std::move(denominator),
            static_cast<sign_type::sign_bool>(lhs.sign_ & rhs.sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_or(const basic_Q & lhs,
                                                                       const basic_Q & rhs)
    -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type numerator = lhs.num_ | rhs.num_;

    if (numerator.is_zero()) { return basic_Q{}; }

    return {std::move(numerator),
            lhs.denom_ | rhs.denom_,
            static_cast<sign_type::sign_bool>(lhs.sign_ | rhs.sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_xor(const basic_Q & lhs,
                                                                        const basic_Q & rhs)
    -> basic_Q {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type denominator = lhs.denom_ ^ rhs.denom_;

    error::division_by_zero::check(denominator);

    basic_N_type numerator = lhs.num_ ^ rhs.num_;

    if (numerator.is_zero()) { return basic_Q{}; }

    return {std::move(numerator),
            std::move(denominator),
            static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_eq(const basic_Q & lhs,
                                                                       const basic_Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return lhs.sign_ == rhs.sign_ && lhs.num_ == rhs.num_ && lhs.denom_ == rhs.denom_;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::strong_ordering basic_Q<BaseInt, BaseIntBig, Allocator>::detail::opr_comp(
    const basic_Q & lhs,
    const basic_Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Rational number comparison
    // To compare a/b with c/d, we can't simply compare numerators and denominators.
    // Instead, we use cross-multiplication to avoid division:
    // a/b < c/d  ⟺  a*d < b*c  (assuming positive denominators)
    //
    // Steps:
    // 1. If signs differ, the positive one is greater
    // 2. If both positive: compare a*d with b*c
    // 3. If both negative: compare in reverse (larger magnitude means smaller value)
    //
    // This avoids the need for actual division and works with integer arithmetic only.

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            return basic_N_type::detail::opr_comp(lhs.num_ * rhs.denom_, rhs.num_ * lhs.denom_);
        } else {
            return std::strong_ordering::greater;
        }
    } else {
        if (rhs.is_positive()) {
            return std::strong_ordering::less;
        } else {
            // Both negative: reverse the comparison (more negative = smaller)
            return basic_N_type::detail::opr_comp(rhs.num_ * lhs.denom_, lhs.num_ * rhs.denom_);
        }
    }
}

}  // namespace jmaths
