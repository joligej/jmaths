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

    // ALGORITHM: Binary GCD (Stein's algorithm)
    // This is an efficient algorithm for computing the Greatest Common Divisor (GCD)
    // of two integers. It's faster than the Euclidean algorithm for binary computers
    // because it uses bit shifts and subtraction instead of division.
    //
    // The algorithm is based on these properties:
    // 1. gcd(0, n) = n
    // 2. If both numbers are even: gcd(2a, 2b) = 2 * gcd(a, b)
    // 3. If one is even and one is odd: gcd(2a, b) = gcd(a, b) where b is odd
    // 4. If both are odd: gcd(a, b) = gcd(|a-b|, min(a,b))
    //
    // Steps:
    // 1. Handle base cases (either number is 0)
    // 2. Find common factor of 2 by counting trailing zeros in both numbers
    // 3. Remove all factors of 2 from both numbers
    // 4. Repeatedly subtract smaller from larger and remove factors of 2
    // 5. Multiply result by the common factor of 2 found in step 2

    using basic_N_t = std::decay_t<basic_N_type_1>;

    if (a.is_zero()) { return std::forward<decltype(b)>(b); }
    if (b.is_zero()) { return std::forward<decltype(a)>(a); }

    TMP::ref_or_copy_t<decltype(a)> num1 = a;
    TMP::ref_or_copy_t<decltype(b)> num2 = b;

    // Count trailing zeros (factors of 2) in both numbers
    const bitcount_t i = num1.ctz();
    const bitcount_t j = num2.ctz();

    // Remove all factors of 2 from both numbers
    num1.opr_bitshift_r_assign_(i);
    num2.opr_bitshift_r_assign_(j);

    // k = common power of 2 that divides both numbers
    const bitcount_t k = std::min(i, j);

    // Main loop: both numbers are now odd
    for (;;) {
        assert(num1.is_odd());
        assert(num2.is_odd());

        // Ensure num1 <= num2 (swap if necessary)
        if (basic_N_t::detail::opr_comp(num1, num2) > 0) { num1.digits_.swap(num2.digits_); }

        // Subtract: num2 = num2 - num1 (difference is even since both are odd)
        num2.opr_subtr_assign_(num1);

        if (num2.is_zero()) { return num1.opr_bitshift_l_(k); }

        // Remove all factors of 2 from num2 (make it odd again)
        num2.opr_bitshift_r_assign_(num2.ctz());
    }
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr auto calc::sqrt(basic_N_type && num)
    -> std::pair<std::decay_t<basic_N_type>, std::decay_t<basic_N_type>> {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Integer square root using binary search
    // Finds the largest integer x such that x² ≤ num, plus the remainder.
    // Returns (sqrt, remainder) where num = sqrt² + remainder
    //
    // The algorithm uses binary search in the range [1, num/2]:
    // 1. If num is 0 or 1, return it directly
    // 2. Set search range: start = 1, end = num/2
    // 3. Binary search:
    //    - Calculate mid = (start + end) / 2
    //    - If mid² = num: return (mid, 0)
    //    - If mid² < num: move start up, remember this as potential answer
    //    - If mid² > num: move end down
    // 4. Calculate remainder = num - answer²
    //
    // Time complexity: O(log n) iterations, each with O(n²) multiplication

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
            // mid² < num: this could be our answer, but try larger values
            ans = mid;
            mid.opr_incr_();
            start = std::move(mid);
        } else {
            // mid² > num: try smaller values
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

    // ALGORITHM: Integer square root (without remainder)
    // Same as sqrt() but only returns the integer part, not the remainder.
    // Slightly optimized since we don't need to calculate the remainder.

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

    // ALGORITHM: Exponentiation by squaring (binary exponentiation)
    // Computes base^exponent efficiently using repeated squaring.
    // This is much faster than repeated multiplication for large exponents.
    //
    // The algorithm is based on the binary representation of the exponent:
    // For example, 3^13 = 3^(1101₂) = 3^8 * 3^4 * 3^1
    //
    // Steps:
    // 1. Initialize result = 1
    // 2. For each bit in exponent (from least to most significant):
    //    - If bit is 1: multiply result by current base power
    //    - Square the base for the next bit position
    // 3. Return result
    //
    // Time complexity: O(log exponent) multiplications instead of O(exponent)
    // This makes huge exponents feasible (e.g., 2^1000000)

    using basic_N_t = std::decay_t<basic_N_type_1>;

    if (exponent.is_zero()) { return basic_N_t::one_; }

    TMP::ref_or_copy_t<decltype(base)> base_num = base;
    TMP::ref_or_copy_t<decltype(exponent)> exponent_num = exponent;

    basic_N_t result(basic_N_t::one_);

    for (;;) {
        // If exponent is odd, multiply result by current base power
        if (exponent_num.is_odd()) { result.opr_mult_assign_(base_num); }

        // Divide exponent by 2 (right shift by 1)
        exponent_num.opr_bitshift_r_assign_(1U);
        if (exponent_num.is_zero()) { break; }

        // Square the base for next iteration
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

    // ALGORITHM: Modular exponentiation by squaring
    // Computes (base^exponent) mod mod efficiently.
    // This is crucial for cryptographic applications (e.g., RSA encryption).
    //
    // Similar to regular exponentiation by squaring, but we apply the modulo
    // operation after each multiplication to keep numbers small and prevent overflow.
    // This is based on the property: (a * b) mod m = ((a mod m) * (b mod m)) mod m
    //
    // Without this optimization, computing something like (2^1000000) mod 17
    // would require storing an astronomically large intermediate result.
    // With this algorithm, intermediate values never exceed mod².
    //
    // Time complexity: O(log exponent) multiplications and modulo operations

    using basic_N_t = std::decay_t<basic_N_type_1>;

    error::division_by_zero::check(mod);

    if (exponent.is_zero()) { return basic_N_t::one_; }

    TMP::ref_or_copy_t<decltype(base)> base_num = base;
    TMP::ref_or_copy_t<decltype(exponent)> exponent_num = exponent;

    basic_N_t result(basic_N_t::one_);

    for (;;) {
        if (exponent_num.is_odd()) {
            result.opr_mult_assign_(base_num);
            // Apply modulo to keep result small
            result = basic_N_t::detail::opr_div(result, mod)
                         .second;  // .second is the remainder
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
