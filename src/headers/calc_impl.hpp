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
#include <ratio>
#include <type_traits>
#include <utility>

#include "TMP.hpp"
#include "basic_N.hpp"
#include "basic_Z.hpp"
#include "calc.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "error.hpp"

// member functions of calc
namespace jmaths {

template <TMP::instance_of<basic_N> basic_N_type_1, TMP::decays_to<basic_N_type_1> basic_N_type_2>
constexpr auto calc::gcd(basic_N_type_1 && a, basic_N_type_2 && b) -> std::decay_t<basic_N_type_1> {
    JMATHS_FUNCTION_TO_LOG;

    using basic_N_t = std::decay_t<basic_N_type_1>;

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

        if (basic_N_t::detail::opr_comp(num1, num2) > 0) { num1.digits_.swap(num2.digits_); }

        num2.opr_subtr_assign_(num1);

        if (num2.is_zero()) { return num1.opr_bitshift_l_(k); }

        num2.opr_bitshift_r_assign_(num2.ctz());
    }
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr auto calc::sqrt(basic_N_type && num)
    -> std::pair<std::decay_t<basic_N_type>, std::decay_t<basic_N_type>> {
    JMATHS_FUNCTION_TO_LOG;

    using basic_N_t = std::decay_t<basic_N_type>;

    if (num.is_zero() || num.is_one()) { return {std::forward<decltype(num)>(num), basic_N_t{}}; }

    basic_N_t start = basic_N_t::one_, end = num.opr_bitshift_r_(1U), ans;

    while (basic_N_t::detail::opr_comp(start, end) <= 0) {
        basic_N_t mid = basic_N_t::detail::opr_add(start, end);
        mid.opr_bitshift_r_assign_(1U);

        const basic_N_t sqr = basic_N_t::detail::opr_mult(mid, mid);

        const auto compared = basic_N_t::detail::opr_comp(sqr, num);

        if (compared == 0) { return {std::move(mid), basic_N_t{}}; }

        if (compared < 0) {
            ans = mid;
            mid.opr_incr_();
            start = std::move(mid);
        } else {
            mid.opr_decr_();
            end = std::move(mid);
        }
    }

    basic_N_t remainder = basic_N_t::detail::opr_subtr(num, basic_N_t::detail::opr_mult(ans, ans));

    return {std::move(ans), std::move(remainder)};
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr auto calc::sqrt_whole(basic_N_type && num) -> std::decay_t<basic_N_type> {
    JMATHS_FUNCTION_TO_LOG;

    using basic_N_t = std::decay_t<basic_N_type>;

    if (num.is_zero() || num.is_one()) { return std::forward<decltype(num)>(num); }

    basic_N_t start = basic_N_t::one_, end = num.opr_bitshift_r_(1U), ans;

    while (basic_N_t::detail::opr_comp(start, end) <= 0) {
        basic_N_t mid = basic_N_t::detail::opr_add(start, end).opr_bitshift_r_(1U);

        const basic_N_t sqr = basic_N_t::detail::opr_mult(mid, mid);

        const auto compared = basic_N_t::detail::opr_comp(sqr, num);

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

template <TMP::instance_of<basic_N> basic_N_type_1, TMP::decays_to<basic_N_type_1> basic_N_type_2>
constexpr auto calc::pow(basic_N_type_1 && base, basic_N_type_2 && exponent)
    -> std::decay_t<basic_N_type_1> {
    JMATHS_FUNCTION_TO_LOG;

    using basic_N_t = std::decay_t<basic_N_type_1>;

    if (exponent.is_zero()) { return basic_N_t::one_; }

    TMP::ref_or_copy_t<decltype(base)> base_num = base;
    TMP::ref_or_copy_t<decltype(exponent)> exponent_num = exponent;

    basic_N_t result(basic_N_t::one_);

    for (;;) {
        if (exponent_num.is_odd()) { result.opr_mult_assign_(base_num); }
        exponent_num.opr_bitshift_r_assign_(1U);
        if (exponent_num.is_zero()) { break; }
        base_num.opr_mult_assign_(base_num);
    }

    return result;
}

template <TMP::instance_of<basic_N> basic_N_type_1, TMP::decays_to<basic_N_type_1> basic_N_type_2>
constexpr auto calc::pow_mod(basic_N_type_1 && base,
                             basic_N_type_2 && exponent,
                             const std::decay_t<basic_N_type_1> & mod)
    -> std::decay_t<basic_N_type_1> {
    JMATHS_FUNCTION_TO_LOG;

    using basic_N_t = std::decay_t<basic_N_type_1>;

    error::division_by_zero::check(mod);

    if (exponent.is_zero()) { return basic_N_t::one_; }

    TMP::ref_or_copy_t<decltype(base)> base_num = base;
    TMP::ref_or_copy_t<decltype(exponent)> exponent_num = exponent;

    basic_N_t result(basic_N_t::one_);

    for (;;) {
        if (exponent_num.is_odd()) {
            result.opr_mult_assign_(base_num);
            result = basic_N_t::detail::opr_div(result, mod)
                         .second;  // maybe use separate function just for mod ???
        }

        exponent_num.opr_bitshift_r_assign_(1U);
        if (exponent_num.is_zero()) { break; }
        base_num.opr_mult_assign_(base_num);
    }

    return result;
}

template <TMP::instance_of<basic_Z> basic_Z_type, TMP::instance_of<basic_N> basic_N_type>
constexpr auto calc::pow(basic_Z_type && base, basic_N_type && exponent)
    -> std::decay_t<basic_Z_type> {
    JMATHS_FUNCTION_TO_LOG;

    const auto sign =
        base.is_negative() && exponent.is_odd() ? sign_type::negative : sign_type::positive;

    return {
        pow(std::forward<decltype(base)>(base).abs(), std::forward<decltype(exponent)>(exponent)),
        sign};
}

}  // namespace jmaths
