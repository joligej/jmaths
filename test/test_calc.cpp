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

/**
 * @file test_calc.cpp
 * @brief Tests for calc namespace mathematical functions
 *
 * This file tests the calc namespace which provides mathematical operations
 * including GCD, square root, exponentiation, and modular arithmetic.
 */

BOOST_AUTO_TEST_SUITE(calc_tests)

// ============================================================================
// GCD (Greatest Common Divisor) Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(gcd_tests)

BOOST_AUTO_TEST_CASE(gcd_basic) {
    N a(48);
    N b(18);
    N result = calc::gcd(a, b);
    BOOST_TEST(result == N(6));
}

BOOST_AUTO_TEST_CASE(gcd_coprime) {
    N a(17);
    N b(19);
    N result = calc::gcd(a, b);
    BOOST_TEST(result == N(1));
}

BOOST_AUTO_TEST_CASE(gcd_one_zero) {
    N a(42);
    N b(0);
    N result = calc::gcd(a, b);
    BOOST_TEST(result == N(42));
}

BOOST_AUTO_TEST_CASE(gcd_fibonacci_sequence) {
    // Test GCD with consecutive Fibonacci numbers (always 1)
    N fib1(89);
    N fib2(144);
    N result = calc::gcd(fib1, fib2);
    BOOST_TEST(result == N(1));
}

BOOST_AUTO_TEST_CASE(gcd_extended_cases) {
    BOOST_TEST(calc::gcd(N(12), N(18)) == 6);
    BOOST_TEST(calc::gcd(N(100), N(50)) == 50);
    BOOST_TEST(calc::gcd(N(7), N(13)) == 1);
    BOOST_TEST(calc::gcd(N(1), N(1)) == 1);
    BOOST_TEST(calc::gcd(N(100), N(100)) == 100);
    BOOST_TEST(calc::gcd(N(42), N(42)) == 42);
}

BOOST_AUTO_TEST_CASE(gcd_with_large_numbers) {
    N a("123456789");
    N b("987654321");
    N result = calc::gcd(a, b);
    BOOST_TEST(result == 9);
}

BOOST_AUTO_TEST_CASE(gcd_very_large_primes) {
    // Test with large primes - GCD should be 1
    N prime1("1000000007");
    N prime2("1000000009");
    N result = calc::gcd(prime1, prime2);
    BOOST_TEST(result == N(1));
}

BOOST_AUTO_TEST_CASE(gcd_power_of_two) {
    N a(1024);
    N b(2048);
    N result = calc::gcd(a, b);
    BOOST_TEST(result == N(1024));
}

BOOST_AUTO_TEST_CASE(gcd_consecutive_numbers) {
    N a(100);
    N b(101);
    N result = calc::gcd(a, b);
    BOOST_TEST(result == N(1));
}

BOOST_AUTO_TEST_CASE(gcd_multiple_of_small_primes) {
    N a(210);
    N b(330);
    N result = calc::gcd(a, b);
    BOOST_TEST(result == N(30));
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Square Root Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(sqrt_tests)

BOOST_AUTO_TEST_CASE(sqrt_perfect_square) {
    N num(144);
    auto [root, remainder] = calc::sqrt(num);
    BOOST_TEST(root == 12);
    BOOST_TEST(remainder == 0);
}

BOOST_AUTO_TEST_CASE(sqrt_non_perfect_square) {
    N num(150);
    auto [root, remainder] = calc::sqrt(num);
    BOOST_TEST(root == 12);
    BOOST_TEST(remainder == 6);  // 150 - 144 = 6
}

BOOST_AUTO_TEST_CASE(sqrt_zero) {
    N num(0);
    auto [root, remainder] = calc::sqrt(num);
    BOOST_TEST(root == 0);
    BOOST_TEST(remainder == 0);
}

BOOST_AUTO_TEST_CASE(sqrt_one) {
    N num(1);
    auto [root, remainder] = calc::sqrt(num);
    BOOST_TEST(root == 1);
    BOOST_TEST(remainder == 0);
}

BOOST_AUTO_TEST_CASE(sqrt_perfect_squares_sequence) {
    // Test multiple perfect squares in sequence
    auto [root1, rem1] = calc::sqrt(N(1));
    BOOST_TEST(root1 == 1);
    BOOST_TEST(rem1 == 0);

    auto [root4, rem4] = calc::sqrt(N(4));
    BOOST_TEST(root4 == 2);
    BOOST_TEST(rem4 == 0);

    auto [root9, rem9] = calc::sqrt(N(9));
    BOOST_TEST(root9 == 3);
    BOOST_TEST(rem9 == 0);

    auto [root16, rem16] = calc::sqrt(N(16));
    BOOST_TEST(root16 == 4);
    BOOST_TEST(rem16 == 0);

    auto [root25, rem25] = calc::sqrt(N(25));
    BOOST_TEST(root25 == 5);
    BOOST_TEST(rem25 == 0);

    auto [root100, rem100] = calc::sqrt(N(100));
    BOOST_TEST(root100 == 10);
    BOOST_TEST(rem100 == 0);

    auto [root10000, rem10000] = calc::sqrt(N(10000));
    BOOST_TEST(root10000 == 100);
    BOOST_TEST(rem10000 == 0);
}

BOOST_AUTO_TEST_CASE(sqrt_prime_numbers) {
    // Test sqrt of prime numbers (always have remainder)
    auto [root2, rem2] = calc::sqrt(N(2));
    BOOST_TEST(root2 == 1);
    BOOST_TEST(rem2 == 1);

    auto [root3, rem3] = calc::sqrt(N(3));
    BOOST_TEST(root3 == 1);
    BOOST_TEST(rem3 == 2);

    auto [root5, rem5] = calc::sqrt(N(5));
    BOOST_TEST(root5 == 2);
    BOOST_TEST(rem5 == 1);

    auto [root7, rem7] = calc::sqrt(N(7));
    BOOST_TEST(root7 == 2);
    BOOST_TEST(rem7 == 3);

    auto [root11, rem11] = calc::sqrt(N(11));
    BOOST_TEST(root11 == 3);
    BOOST_TEST(rem11 == 2);
}

BOOST_AUTO_TEST_CASE(sqrt_large_perfect_square) {
    N num("10000000000000000");
    auto [root, remainder] = calc::sqrt(num);
    BOOST_TEST(remainder == N(0));
}

BOOST_AUTO_TEST_CASE(sqrt_just_below_perfect_square) {
    N num(99);
    auto [root, remainder] = calc::sqrt(num);
    BOOST_TEST(root == N(9));
    BOOST_TEST(remainder == N(18));
}

BOOST_AUTO_TEST_CASE(sqrt_just_above_perfect_square) {
    N num(101);
    auto [root, remainder] = calc::sqrt(num);
    BOOST_TEST(root == N(10));
    BOOST_TEST(remainder == N(1));
}

BOOST_AUTO_TEST_CASE(sqrt_very_large_number) {
    N num("999999999999999999999999999999");
    auto [root, remainder] = calc::sqrt(num);
    BOOST_TEST(root > N(0));
    BOOST_TEST(remainder >= N(0));
    N root_squared = root * root;
    N root_plus_one_squared = (root + N(1)) * (root + N(1));
    BOOST_TEST(root_squared <= num);
    BOOST_TEST(num < root_plus_one_squared);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Square Root Whole Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(sqrt_whole_tests)

BOOST_AUTO_TEST_CASE(sqrt_whole_perfect_square) {
    N num(169);
    N root = calc::sqrt_whole(num);
    BOOST_TEST(root == 13);
}

BOOST_AUTO_TEST_CASE(sqrt_whole_comparison) {
    for (unsigned i = 0; i <= 100; ++i) {
        N num(i);
        auto [root_pair, rem] = calc::sqrt(num);
        N root_whole = calc::sqrt_whole(num);
        BOOST_TEST(root_pair == root_whole);
    }
}

BOOST_AUTO_TEST_CASE(sqrt_whole_large_number) {
    N num(1000000);
    N root = calc::sqrt_whole(num);
    BOOST_TEST(root == N(1000));
}

BOOST_AUTO_TEST_CASE(sqrt_whole_non_perfect) {
    N num(1000);
    N root = calc::sqrt_whole(num);
    BOOST_TEST(root == N(31));
}

BOOST_AUTO_TEST_CASE(sqrt_whole_small_primes) {
    BOOST_TEST(calc::sqrt_whole(N(2)) == N(1));
    BOOST_TEST(calc::sqrt_whole(N(3)) == N(1));
    BOOST_TEST(calc::sqrt_whole(N(5)) == N(2));
    BOOST_TEST(calc::sqrt_whole(N(7)) == N(2));
}

BOOST_AUTO_TEST_CASE(sqrt_whole_powers_of_ten) {
    BOOST_TEST(calc::sqrt_whole(N(10)) == N(3));
    BOOST_TEST(calc::sqrt_whole(N(100)) == N(10));
    BOOST_TEST(calc::sqrt_whole(N(1000)) == N(31));
    BOOST_TEST(calc::sqrt_whole(N(10000)) == N(100));
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Power (N) Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(pow_n_tests)

BOOST_AUTO_TEST_CASE(pow_basic) {
    N base(2);
    N exponent(10);
    N result = calc::pow(base, exponent);
    BOOST_TEST(result == 1024);
}

BOOST_AUTO_TEST_CASE(pow_zero_exponent) {
    N base(42);
    N exponent(0);
    N result = calc::pow(base, exponent);
    BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE(pow_one_exponent) {
    N base(42);
    N exponent(1);
    N result = calc::pow(base, exponent);
    BOOST_TEST(result == 42);
}

BOOST_AUTO_TEST_CASE(pow_base_one_exponent_range) {
    // Test that 1^n always equals 1 for various exponents
    N base(1);
    for (unsigned exp = 0; exp <= 100; ++exp) {
        N result = calc::pow(base, N(exp));
        BOOST_TEST(result == N(1));
    }
}

BOOST_AUTO_TEST_CASE(pow_powers_of_three) {
    // Test 3^n for various n
    BOOST_TEST(calc::pow(N(3), N(0)) == N(1));
    BOOST_TEST(calc::pow(N(3), N(1)) == N(3));
    BOOST_TEST(calc::pow(N(3), N(2)) == N(9));
    BOOST_TEST(calc::pow(N(3), N(3)) == N(27));
    BOOST_TEST(calc::pow(N(3), N(4)) == N(81));
    BOOST_TEST(calc::pow(N(3), N(20)) == N("3486784401"));
}

BOOST_AUTO_TEST_CASE(pow_edge_cases_comprehensive) {
    BOOST_TEST(calc::pow(N(0), N(0)) == 1);
    BOOST_TEST(calc::pow(N(0), N(5)) == 0);
    BOOST_TEST(calc::pow(N(1), N(1000)) == 1);
    BOOST_TEST(calc::pow(N(42), N(1)) == 42);
    BOOST_TEST(calc::pow(N(2), N(8)) == 256);
    BOOST_TEST(calc::pow(N(2), N(16)) == 65536);
}

BOOST_AUTO_TEST_CASE(pow_large_exponent) {
    N base(2);
    N exponent(100);
    N result = calc::pow(base, exponent);
    BOOST_TEST(result.to_str() == "1267650600228229401496703205376");
}

BOOST_AUTO_TEST_CASE(pow_base_ten) {
    N base(10);
    N exponent(20);
    N result = calc::pow(base, exponent);
    BOOST_TEST(result == N("100000000000000000000"));
}

BOOST_AUTO_TEST_CASE(pow_large_base_small_exp) {
    N base(999999);
    N exponent(3);
    N result = calc::pow(base, exponent);
    BOOST_TEST(result == N("999997000002999999"));
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Power (Z) Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(pow_z_tests)

BOOST_AUTO_TEST_CASE(pow_z_positive_base) {
    Z base(5);
    N exponent(3);
    Z result = calc::pow(base, exponent);
    BOOST_TEST(result == Z(125));
}

BOOST_AUTO_TEST_CASE(pow_z_negative_base_even_exp) {
    Z base(-3);
    N exponent(4);
    Z result = calc::pow(base, exponent);
    BOOST_TEST(result == Z(81));
}

BOOST_AUTO_TEST_CASE(pow_z_negative_base_odd_exp) {
    Z base(-2);
    N exponent(5);
    Z result = calc::pow(base, exponent);
    BOOST_TEST(result == Z(-32));
}

BOOST_AUTO_TEST_CASE(pow_z_zero_exponent) {
    Z base(-999);
    N exponent(0);
    Z result = calc::pow(base, exponent);
    BOOST_TEST(result == Z(1));
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Modular Exponentiation Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(pow_mod_tests)

BOOST_AUTO_TEST_CASE(pow_mod_basic) {
    N base(2);
    N exponent(10);
    N modulus(1000);
    N result = calc::pow_mod(base, exponent, modulus);
    BOOST_TEST(result == N(24));
}

BOOST_AUTO_TEST_CASE(pow_mod_fermat_little_theorem) {
    N base(3);
    N exponent(6);
    N mod(7);
    N result = calc::pow_mod(base, exponent, mod);
    BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE(pow_mod_large_exponent) {
    N base(3);
    N exponent(100);
    N modulus(97);
    N result = calc::pow_mod(base, exponent, modulus);
    BOOST_TEST(result < modulus);
}

BOOST_AUTO_TEST_CASE(pow_mod_base_greater_than_mod) {
    N base(15);
    N exponent(3);
    N modulus(7);
    N result = calc::pow_mod(base, exponent, modulus);
    BOOST_TEST(result == N(1));
}

BOOST_AUTO_TEST_CASE(pow_mod_zero_exponent) {
    N base(100);
    N exponent(0);
    N modulus(7);
    N result = calc::pow_mod(base, exponent, modulus);
    BOOST_TEST(result == N(1));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
