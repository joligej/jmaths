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

#include <algorithm>
#include <bit>
#include <cassert>
#include <cmath>  // only for karatsuba
#include <compare>
#include <cstddef>
#include <istream>
#include <iterator>  // only for std::back_inserter
#include <limits>
#include <ostream>
#include <string>
#include <utility>

#include "basic_N.hpp"
#include "constants_and_types.hpp"
#include "def.hh"

// member functions of N::detail
namespace jmaths {

/**********************************************************/
// implementation functions

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::ostream & basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_ins(
    std::ostream & os,
    const basic_N & n) {
    JMATHS_FUNCTION_TO_LOG;

    return os << n.conv_to_base_();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::istream & basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_extr(std::istream & is,
                                                                                   basic_N & n) {
    JMATHS_FUNCTION_TO_LOG;

    std::string num_str;
    is >> num_str;
    n.opr_assign_(num_str);

    assert(n == N{num_str});

    return is;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_add(const basic_N & lhs,
                                                                        const basic_N & rhs)
    -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    // check for additive identity
    if (lhs.is_zero()) { return rhs; }
    if (rhs.is_zero()) { return lhs; }

    const basic_N * longest;
    const basic_N * shortest;

    if (lhs.digits_.size() < rhs.digits_.size()) {
        longest = &rhs;
        shortest = &lhs;
    } else {
        longest = &lhs;
        shortest = &rhs;
    }

    assert(longest && shortest);

    basic_N sum;
    sum.digits_.reserve(longest->digits_.size() + 1U);

    bool carry = false;

    std::size_t i = 0U;

    for (; i < shortest->digits_.size(); ++i) {
        // checks if lhs + rhs + carry < base
        const bool next_carry = !(lhs.digits_[i] < (carry ? max_digit : radix) - rhs.digits_[i]);

        sum.digits_.emplace_back(lhs.digits_[i] + rhs.digits_[i] +
                                 static_cast<base_int_type>(carry));
        carry = next_carry;
    }

    if (carry) {
        for (; i < longest->digits_.size(); ++i) {
            sum.digits_.emplace_back(longest->digits_[i] + 1U);

            if (longest->digits_[i] < max_digit) {
                ++i;
                goto loop_without_carry;
            }
        }

        sum.digits_.emplace_back(1);

        goto end_of_function;
    }

loop_without_carry:

    for (; i < longest->digits_.size(); ++i) {
        sum.digits_.emplace_back(longest->digits_[i]);
    }

end_of_function:

    assert(sum.is_zero() || sum.digits_.back() != 0U);

    return sum;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_subtr(basic_N lhs,
                                                                          const basic_N & rhs)
    -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    // this functions assumes that lhs >= rhs and for effiency reasons should
    // only be called when lhs > rhs

    // check for additive identity
    if (rhs.is_zero()) { return lhs; }

    basic_N difference;
    difference.digits_.reserve(lhs.digits_.size());

    std::size_t i = 0U;

    for (; i < rhs.digits_.size(); ++i) {
        if (lhs.digits_[i] < rhs.digits_[i]) {
            for (std::size_t j = i + 1U; j < lhs.digits_.size(); ++j) {
                if (lhs.digits_[j]-- > 0U) { break; }
            }
        }

        difference.digits_.emplace_back(lhs.digits_[i] - rhs.digits_[i]);
    }

    for (; i < lhs.digits_.size(); ++i) {
        difference.digits_.emplace_back(lhs.digits_[i]);
    }

    difference.remove_leading_zeroes_();

    assert(difference.is_zero() || difference.digits_.back() != 0U);

    return difference;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_mult(const basic_N & lhs,
                                                                         const basic_N & rhs)
    -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

#ifndef JMATHS_KARATSUBA
    #error "JMATHS_KARATSUBA is not defined, please define it when compiling!"
#endif

#if JMATHS_KARATSUBA

    #error "The Karatsuba algorithm is still a work in progress, please define KARATSUBA as 0"

    // check for multiplicative identity
    if (lhs.is_one()) { return rhs; }
    if (rhs.is_one()) { return lhs; }

    // check for multiplicative zero
    if (lhs.is_zero() || rhs.is_zero()) { return N{}; }

    const N & longest = lhs.digits_.size() > rhs.digits_.size() ? lhs : rhs;

    const std::size_t number_of_digits = std::ceil(longest.digits_.size() / 2.0L);

    const N lhs_l = lhs.opr_bitshift_r_(number_of_digits * base_int_type_bits);
    const N rhs_l = rhs.opr_bitshift_r_(number_of_digits * base_int_type_bits);

    const N lhs_l_rhs_l = opr_mult(lhs_l, rhs_l);

    const auto right_side_mask =
        --(N(N::one_) <<= (longest.digits_.size() - number_of_digits) * base_int_type_bits);

    const N lhs_r = opr_and(lhs, right_side_mask);
    const N rhs_r = opr_and(rhs, right_side_mask);

    const N lhs_r_rhs_r = opr_mult(lhs_r, rhs_r);

    static const N N_radix(radix);

    const auto power = calc::pow(N_radix, number_of_digits);

    return power * (power * lhs_l_rhs_l +
                    ((lhs_l + lhs_r) * (rhs_l + rhs_r) - lhs_l_rhs_l - lhs_r_rhs_r)) +
           lhs_r_rhs_r;

#else

    // check for multiplicative identity
    if (lhs.is_one()) { return rhs; }
    if (rhs.is_one()) { return lhs; }

    // check for multiplicative zero
    if (lhs.is_zero() || rhs.is_zero()) { return basic_N{}; }

    basic_N product;

    // max overhead would be base_int_type_size
    product.digits_.reserve(lhs.digits_.size() + rhs.digits_.size());

    for (std::size_t i = 0U; i < lhs.digits_.size(); ++i) {
        basic_N temp1;
        temp1.digits_.reserve(i + rhs.digits_.size() + 1U);
        temp1.digits_.insert(temp1.digits_.begin(), i, 0U);
        base_int_type carry = 0U;
        for (std::size_t j = 0U; j < rhs.digits_.size(); ++j) {
            const base_int_big_type temp2 = static_cast<base_int_big_type>(lhs.digits_[i]) *
                                            static_cast<base_int_big_type>(rhs.digits_[j]);
            const base_int_type temp3 = static_cast<base_int_type>(temp2);
            temp1.digits_.emplace_back(carry + temp3);
            const base_int_type temp_carry = static_cast<base_int_type>(
                (static_cast<base_int_big_type>(carry) + static_cast<base_int_big_type>(temp3)) >>
                base_int_type_bits);
            carry = static_cast<base_int_type>((temp2 >> base_int_type_bits) + temp_carry);
        }

        if (carry != 0U) { temp1.digits_.emplace_back(carry); }
        product.opr_add_assign_(temp1);
    }

    return product;

#endif
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_div(const basic_N & lhs,
                                                                        const basic_N & rhs)
    -> std::pair<basic_N, basic_N> {
    JMATHS_FUNCTION_TO_LOG;

    if (lhs.is_zero()) { return {basic_N{}, basic_N{}}; }

    // check if lhs == rhs
    if (opr_eq(lhs, rhs)) { return {one_, basic_N{}}; }

    std::pair<basic_N, basic_N> result;

    auto & [q, r] = result;

    if (const auto max_size = lhs.digits_.size() - rhs.digits_.size() + 1U;
        lhs.digits_.size() + 1U >= rhs.digits_.size()) {
        // rhs.digits_.size() is always >= 1 so even if lhs.digits_.size() + 1
        // overflows to 0 no errors shall occur
#ifndef NDEBUG
        using nlim = std::numeric_limits<decltype(lhs.digits_.size())>;
#endif

        assert(
            lhs.digits_.size() == nlim::max() ?
                !rhs.digits_.empty() :
            lhs.digits_.size() >= rhs.digits_.size() ?
                lhs.digits_.size() - rhs.digits_.size() <= nlim::max() - 1U :
                true /*-(rhs.digits_.size() - lhs.digits_.size()) <= nlim::max() - 1*/);  // assert
                                                                                          // for
                                                                                          // overflow
        assert(lhs.digits_.size() == nlim::max() ?
                   true :
                   lhs.digits_.size() + 1U >= rhs.digits_.size());  // assert for overflow to 0

        q.digits_.reserve(max_size);
    }

    r.digits_.reserve(rhs.digits_.size());

    for (bitpos_t i = lhs.digits_.size() * base_int_type_bits -
                      static_cast<bitcount_t>(std::countl_zero(lhs.digits_.back()));
         i-- > 0U;) {
        r.opr_bitshift_l_assign_(1U);
        r.bit_(0U, lhs.bit_(i));
        if (opr_comp(r, rhs) >= 0) {
            r.opr_subtr_assign_(rhs);
            q.bit_(i, 1);
        }
    }

    assert(q * rhs + r == lhs);

    return result;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_and(const basic_N & lhs,
                                                                        const basic_N & rhs)
    -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    if (lhs.is_zero() || rhs.is_zero()) { return basic_N{}; }

    const basic_N & shortest = lhs.digits_.size() < rhs.digits_.size() ? lhs : rhs;

    basic_N and_result;
    and_result.digits_.reserve(shortest.digits_.size());

    for (std::size_t i = 0U; i < shortest.digits_.size(); ++i) {
        and_result.digits_.emplace_back(lhs.digits_[i] & rhs.digits_[i]);
    }

    and_result.remove_leading_zeroes_();

    return and_result;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_or(const basic_N & lhs,
                                                                       const basic_N & rhs)
    -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    if (lhs.is_zero()) { return rhs; }
    if (rhs.is_zero()) { return lhs; }

    const basic_N * longest;
    const basic_N * shortest;

    if (lhs.digits_.size() < rhs.digits_.size()) {
        longest = &rhs;
        shortest = &lhs;
    } else {
        longest = &lhs;
        shortest = &rhs;
    }

    basic_N or_result;
    or_result.digits_.reserve(longest->digits_.size());

    std::size_t i = 0U;

    for (; i < shortest->digits_.size(); ++i) {
        or_result.digits_.emplace_back(lhs.digits_[i] | rhs.digits_[i]);
    }

    for (; i < longest->digits_.size(); ++i) {
        or_result.digits_.emplace_back(longest->digits_[i]);
    }

    return or_result;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_xor(const basic_N & lhs,
                                                                        const basic_N & rhs)
    -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    if (lhs.is_zero()) { return rhs; }
    if (rhs.is_zero()) { return lhs; }

    const basic_N * longest;
    const basic_N * shortest;

    if (lhs.digits_.size() < rhs.digits_.size()) {
        longest = &rhs;
        shortest = &lhs;
    } else {
        longest = &lhs;
        shortest = &rhs;
    }

    basic_N xor_result;
    xor_result.digits_.reserve(longest->digits_.size());

    std::size_t i = 0U;

    for (; i < shortest->digits_.size(); ++i) {
        xor_result.digits_.emplace_back(lhs.digits_[i] ^ rhs.digits_[i]);
    }

    for (; i < longest->digits_.size(); ++i) {
        xor_result.digits_.emplace_back(longest->digits_[i]);
    }

    xor_result.remove_leading_zeroes_();

    return xor_result;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_eq(const basic_N & lhs,
                                                                       const basic_N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return lhs.digits_ == rhs.digits_;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::strong_ordering basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_comp(
    const basic_N & lhs,
    const basic_N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (lhs.digits_.size() < rhs.digits_.size()) { return std::strong_ordering::less; }
    if (lhs.digits_.size() > rhs.digits_.size()) { return std::strong_ordering::greater; }

    for (auto crit_lhs = lhs.digits_.crbegin(), crit_rhs = rhs.digits_.crbegin();
         crit_lhs != lhs.digits_.crend();
         ++crit_lhs, ++crit_rhs) {
        if (*crit_lhs < *crit_rhs) { return std::strong_ordering::less; }
        if (*crit_lhs > *crit_rhs) { return std::strong_ordering::greater; }
    }

    return std::strong_ordering::equal;
}

}  // namespace jmaths
