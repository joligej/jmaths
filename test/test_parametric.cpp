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

#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>

#include "all.hpp"

namespace bdata = boost::unit_test::data;
using namespace jmaths;

BOOST_AUTO_TEST_SUITE(parametric_tests)

// Test data for addition
static const std::vector<std::tuple<int, int, int>> addition_data =
    {{0, 0, 0}, {1, 1, 2}, {10, 20, 30}, {100, 200, 300}, {999, 1, 1000}, {12345, 67890, 80235}};

BOOST_DATA_TEST_CASE(parametric_addition, bdata::make(addition_data), a, b, expected) {
    N na(a);
    N nb(b);
    N result = na + nb;
    BOOST_TEST(result == expected);
}

// Test data for multiplication
static const std::vector<std::tuple<int, int, int>> multiplication_data =
    {{0, 0, 0}, {0, 1, 0}, {1, 0, 0}, {1, 1, 1}, {2, 3, 6}, {10, 10, 100}, {123, 456, 56088}};

BOOST_DATA_TEST_CASE(parametric_multiplication, bdata::make(multiplication_data), a, b, expected) {
    N na(a);
    N nb(b);
    N result = na * nb;
    BOOST_TEST(result == expected);
}

// Test data for GCD
static const std::vector<std::tuple<int, int, int>> gcd_data = {{48, 18, 6},
                                                                {100, 50, 50},
                                                                {17, 13, 1},  // Coprime
                                                                {1024, 256, 256},
                                                                {97, 97, 97},  // Same number
                                                                {1000, 500, 500}};

BOOST_DATA_TEST_CASE(parametric_gcd, bdata::make(gcd_data), a, b, expected) {
    N na(a);
    N nb(b);
    N result = calc::gcd(na, nb);
    BOOST_TEST(result == expected);
}

// Test data for power
static const std::vector<std::tuple<int, int, int>> power_data =
    {{2, 0, 1}, {2, 1, 2}, {2, 2, 4}, {2, 3, 8}, {2, 4, 16}, {3, 3, 27}, {5, 2, 25}, {10, 3, 1000}};

BOOST_DATA_TEST_CASE(parametric_power, bdata::make(power_data), base, exp, expected) {
    N nbase(base);
    N nexp(exp);
    N result = calc::pow(nbase, nexp);
    BOOST_TEST(result == expected);
}

// Test signed operations
static const std::vector<std::tuple<int, int, int>> signed_addition_data = {{5, -3, 2},
                                                                            {-5, 3, -2},
                                                                            {-5, -3, -8},
                                                                            {10, -10, 0},
                                                                            {-10, 10, 0}};

BOOST_DATA_TEST_CASE(parametric_signed_addition, bdata::make(signed_addition_data), a, b, expected) {
    Z za(a);
    Z zb(b);
    Z result = za + zb;
    BOOST_TEST(result == expected);
}

// Test signed multiplication
static const std::vector<std::tuple<int, int, int>> signed_mult_data =
    {{2, 3, 6}, {-2, 3, -6}, {2, -3, -6}, {-2, -3, 6}, {0, 5, 0}, {-5, 0, 0}};

BOOST_DATA_TEST_CASE(parametric_signed_multiplication,
                     bdata::make(signed_mult_data),
                     a,
                     b,
                     expected) {
    Z za(a);
    Z zb(b);
    Z result = za * zb;
    BOOST_TEST(result == expected);
}

// Test comparison operations
static const std::vector<std::tuple<int, int, bool, bool, bool>> comparison_data = {
    // {a, b, a<b, a==b, a>b}
    {5, 10, true, false, false},
    {10, 5, false, false, true},
    {7, 7, false, true, false},
    {0, 1, true, false, false},
    {1, 0, false, false, true}};

BOOST_DATA_TEST_CASE(parametric_comparisons,
                     bdata::make(comparison_data),
                     a,
                     b,
                     less,
                     equal,
                     greater) {
    N na(a);
    N nb(b);

    BOOST_TEST((na < nb) == less);
    BOOST_TEST((na == nb) == equal);
    BOOST_TEST((na > nb) == greater);
    BOOST_TEST((na <= nb) == (less || equal));
    BOOST_TEST((na >= nb) == (greater || equal));
}

// Test modulo operations
static const std::vector<std::tuple<int, int, int>> modulo_data = {{10, 3, 1},
                                                                   {100, 7, 2},
                                                                   {50, 10, 0},
                                                                   {17, 5, 2},
                                                                   {1000, 17, 14}};

BOOST_DATA_TEST_CASE(parametric_modulo, bdata::make(modulo_data), a, b, expected) {
    N na(a);
    N nb(b);
    auto [quotient, remainder] = na / nb;
    BOOST_TEST(remainder == expected);
}

// Test bit shift operations
static const std::vector<std::tuple<int, int, int, int>> bitshift_data = {
    // {value, shift, left_result, right_result}
    {1, 1, 2, 0},
    {1, 2, 4, 0},
    {1, 3, 8, 0},
    {8, 1, 16, 4},
    {8, 2, 32, 2},
    {16, 1, 32, 8}};

BOOST_DATA_TEST_CASE(parametric_bitshift,
                     bdata::make(bitshift_data),
                     value,
                     shift,
                     left_expected,
                     right_expected) {
    N n(value);
    auto shift_amount = static_cast<jmaths::bitcount_t>(shift);

    N left_result = n << shift_amount;
    BOOST_TEST(left_result == left_expected);

    N right_result = n >> shift_amount;
    BOOST_TEST(right_result == right_expected);
}

// Test bitwise AND operations
static const std::vector<std::tuple<int, int, int>> bitwise_and_data = {{0xFF, 0x0F, 0x0F},
                                                                        {0xAA, 0x55, 0x00},
                                                                        {0xFF, 0xFF, 0xFF},
                                                                        {0x12, 0x34, 0x10}};

BOOST_DATA_TEST_CASE(parametric_bitwise_and, bdata::make(bitwise_and_data), a, b, expected) {
    N na(a);
    N nb(b);
    N result = na & nb;
    BOOST_TEST(result == expected);
}

// Test bitwise OR operations
static const std::vector<std::tuple<int, int, int>> bitwise_or_data = {{0x0F, 0xF0, 0xFF},
                                                                       {0xAA, 0x55, 0xFF},
                                                                       {0x12, 0x34, 0x36},
                                                                       {0x00, 0xFF, 0xFF}};

BOOST_DATA_TEST_CASE(parametric_bitwise_or, bdata::make(bitwise_or_data), a, b, expected) {
    N na(a);
    N nb(b);
    N result = na | nb;
    BOOST_TEST(result == expected);
}

// Test bitwise XOR operations
static const std::vector<std::tuple<int, int, int>> bitwise_xor_data = {{0xFF, 0xFF, 0x00},
                                                                        {0xAA, 0x55, 0xFF},
                                                                        {0x12, 0x34, 0x26},
                                                                        {0x00, 0xFF, 0xFF}};

BOOST_DATA_TEST_CASE(parametric_bitwise_xor, bdata::make(bitwise_xor_data), a, b, expected) {
    N na(a);
    N nb(b);
    N result = na ^ nb;
    BOOST_TEST(result == expected);
}

// Test string conversion round-trip
static const std::vector<std::string> string_conversion_data =
    {"0", "1", "42", "123", "1000", "99999", "123456789", "999999999999", "1234567890123456789"};

BOOST_DATA_TEST_CASE(parametric_string_conversion, bdata::make(string_conversion_data), str) {
    N n(str);
    std::string result = n.to_str();
    BOOST_TEST(result == str);
}

BOOST_AUTO_TEST_SUITE_END()
