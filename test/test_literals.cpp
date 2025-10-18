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

#include <boost/test/unit_test.hpp>

#include "all.hpp"

using namespace jmaths;
using namespace jmaths::literals;

/**
 * @file test_literals.cpp
 * @brief Tests for user-defined literals (_N, _Z, _Q)
 *
 * This file tests the user-defined literal operators that provide convenient
 * syntax for creating arbitrary-precision numbers: _N for unsigned, _Z for
 * signed, and _Q for rational numbers.
 */

BOOST_AUTO_TEST_SUITE(literals_tests)

// ============================================================================
// _N Literal Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(n_literal_tests)

BOOST_AUTO_TEST_CASE(n_literal_small) {
    auto num = 42_N;
    BOOST_TEST(num == 42);
}

BOOST_AUTO_TEST_CASE(n_literal_large) {
    auto num = 123456789012345678901234567890_N;
    BOOST_TEST(num.to_str() == "123456789012345678901234567890");
}

BOOST_AUTO_TEST_CASE(n_literal_zero_and_one) {
    auto zero = 0_N;
    BOOST_TEST(zero == 0);

    auto one = 1_N;
    BOOST_TEST(one == N(1));
}

BOOST_AUTO_TEST_CASE(n_literal_boundary_values) {
    // Test power of 2 and max uint64
    auto power = 1024_N;
    BOOST_TEST(power == N(1024));

    auto max_val = 18446744073709551615_N;
    BOOST_TEST(max_val.to_str() == "18446744073709551615");
}

BOOST_AUTO_TEST_CASE(n_literal_constexpr) {
    auto num = 999_N;
    BOOST_TEST(num == N(999));
}

BOOST_AUTO_TEST_CASE(n_literal_constexpr_operations) {
    auto sum = 10_N + 20_N;
    BOOST_TEST(sum == N(30));
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// _Z Literal Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(z_literal_tests)

BOOST_AUTO_TEST_CASE(z_literal_positive) {
    auto num = 42_Z;
    BOOST_TEST(num == 42);
    BOOST_TEST(num > 0);
}

BOOST_AUTO_TEST_CASE(z_literal_negative) {
    auto num = -42_Z;
    BOOST_TEST(num == -42);
    BOOST_TEST(num < 0);
}

BOOST_AUTO_TEST_CASE(z_literal_zero_one_minus_one) {
    auto zero = 0_Z;
    BOOST_TEST(zero == 0);

    auto one = 1_Z;
    BOOST_TEST(one.is_positive());
    BOOST_TEST(one == Z(1));

    auto minus_one = -1_Z;
    BOOST_TEST(minus_one.is_negative());
    BOOST_TEST(minus_one == Z(-1));
}

BOOST_AUTO_TEST_CASE(z_literal_extreme_values) {
    auto large_pos = 9999999999999999999_Z;
    BOOST_TEST(large_pos > Z(0));

    auto large_neg = -9999999999999999999_Z;
    BOOST_TEST(large_neg < Z(0));
}

BOOST_AUTO_TEST_CASE(z_literal_constexpr_operations) {
    auto diff = 100_Z - 42_Z;
    BOOST_TEST(diff == Z(58));
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// _Q Literal Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(q_literal_tests)

BOOST_AUTO_TEST_CASE(q_literal_special_values) {
    // Test zero, one, and basic integers
    auto zero = 0_Q;
    BOOST_TEST(zero == Q("0/1"));

    auto one = 1_Q;
    BOOST_TEST(one == Q("1/1"));

    auto integer = 42_Q;
    BOOST_TEST(integer.to_str() == "42/1");

    auto negative = -42_Q;
    BOOST_TEST(negative.to_str() == "-42/1");
}

BOOST_AUTO_TEST_CASE(q_literal_large_integers) {
    auto large = 123456789_Q;
    BOOST_TEST(large.to_str() == "123456789/1");
    BOOST_TEST(large == Q("123456789/1"));

    auto negative_large = -987654321_Q;
    BOOST_TEST(negative_large.to_str() == "-987654321/1");
    BOOST_TEST(negative_large == Q("-987654321/1"));
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Mixed Literal Arithmetic Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(literal_arithmetic_tests)

BOOST_AUTO_TEST_CASE(literal_arithmetic) {
    auto result = 10_N + 20_N;
    BOOST_TEST(result == 30);

    auto result2 = 50_Z - 20_Z;
    BOOST_TEST(result2 == 30);
}

BOOST_AUTO_TEST_CASE(n_literal_multiplication) {
    auto result = 5_N * 7_N;
    BOOST_TEST(result == N(35));
}

BOOST_AUTO_TEST_CASE(n_literal_division) {
    auto [quotient, remainder] = 100_N / 5_N;  // Division returns pair<N,N> (quotient, remainder)
    BOOST_TEST(quotient == N(20));
    BOOST_TEST(remainder == N(0));
}

BOOST_AUTO_TEST_CASE(z_literal_mixed_signs_add) {
    auto result = 50_Z + (-20_Z);
    BOOST_TEST(result == Z(30));
}

BOOST_AUTO_TEST_CASE(z_literal_mixed_signs_multiply) {
    auto result = 6_Z * (-7_Z);
    BOOST_TEST(result == Z(-42));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
