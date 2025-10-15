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
#include <limits>
#include <stdexcept>

#include "all.hpp"

using namespace jmaths;

BOOST_AUTO_TEST_SUITE(edge_cases_tests)

// Test very large numbers
BOOST_AUTO_TEST_CASE(very_large_number_operations) {
    // Create numbers with hundreds of digits
    std::string large1(300, '9');
    std::string large2(300, '8');

    N n1(large1);
    N n2(large2);

    // Addition should work
    N sum = n1 + n2;
    BOOST_TEST(sum > n1);
    BOOST_TEST(sum > n2);

    // Multiplication should work
    N product = n1 * N(2);
    BOOST_TEST(product > n1);
}

// Test operations with zero
BOOST_AUTO_TEST_CASE(operations_with_zero) {
    N zero(0);
    N nonzero(42);

    BOOST_TEST((zero + nonzero) == nonzero);
    BOOST_TEST((nonzero + zero) == nonzero);
    BOOST_TEST((zero * nonzero) == zero);
    BOOST_TEST((nonzero * zero) == zero);
    BOOST_TEST((zero - zero) == zero);
}

// Test boundary conditions for N
BOOST_AUTO_TEST_CASE(n_boundary_conditions) {
    N one(1);
    N max_small(std::numeric_limits<std::uint32_t>::max());

    // Test increment past uint32 boundary
    N result = max_small + one;
    BOOST_TEST(result > max_small);
}

// Test Z sign changes
BOOST_AUTO_TEST_CASE(z_sign_transitions) {
    Z positive(10);
    Z negative(-10);
    Z zero(0);

    // Cross zero boundary
    BOOST_TEST((positive + negative) == zero);
    BOOST_TEST((negative + positive) == zero);

    // Sign preservation
    BOOST_TEST((positive * positive) > zero);
    BOOST_TEST((negative * negative) > zero);
    BOOST_TEST((positive * negative) < zero);
}

// Test Q edge cases
BOOST_AUTO_TEST_CASE(q_edge_cases) {
    Q half("1/2");
    Q third("1/3");
    Q zero("0/1");

    // Very small fractions
    Q tiny("1/1000000000");
    BOOST_TEST(tiny.to_str().find("1000000000") != std::string::npos);

    // Addition creating common denominator
    Q sum = half + third;
    // 1/2 + 1/3 = 5/6
    BOOST_TEST(sum.to_str() == "5/6");
}

// Test division by zero attempts
BOOST_AUTO_TEST_CASE(division_by_zero_protection) {
    N numerator(42);
    N zero(0);

    // Should throw division_by_zero error
    BOOST_CHECK_THROW(numerator / zero, error::division_by_zero);
}

// Test string parsing edge cases
BOOST_AUTO_TEST_CASE(string_parsing_edge_cases) {
    // Leading zeros
    N n1("00042");
    BOOST_TEST(n1 == 42);

    // Very long string
    std::string long_num(1000, '1');
    N n2(long_num);
    BOOST_TEST(n2.to_str().length() == 1000);
}

// Test comparison edge cases
BOOST_AUTO_TEST_CASE(comparison_edge_cases) {
    N a(100);
    N b(100);
    N c(101);

    BOOST_TEST((a == b));
    BOOST_TEST(!(a == c));
    BOOST_TEST((a != c));
    BOOST_TEST((a < c));
    BOOST_TEST((c > a));
    BOOST_TEST((a <= b));
    BOOST_TEST((a >= b));
}

// Test power of zero and one
BOOST_AUTO_TEST_CASE(power_special_cases) {
    N base(42);
    N zero(0);
    N one(1);

    // Any number to power 0 is 1
    N result1 = calc::pow(base, zero);
    BOOST_TEST(result1 == 1);

    // Any number to power 1 is itself
    N result2 = calc::pow(base, one);
    BOOST_TEST(result2 == base);

    // 0 to any positive power is 0
    N result3 = calc::pow(zero, base);
    BOOST_TEST(result3 == 0);
}

// Test GCD edge cases
BOOST_AUTO_TEST_CASE(gcd_edge_cases) {
    N zero(0);
    N five(5);

    // GCD with zero
    N result1 = calc::gcd(five, zero);
    BOOST_TEST(result1 == five);

    N result2 = calc::gcd(zero, five);
    BOOST_TEST(result2 == five);

    // GCD of same number
    N result3 = calc::gcd(five, five);
    BOOST_TEST(result3 == five);
}

// Test modulo operations
BOOST_AUTO_TEST_CASE(modulo_edge_cases) {
    N a(100);
    N b(7);
    N c(100);

    auto [q1, mod1] = a / b;
    BOOST_TEST(mod1 < b);

    // Modulo by same number
    auto [q2, mod2] = a / c;
    BOOST_TEST(mod2 == 0);
}

// Test bitwise operations on zero
BOOST_AUTO_TEST_CASE(bitwise_operations_zero) {
    N zero(0);
    N ones(0xFF);

    BOOST_TEST((zero & ones) == zero);
    BOOST_TEST((zero | ones) == ones);
    BOOST_TEST((zero ^ ones) == ones);
}

// Test overflow prevention
BOOST_AUTO_TEST_CASE(overflow_prevention) {
    // Create very large numbers that would overflow standard types
    N large1("18446744073709551615");  // Max uint64
    N large2("18446744073709551615");

    // Addition should not overflow
    N sum = large1 + large2;
    BOOST_TEST(sum > large1);
    BOOST_TEST(sum > large2);

    // Multiplication should not overflow
    N product = large1 * N(2);
    BOOST_TEST(product > large1);
}

// Test mixed-type operations consistency
BOOST_AUTO_TEST_CASE(mixed_type_consistency) {
    N n(10);
    Z z(10);

    // Both should give same results for positive operations
    N n_result = n + N(5);
    Z z_result = z + Z(5);
    BOOST_TEST(n_result == 15);
    BOOST_TEST(z_result == 15);
}

// Test Q normalization
BOOST_AUTO_TEST_CASE(q_normalization) {
    // These should normalize to same value
    Q q1("2/4");
    Q q2("1/2");
    Q q3("3/6");

    BOOST_TEST(q1 == q2);
    BOOST_TEST(q2 == q3);
}

// Test negative zero handling in Z
BOOST_AUTO_TEST_CASE(negative_zero_handling) {
    Z zero1(0);
    Z zero2(-0);
    Z pos_minus_pos = Z(5) - Z(5);

    BOOST_TEST(zero1 == zero2);
    BOOST_TEST(pos_minus_pos == zero1);
}

BOOST_AUTO_TEST_SUITE_END()
