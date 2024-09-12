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
#include <cassert>
#include <utility>

#include "N.hpp"
#include "TMP.hpp"
#include "Z.hpp"
#include "calc.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "error.hpp"

// member functions of calc
namespace jmaths {

N calc::gcd(TMP::decays_to<N> auto && a, TMP::decays_to<N> auto && b) {
    JMATHS_FUNCTION_TO_LOG;

    if (a.is_zero()) { return std::forward<decltype(b)>(b); }
    if (b.is_zero()) { return std::forward<decltype(a)>(a); }

    TMP::ref_or_copy_t<decltype(a)> num1 = a;
    TMP::ref_or_copy_t<decltype(b)> num2 = b;

    const bitcount_t i = num1.ctz();
    const bitcount_t j = num2.ctz();

    num1.opr_bitshift_r_assign_(i);
    num2.opr_bitshift_r_assign_(j);

    const bitcount_t k = std::min(i, j);

    for (;;) {
        assert(num1.is_odd());
        assert(num2.is_odd());

        if (N::detail::opr_comp(num1, num2) > 0) { num1.digits_.swap(num2.digits_); }

        num2.opr_subtr_assign_(num1);

        if (num2.is_zero()) { return num1.opr_bitshift_l_(k); }

        num2.opr_bitshift_r_assign_(num2.ctz());
    }
}

std::pair<N, N> calc::sqrt(TMP::decays_to<N> auto && num) {
    JMATHS_FUNCTION_TO_LOG;

    if (num.is_zero() || num.is_one()) { return {std::forward<decltype(num)>(num), N{}}; }

    N start = N::one_, end = num.opr_bitshift_r_(1U), ans;

    while (N::detail::opr_comp(start, end) <= 0) {
        N mid = N::detail::opr_add(start, end);
        mid.opr_bitshift_r_assign_(1U);

        const N sqr = N::detail::opr_mult(mid, mid);

        const auto compared = N::detail::opr_comp(sqr, num);

        if (compared == 0) { return {std::move(mid), N{}}; }

        if (compared < 0) {
            ans = mid;
            mid.opr_incr_();
            start = std::move(mid);
        } else {
            mid.opr_decr_();
            end = std::move(mid);
        }
    }

    N remainder = N::detail::opr_subtr(num, N::detail::opr_mult(ans, ans));

    return {std::move(ans), std::move(remainder)};
}

N calc::sqrt_whole(TMP::decays_to<N> auto && num) {
    JMATHS_FUNCTION_TO_LOG;

    if (num.is_zero() || num.is_one()) { return std::forward<decltype(num)>(num); }

    N start = N::one_, end = num.opr_bitshift_r_(1U), ans;

    while (N::detail::opr_comp(start, end) <= 0) {
        N mid = N::detail::opr_add(start, end).opr_bitshift_r_(1U);

        const N sqr = N::detail::opr_mult(mid, mid);

        const auto compared = N::detail::opr_comp(sqr, num);

        if (compared == 0) { return mid; }

        if (compared < 0) {
            ans = mid;
            mid.opr_incr_();
            start = std::move(mid);
        } else {
            mid.opr_decr_();
            end = std::move(mid);
        }
    }

    return ans;
}

N calc::pow(TMP::decays_to<N> auto && base, TMP::decays_to<N> auto && exponent) {
    JMATHS_FUNCTION_TO_LOG;

    if (exponent.is_zero()) { return N::one_; }

    TMP::ref_or_copy_t<decltype(base)> base_num = base;
    TMP::ref_or_copy_t<decltype(exponent)> exponent_num = exponent;

    N result(N::one_);

    for (;;) {
        if (exponent_num.is_odd()) { result.opr_mult_assign_(base_num); }
        exponent_num.opr_bitshift_r_assign_(1U);
        if (exponent_num.is_zero()) { break; }
        base_num.opr_mult_assign_(base_num);
    }

    return result;
}

N calc::pow_mod(TMP::decays_to<N> auto && base, TMP::decays_to<N> auto && exponent, const N & mod) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(mod);

    if (exponent.is_zero()) { return N::one_; }

    TMP::ref_or_copy_t<decltype(base)> base_num = base;
    TMP::ref_or_copy_t<decltype(exponent)> exponent_num = exponent;

    N result(N::one_);

    for (;;) {
        if (exponent_num.is_odd()) {
            result.opr_mult_assign_(base_num);
            result = N::detail::opr_div(result, mod)
                         .second;  // maybe use separate function just for mod ???
        }

        exponent_num.opr_bitshift_r_assign_(1U);
        if (exponent_num.is_zero()) { break; }
        base_num.opr_mult_assign_(base_num);
    }

    return result;
}

Z calc::pow(TMP::decays_to<Z> auto && base, TMP::decays_to<N> auto && exponent) {
    JMATHS_FUNCTION_TO_LOG;

    const auto sign =
        base.is_negative() && exponent.is_odd() ? sign_type::negative : sign_type::positive;

    return {
        pow(std::forward<decltype(base)>(base).abs(), std::forward<decltype(exponent)>(exponent)),
        sign};
}

}  // namespace jmaths
