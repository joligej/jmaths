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

#include "calc.hpp"

#include <algorithm>
#include <cassert>
#include <utility>

#include "N.hpp"
#include "Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "detail.hpp"
#include "error.hpp"

namespace jmaths {

N calc::gcd(N a, N b) {
    JMATHS_FUNCTION_TO_LOG;

    if (a.is_zero()) { return b; }
    if (b.is_zero()) { return a; }

    const bitcount_t i = a.ctz();
    const bitcount_t j = b.ctz();

    a.opr_bitshift_r_assign_(i);
    b.opr_bitshift_r_assign_(j);

    const bitcount_t k = std::min(i, j);

    for (;;) {
        assert(a.is_odd());
        assert(b.is_odd());

        if (detail::opr_comp(a, b) > 0) { a.digits_.swap(b.digits_); }

        b.opr_subtr_assign_(a);

        if (b.is_zero()) { return a.opr_bitshift_l_(k); }

        b.opr_bitshift_r_assign_(b.ctz());
    }
}

std::pair<N, N> calc::sqrt(const N & num) {
    JMATHS_FUNCTION_TO_LOG;

    if (num.is_zero() || num.is_one()) { return {num, N{}}; }

    N start = N::one_, end = num.opr_bitshift_r_(1U), ans;

    while (detail::opr_comp(start, end) <= 0) {
        N mid = detail::opr_add(start, end);
        mid.opr_bitshift_r_assign_(1U);

        const N sqr = detail::opr_mult(mid, mid);

        const auto compared = detail::opr_comp(sqr, num);

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

    N remainder = detail::opr_subtr(num, detail::opr_mult(ans, ans));

    return {std::move(ans), std::move(remainder)};
}

N calc::sqrt_whole(const N & num) {
    JMATHS_FUNCTION_TO_LOG;

    if (num.is_zero() || num.is_one()) { return num; }

    N start = N::one_, end = num.opr_bitshift_r_(1U), ans;

    while (detail::opr_comp(start, end) <= 0) {
        N mid = detail::opr_add(start, end).opr_bitshift_r_(1U);

        const N sqr = detail::opr_mult(mid, mid);

        const auto compared = detail::opr_comp(sqr, num);

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

N calc::pow(N base, N exponent) {
    JMATHS_FUNCTION_TO_LOG;

    if (exponent.is_zero()) { return N::one_; }

    N result(N::one_);

    for (;;) {
        if (exponent.is_odd()) { result.opr_mult_assign_(base); }
        exponent.opr_bitshift_r_assign_(1U);
        if (exponent.is_zero()) { break; }
        base.opr_mult_assign_(base);
    }

    return result;
}

N calc::pow_mod(N base, N exponent, const N & mod) {
    JMATHS_FUNCTION_TO_LOG;

    if (mod.is_zero()) { throw error::division_by_zero{}; }

    if (exponent.is_zero()) { return N::one_; }

    N result(N::one_);

    for (;;) {
        if (exponent.is_odd()) {
            result.opr_mult_assign_(base);
            result =
                detail::opr_div(result, mod).second;  // maybe use separate function just for mod ???
        }

        exponent.opr_bitshift_r_assign_(1U);
        if (exponent.is_zero()) { break; }
        base.opr_mult_assign_(base);
    }

    return result;
}

Z calc::pow(Z base, N exponent) {
    JMATHS_FUNCTION_TO_LOG;

    const auto sign =
        base.is_negative() && exponent.is_odd() ? sign_type::negative : sign_type::positive;

    return {pow(std::move(std::move(base).abs()), std::move(exponent)), sign};
}

}  // namespace jmaths

#include "undef.hh"
