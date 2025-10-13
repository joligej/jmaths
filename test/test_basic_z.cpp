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
#include <sstream>
#include "all.hpp"

using namespace jmaths;

BOOST_AUTO_TEST_SUITE(basic_z_tests)

BOOST_AUTO_TEST_CASE(default_constructor) {
    Z num;
    BOOST_TEST(num == 0);
}

BOOST_AUTO_TEST_CASE(positive_constructor) {
    Z num(42);
    BOOST_TEST(num == 42);
    BOOST_TEST(num > 0);
}

BOOST_AUTO_TEST_CASE(negative_constructor) {
    Z num(-42);
    BOOST_TEST(num == -42);
    BOOST_TEST(num < 0);
}

BOOST_AUTO_TEST_CASE(string_constructor_positive) {
    Z num("123");
    BOOST_TEST(num == 123);
}

BOOST_AUTO_TEST_CASE(string_constructor_negative) {
    Z num("-123");
    BOOST_TEST(num == -123);
}

BOOST_AUTO_TEST_CASE(addition_positive) {
    Z a(10);
    Z b(20);
    Z c = a + b;
    BOOST_TEST(c == 30);
}

BOOST_AUTO_TEST_CASE(addition_negative) {
    Z a(-10);
    Z b(-20);
    Z c = a + b;
    BOOST_TEST(c == -30);
}

BOOST_AUTO_TEST_CASE(addition_mixed_signs) {
    Z a(50);
    Z b(-20);
    Z c = a + b;
    BOOST_TEST(c == 30);
    
    Z d = b + a;
    BOOST_TEST(d == 30);
}

BOOST_AUTO_TEST_CASE(subtraction_basic) {
    Z a(50);
    Z b(20);
    Z c = a - b;
    BOOST_TEST(c == 30);
}

BOOST_AUTO_TEST_CASE(subtraction_negative_result) {
    Z a(20);
    Z b(50);
    Z c = a - b;
    BOOST_TEST(c == -30);
}

BOOST_AUTO_TEST_CASE(subtraction_with_negatives) {
    Z a(-10);
    Z b(-20);
    Z c = a - b;
    BOOST_TEST(c == 10);
}

BOOST_AUTO_TEST_CASE(multiplication_positive) {
    Z a(6);
    Z b(7);
    Z c = a * b;
    BOOST_TEST(c == 42);
}

BOOST_AUTO_TEST_CASE(multiplication_negative) {
    Z a(-6);
    Z b(7);
    Z c = a * b;
    BOOST_TEST(c == -42);
    
    Z d = b * a;
    BOOST_TEST(d == -42);
}

BOOST_AUTO_TEST_CASE(multiplication_both_negative) {
    Z a(-6);
    Z b(-7);
    Z c = a * b;
    BOOST_TEST(c == 42);
}

BOOST_AUTO_TEST_CASE(division_positive) {
    Z a(100);
    Z b(10);
    auto [quotient, remainder] = a / b;
    BOOST_TEST(quotient == 10);
    BOOST_TEST(remainder == 0);
}

BOOST_AUTO_TEST_CASE(division_negative_dividend) {
    Z a(-100);
    Z b(10);
    auto [quotient, remainder] = a / b;
    BOOST_TEST(quotient == -10);
    BOOST_TEST(remainder == 0);
}

BOOST_AUTO_TEST_CASE(division_negative_divisor) {
    Z a(100);
    Z b(-10);
    auto [quotient, remainder] = a / b;
    BOOST_TEST(quotient == -10);
    BOOST_TEST(remainder == 0);
}

BOOST_AUTO_TEST_CASE(division_both_negative) {
    Z a(-100);
    Z b(-10);
    auto [quotient, remainder] = a / b;
    BOOST_TEST(quotient == 10);
    BOOST_TEST(remainder == 0);
}

BOOST_AUTO_TEST_CASE(negation_operator) {
    Z a(42);
    Z b = -a;
    BOOST_TEST(b == -42);
    
    Z c = -b;
    BOOST_TEST(c == 42);
}

BOOST_AUTO_TEST_CASE(abs_function) {
    Z a(42);
    Z b(-42);
    BOOST_TEST(a.abs() == 42);
    BOOST_TEST(b.abs() == 42);
}

BOOST_AUTO_TEST_CASE(sign_detection) {
    Z positive(42);
    Z negative(-42);
    Z zero(0);
    
    BOOST_TEST(positive > 0);
    BOOST_TEST(negative < 0);
    BOOST_TEST(zero == 0);
}

BOOST_AUTO_TEST_CASE(comparison_operators) {
    Z a(10);
    Z b(20);
    Z c(-10);
    
    BOOST_TEST(a < b);
    BOOST_TEST(c < a);
    BOOST_TEST(b > a);
    BOOST_TEST(a > c);
}

BOOST_AUTO_TEST_CASE(stream_operations) {
    Z num(-42);
    std::ostringstream oss;
    oss << num;
    BOOST_TEST(oss.str() == "-42");
    
    std::istringstream iss("-123");
    Z input;
    iss >> input;
    BOOST_TEST(input == -123);
}

BOOST_AUTO_TEST_SUITE_END()
