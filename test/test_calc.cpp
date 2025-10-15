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

BOOST_AUTO_TEST_SUITE(calc_tests)

BOOST_AUTO_TEST_CASE(gcd_basic) {
    N a(48);
    N b(18);
    N result = calc::gcd(a, b);
    BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE(gcd_coprime) {
    N a(17);
    N b(19);
    N result = calc::gcd(a, b);
    BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE(gcd_one_zero) {
    N a(42);
    N b(0);
    N result = calc::gcd(a, b);
    BOOST_TEST(result == 42);
}

BOOST_AUTO_TEST_CASE(gcd_both_same) {
    N a(42);
    N b(42);
    N result = calc::gcd(a, b);
    BOOST_TEST(result == 42);
}

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

BOOST_AUTO_TEST_CASE(sqrt_whole_perfect_square) {
    N num(169);
    N root = calc::sqrt_whole(num);
    BOOST_TEST(root == 13);
}

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

BOOST_AUTO_TEST_CASE(pow_base_one) {
    N base(1);
    N exponent(100);
    N result = calc::pow(base, exponent);
    BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE(pow_large) {
    N base(3);
    N exponent(20);
    N result = calc::pow(base, exponent);
    // 3^20 = 3486784401
    BOOST_TEST(result.to_str() == "3486784401");
}

BOOST_AUTO_TEST_CASE(pow_edge_cases_comprehensive) {
    // 0^0 = 1 (by convention)
    BOOST_TEST(calc::pow(N(0), N(0)) == 1);

    // 0^n = 0 for n > 0
    BOOST_TEST(calc::pow(N(0), N(5)) == 0);

    // 1^n = 1
    BOOST_TEST(calc::pow(N(1), N(1000)) == 1);

    // n^1 = n
    BOOST_TEST(calc::pow(N(42), N(1)) == 42);

    // 2^n powers
    BOOST_TEST(calc::pow(N(2), N(8)) == 256);
    BOOST_TEST(calc::pow(N(2), N(16)) == 65536);
}

BOOST_AUTO_TEST_CASE(pow_mod_basic) {
    // Test modular exponentiation if it exists
    N base(2);
    N exponent(10);
    N mod(100);

    // 2^10 = 1024, 1024 % 100 = 24
    N result = calc::pow_mod(base, exponent, mod);
    BOOST_TEST(result == 24);
}

BOOST_AUTO_TEST_CASE(pow_mod_large_exponent) {
    // Test with large exponent (Fermat's little theorem test)
    N base(3);
    N exponent(6);  // φ(7) = 6, so 3^6 ≡ 1 (mod 7)
    N mod(7);

    N result = calc::pow_mod(base, exponent, mod);
    BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE(gcd_extended_cases) {
    // More comprehensive GCD tests
    BOOST_TEST(calc::gcd(N(12), N(18)) == 6);
    BOOST_TEST(calc::gcd(N(100), N(50)) == 50);
    BOOST_TEST(calc::gcd(N(7), N(13)) == 1);  // Coprime
    BOOST_TEST(calc::gcd(N(1), N(1)) == 1);
    BOOST_TEST(calc::gcd(N(100), N(100)) == 100);  // Same number
}

BOOST_AUTO_TEST_CASE(gcd_with_large_numbers) {
    N a("123456789");
    N b("987654321");
    N result = calc::gcd(a, b);
    // GCD(123456789, 987654321) = 9
    BOOST_TEST(result == 9);
}

BOOST_AUTO_TEST_CASE(sqrt_more_cases) {
    // Perfect squares
    auto [root1, rem1] = calc::sqrt(N(1));
    BOOST_TEST(root1 == 1);
    BOOST_TEST(rem1 == 0);

    auto [root4, rem4] = calc::sqrt(N(4));
    BOOST_TEST(root4 == 2);
    BOOST_TEST(rem4 == 0);

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

BOOST_AUTO_TEST_CASE(sqrt_non_perfect_squares) {
    auto [root, rem] = calc::sqrt(N(2));
    BOOST_TEST(root == 1);
    BOOST_TEST(rem == 1);  // 2 - 1^2 = 1

    auto [root3, rem3] = calc::sqrt(N(3));
    BOOST_TEST(root3 == 1);
    BOOST_TEST(rem3 == 2);  // 3 - 1^2 = 2

    auto [root8, rem8] = calc::sqrt(N(8));
    BOOST_TEST(root8 == 2);
    BOOST_TEST(rem8 == 4);  // 8 - 2^2 = 4

    auto [root50, rem50] = calc::sqrt(N(50));
    BOOST_TEST(root50 == 7);
    BOOST_TEST(rem50 == 1);  // 50 - 7^2 = 1
}

BOOST_AUTO_TEST_CASE(sqrt_whole_comparison) {
    // sqrt_whole should match the root from sqrt
    for (unsigned i = 0; i <= 100; ++i) {
        N num(i);
        auto [root_pair, rem] = calc::sqrt(num);
        N root_whole = calc::sqrt_whole(num);
        BOOST_TEST(root_pair == root_whole);
    }
}

BOOST_AUTO_TEST_SUITE_END()
