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

    // ALGORITHM: Multi-precision addition with carry propagation
    // This implements the classic schoolbook addition algorithm for arbitrary-precision integers.
    // The number is stored as a vector of base_int_type digits in little-endian order
    // (least significant digit first).

    // check for additive identity
    if (lhs.is_zero()) { return rhs; }
    if (rhs.is_zero()) { return lhs; }

    // Optimization: identify the longer and shorter operands to minimize operations
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

    // Phase 1: Add corresponding digits from both operands with carry propagation
    // For each digit position, we compute: sum[i] = lhs[i] + rhs[i] + carry
    // A carry occurs when the sum would exceed the maximum digit value (radix - 1)
    for (; i < shortest->digits_.size(); ++i) {
        // Predict next carry: checks if lhs[i] + rhs[i] + carry >= radix
        // This is done by checking if lhs[i] < radix - rhs[i] - carry (to avoid overflow)
        const bool next_carry = !(lhs.digits_[i] < (carry ? max_digit : radix) - rhs.digits_[i]);

        sum.digits_.emplace_back(lhs.digits_[i] + rhs.digits_[i] +
                                 static_cast<base_int_type>(carry));
        carry = next_carry;
    }

    // Phase 2: Propagate carry through remaining digits of the longer operand
    if (carry) {
        // Continue adding carry to remaining digits
        for (; i < longest->digits_.size(); ++i) {
            sum.digits_.emplace_back(longest->digits_[i] + 1U);

            // If the digit was not at maximum, carry stops propagating
            if (longest->digits_[i] < max_digit) {
                ++i;
                goto loop_without_carry;
            }
        }

        // If carry propagates past all digits, we need an extra digit
        sum.digits_.emplace_back(1);

        goto end_of_function;
    }

loop_without_carry:

    // Phase 3: Copy remaining digits (no more carry to propagate)
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

    // ALGORITHM: Multi-precision subtraction with borrow propagation
    // This implements the classic schoolbook subtraction algorithm for arbitrary-precision integers.
    // PRECONDITION: lhs >= rhs (for efficiency, should only be called when lhs > rhs)
    // The algorithm borrows from higher-order digits when a digit in lhs is smaller than
    // the corresponding digit in rhs.

    // check for additive identity
    if (rhs.is_zero()) { return lhs; }

    basic_N difference;
    difference.digits_.reserve(lhs.digits_.size());

    std::size_t i = 0U;

    // Phase 1: Subtract corresponding digits with borrow propagation
    for (; i < rhs.digits_.size(); ++i) {
        // If current lhs digit is smaller than rhs digit, we need to borrow
        if (lhs.digits_[i] < rhs.digits_[i]) {
            // Borrow propagation: find the next non-zero digit and decrement it
            // All zero digits in between become max_digit after borrowing
            for (std::size_t j = i + 1U; j < lhs.digits_.size(); ++j) {
                if (lhs.digits_[j]-- > 0U) { break; }
                // If digit was 0, it becomes max_digit and we continue borrowing
            }
        }

        // Perform the subtraction (borrow is already incorporated in lhs)
        difference.digits_.emplace_back(lhs.digits_[i] - rhs.digits_[i]);
    }

    // Phase 2: Copy remaining digits from lhs
    for (; i < lhs.digits_.size(); ++i) {
        difference.digits_.emplace_back(lhs.digits_[i]);
    }

    // Remove any leading zeros that may have resulted from the subtraction
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

    // ALGORITHM: Karatsuba multiplication (work in progress)
    // This is a divide-and-conquer algorithm that reduces multiplication complexity
    // from O(n²) to O(n^log₂3) ≈ O(n^1.585)
    //
    // The algorithm splits each number into high and low parts:
    // lhs = lhs_l * B^m + lhs_r
    // rhs = rhs_l * B^m + rhs_r
    // where B is the base (radix) and m is half the number of digits
    //
    // Then computes: lhs * rhs = lhs_l*rhs_l * B^(2m) +
    //                            ((lhs_l + lhs_r)*(rhs_l + rhs_r) - lhs_l*rhs_l - lhs_r*rhs_r) * B^m +
    //                            lhs_r*rhs_r
    // This reduces 4 multiplications to 3, saving operations for large numbers.

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

    // ALGORITHM: Schoolbook multiplication (long multiplication)
    // This implements the classic O(n²) multiplication algorithm taught in elementary school.
    //
    // For two numbers with m and n digits respectively:
    // 1. Multiply the first number by each digit of the second number
    // 2. Shift each partial product by the appropriate number of positions
    // 3. Add all partial products together
    //
    // Example (in base 10): 123 * 45
    //     123
    //   x  45
    //   -----
    //     615  (123 * 5)
    //   4920   (123 * 4, shifted left by 1)
    //   -----
    //   5535
    //
    // Implementation uses a wider integer type (base_int_big_type) to handle
    // the multiplication of two digits without overflow, then splits the result
    // into the current digit and carry for the next position.

    // check for multiplicative identity
    if (lhs.is_one()) { return rhs; }
    if (rhs.is_one()) { return lhs; }

    // check for multiplicative zero
    if (lhs.is_zero() || rhs.is_zero()) { return basic_N{}; }

    basic_N product;

    // max overhead would be base_int_type_size
    product.digits_.reserve(lhs.digits_.size() + rhs.digits_.size());

    // Outer loop: iterate over each digit of lhs
    for (std::size_t i = 0U; i < lhs.digits_.size(); ++i) {
        // Create a partial product for lhs[i] * rhs
        basic_N temp1;
        temp1.digits_.reserve(i + rhs.digits_.size() + 1U);

        // Insert i zeros to represent multiplication by radix^i (positional shift)
        temp1.digits_.insert(temp1.digits_.begin(), i, 0U);
        base_int_type carry = 0U;

        // Inner loop: multiply lhs[i] by each digit of rhs
        for (std::size_t j = 0U; j < rhs.digits_.size(); ++j) {
            // Use wider type to prevent overflow in multiplication
            const base_int_big_type temp2 = static_cast<base_int_big_type>(lhs.digits_[i]) *
                                            static_cast<base_int_big_type>(rhs.digits_[j]);

            // Extract lower part (current digit)
            const base_int_type temp3 = static_cast<base_int_type>(temp2);

            // Add carry and current product to result
            temp1.digits_.emplace_back(carry + temp3);

            // Calculate carry for next iteration:
            // 1. Check if adding carry to temp3 caused overflow
            const base_int_type temp_carry = static_cast<base_int_type>(
                (static_cast<base_int_big_type>(carry) + static_cast<base_int_big_type>(temp3)) >>
                base_int_type_bits);

            // 2. Add upper part of multiplication plus overflow from addition
            carry = static_cast<base_int_type>((temp2 >> base_int_type_bits) + temp_carry);
        }

        // If there's remaining carry, add it as a new digit
        if (carry != 0U) { temp1.digits_.emplace_back(carry); }

        // Add this partial product to the running total
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

    // ALGORITHM: Binary long division (restoring division)
    // This implements a bit-by-bit division algorithm similar to long division taught in school,
    // but working in binary. It's called "restoring" because when the remainder becomes negative,
    // we restore it by adding back the divisor.
    //
    // The algorithm:
    // 1. Initialize quotient q = 0 and remainder r = 0
    // 2. For each bit in the dividend (from most significant to least):
    //    a. Shift remainder left by 1 bit (r = r << 1)
    //    b. Set the least significant bit of r to the current dividend bit
    //    c. If r >= divisor:
    //       - Subtract divisor from r (r = r - divisor)
    //       - Set the corresponding bit in quotient to 1
    // 3. Return (quotient, remainder)
    //
    // Time complexity: O(n) where n is the number of bits in the dividend
    // This is more efficient than repeated subtraction but slower than more advanced
    // algorithms like Newton-Raphson division for very large numbers.

    if (lhs.is_zero()) { return {basic_N{}, basic_N{}}; }

    // check if lhs == rhs
    if (opr_eq(lhs, rhs)) { return {one_, basic_N{}}; }

    std::pair<basic_N, basic_N> result;

    auto & [q, r] = result;

    // Pre-allocate memory for quotient to avoid reallocations
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

    // Main division loop: process each bit from most significant to least significant
    // Calculate the actual number of significant bits in lhs
    for (bitpos_t i = lhs.digits_.size() * base_int_type_bits -
                      static_cast<bitcount_t>(std::countl_zero(lhs.digits_.back()));
         i-- > 0U;) {
        // Shift remainder left and bring down next bit from dividend
        r.opr_bitshift_l_assign_(1U);
        r.bit_(0U, lhs.bit_(i));

        // If remainder >= divisor, subtract and set quotient bit
        if (opr_comp(r, rhs) >= 0) {
            r.opr_subtr_assign_(rhs);
            q.bit_(i, 1);
        }
    }

    // Verify the division: quotient * divisor + remainder should equal dividend
    assert(q * rhs + r == lhs);

    return result;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_and(const basic_N & lhs,
                                                                        const basic_N & rhs)
    -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Bitwise AND operation
    // Performs digit-by-digit bitwise AND between two arbitrary-precision integers.
    // The result has at most as many digits as the shorter operand, since
    // ANDing with implicit zeros gives zero.

    if (lhs.is_zero() || rhs.is_zero()) { return basic_N{}; }

    const basic_N & shortest = lhs.digits_.size() < rhs.digits_.size() ? lhs : rhs;

    basic_N and_result;
    and_result.digits_.reserve(shortest.digits_.size());

    for (std::size_t i = 0U; i < shortest.digits_.size(); ++i) {
        and_result.digits_.emplace_back(lhs.digits_[i] & rhs.digits_[i]);
    }

    // Remove leading zeros since ANDing may produce zeros at high-order positions
    and_result.remove_leading_zeroes_();

    return and_result;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_or(const basic_N & lhs,
                                                                       const basic_N & rhs)
    -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Bitwise OR operation
    // Performs digit-by-digit bitwise OR between two arbitrary-precision integers.
    // The result has as many digits as the longer operand, since ORing with
    // implicit zeros preserves the bit values.

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

    // OR corresponding digits where both operands have digits
    for (; i < shortest->digits_.size(); ++i) {
        or_result.digits_.emplace_back(lhs.digits_[i] | rhs.digits_[i]);
    }

    // Copy remaining digits from longer operand (OR with 0 = identity)
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

    // ALGORITHM: Bitwise XOR operation
    // Performs digit-by-digit bitwise XOR between two arbitrary-precision integers.
    // XOR is useful for cryptography and error detection. Properties:
    // - a XOR 0 = a (identity)
    // - a XOR a = 0 (self-inverse)
    // - a XOR b = b XOR a (commutative)

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

    // XOR corresponding digits where both operands have digits
    for (; i < shortest->digits_.size(); ++i) {
        xor_result.digits_.emplace_back(lhs.digits_[i] ^ rhs.digits_[i]);
    }

    // Copy remaining digits from longer operand (XOR with 0 = identity)
    for (; i < longest->digits_.size(); ++i) {
        xor_result.digits_.emplace_back(longest->digits_[i]);
    }

    // Remove leading zeros since XOR may produce zeros at high-order positions
    xor_result.remove_leading_zeroes_();

    return xor_result;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_eq(const basic_N & lhs,
                                                                       const basic_N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Equality comparison
    // Two numbers are equal if and only if they have the same digit representation.
    // Since leading zeros are always removed, vector equality is sufficient.

    return lhs.digits_ == rhs.digits_;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::strong_ordering basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_comp(
    const basic_N & lhs,
    const basic_N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Three-way comparison (spaceship operator)
    // Compares two arbitrary-precision unsigned integers.
    //
    // Strategy:
    // 1. First compare the number of digits - more digits means larger number
    //    (since leading zeros are always removed)
    // 2. If same number of digits, compare digit-by-digit from most significant
    //    to least significant (like comparing strings lexicographically)
    //
    // Time complexity: O(1) best case (different sizes), O(n) worst case (same size)

    // Quick comparison by number of digits
    if (lhs.digits_.size() < rhs.digits_.size()) { return std::strong_ordering::less; }
    if (lhs.digits_.size() > rhs.digits_.size()) { return std::strong_ordering::greater; }

    // Same number of digits: compare from most significant to least significant
    for (auto crit_lhs = lhs.digits_.crbegin(), crit_rhs = rhs.digits_.crbegin();
         crit_lhs != lhs.digits_.crend();
         ++crit_lhs, ++crit_rhs) {
        if (*crit_lhs < *crit_rhs) { return std::strong_ordering::less; }
        if (*crit_lhs > *crit_rhs) { return std::strong_ordering::greater; }
    }

    return std::strong_ordering::equal;
}

}  // namespace jmaths
