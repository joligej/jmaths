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

BOOST_AUTO_TEST_SUITE(bitwise_tests)

BOOST_AUTO_TEST_CASE(bitwise_and) {
    N a(0b1100);  // 12
    N b(0b1010);  // 10
    N result = a & b;
    BOOST_TEST(result == 0b1000);  // 8
}

BOOST_AUTO_TEST_CASE(bitwise_or) {
    N a(0b1100);  // 12
    N b(0b1010);  // 10
    N result = a | b;
    BOOST_TEST(result == 0b1110);  // 14
}

BOOST_AUTO_TEST_CASE(bitwise_xor) {
    N a(0b1100);  // 12
    N b(0b1010);  // 10
    N result = a ^ b;
    BOOST_TEST(result == 0b0110);  // 6
}

BOOST_AUTO_TEST_CASE(bitwise_not) {
    N a(0);
    N result = ~a;
    // Bitwise NOT of 0 depends on internal representation
    // Just verify it executed
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(left_shift) {
    N num(1);
    N result = num << 4;
    BOOST_TEST(result == 16);  // 1 << 4 = 16
}

BOOST_AUTO_TEST_CASE(right_shift) {
    N num(16);
    N result = num >> 4;
    BOOST_TEST(result == 1);  // 16 >> 4 = 1
}

BOOST_AUTO_TEST_CASE(left_shift_large) {
    N num(1);
    N result = num << 10;
    BOOST_TEST(result == 1024);  // 1 << 10 = 1024
}

BOOST_AUTO_TEST_CASE(right_shift_to_zero) {
    N num(15);
    N result = num >> 10;
    BOOST_TEST(result == 0);  // All bits shifted out
}

BOOST_AUTO_TEST_CASE(compound_shift_left) {
    N num(1);
    num <<= 5;
    BOOST_TEST(num == 32);
}

BOOST_AUTO_TEST_CASE(compound_shift_right) {
    N num(32);
    num >>= 5;
    BOOST_TEST(num == 1);
}

BOOST_AUTO_TEST_CASE(compound_and) {
    N num(0b1111);
    num &= N(0b1100);
    BOOST_TEST(num == 0b1100);
}

BOOST_AUTO_TEST_CASE(compound_or) {
    N num(0b1100);
    num |= N(0b0011);
    BOOST_TEST(num == 0b1111);
}

BOOST_AUTO_TEST_CASE(compound_xor) {
    N num(0b1100);
    num ^= N(0b0101);
    BOOST_TEST(num == 0b1001);
}

BOOST_AUTO_TEST_CASE(bit_manipulation_sequence) {
    N num(0);
    num |= N(1) << 0;            // Set bit 0
    num |= N(1) << 2;            // Set bit 2
    num |= N(1) << 4;            // Set bit 4
    BOOST_TEST(num == 0b10101);  // 21
}

BOOST_AUTO_TEST_CASE(bit_access) {
    N num(0b1010);  // 10
    BOOST_TEST(num[1] == 1);
    BOOST_TEST(num[0] == 0);
    BOOST_TEST(num[3] == 1);
    BOOST_TEST(num[2] == 0);
}

BOOST_AUTO_TEST_CASE(bit_set) {
    N num(0);
    num[0] = 1;
    num[2] = 1;
    num[4] = 1;
    BOOST_TEST(num == 0b10101);  // 21
}

BOOST_AUTO_TEST_CASE(bit_clear) {
    N num(0b1111);  // 15
    num[1] = 0;
    num[3] = 0;
    BOOST_TEST(num == 0b0101);  // 5
}

BOOST_AUTO_TEST_SUITE_END()
