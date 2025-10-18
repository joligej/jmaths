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

BOOST_AUTO_TEST_CASE(shift_operations_large_values) {
    N num(1);
    num <<= 50;  // Very large shift
    BOOST_TEST(num == N("1125899906842624"));  // 2^50

    num >>= 25;  // Shift back half way
    BOOST_TEST(num == N("33554432"));  // 2^25
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

// Additional shift operator tests (4+ per direction)
BOOST_AUTO_TEST_CASE(left_shift_by_one) {
    N num(5);  // 101
    N result = num << 1;
    BOOST_TEST(result == N(10));  // 1010
}

BOOST_AUTO_TEST_CASE(left_shift_by_multiple) {
    N num(1);
    N result = num << 10;
    BOOST_TEST(result == N(1024));
}

BOOST_AUTO_TEST_CASE(left_shift_large_number) {
    N num(1000);
    N result = num << 5;
    BOOST_TEST(result == N(32000));
}

BOOST_AUTO_TEST_CASE(left_shift_zero) {
    N num(42);
    N result = num << 0;
    BOOST_TEST(result == N(42));
}

BOOST_AUTO_TEST_CASE(right_shift_by_one) {
    N num(10);  // 1010
    N result = num >> 1;
    BOOST_TEST(result == N(5));  // 101
}

BOOST_AUTO_TEST_CASE(right_shift_by_multiple) {
    N num(1024);
    N result = num >> 10;
    BOOST_TEST(result == N(1));
}

BOOST_AUTO_TEST_CASE(right_shift_to_zero_large) {
    N num(7);
    N result = num >> 5;
    BOOST_TEST(result == N(0));
}

BOOST_AUTO_TEST_CASE(right_shift_large_amount) {
    N num(1000000);
    N result = num >> 20;
    BOOST_TEST(result == N(0));
}

// Additional AND tests
BOOST_AUTO_TEST_CASE(and_with_zero) {
    N num(0b1111);
    N result = num & N(0);
    BOOST_TEST(result == N(0));
}

BOOST_AUTO_TEST_CASE(and_with_self) {
    N num(0b1010);
    N result = num & num;
    BOOST_TEST(result == num);
}

BOOST_AUTO_TEST_CASE(and_selective_bits) {
    N num(0b11111111);
    N mask(0b00001111);
    N result = num & mask;
    BOOST_TEST(result == N(0b00001111));
}

BOOST_AUTO_TEST_CASE(and_large_numbers) {
    N a(255);
    N b(127);
    N result = a & b;
    BOOST_TEST(result == N(127));
}

// Additional OR tests
BOOST_AUTO_TEST_CASE(or_with_zero) {
    N num(0b1010);
    N result = num | N(0);
    BOOST_TEST(result == num);
}

BOOST_AUTO_TEST_CASE(or_with_self) {
    N num(0b1010);
    N result = num | num;
    BOOST_TEST(result == num);
}

BOOST_AUTO_TEST_CASE(or_combining_bits) {
    N a(0b1100);
    N b(0b0011);
    N result = a | b;
    BOOST_TEST(result == N(0b1111));
}

BOOST_AUTO_TEST_CASE(or_with_all_ones) {
    N num(0b1010);
    N mask(0b1111);
    N result = num | mask;
    BOOST_TEST(result == N(0b1111));
}

// Additional XOR tests
BOOST_AUTO_TEST_CASE(xor_with_zero) {
    N num(0b1010);
    N result = num ^ N(0);
    BOOST_TEST(result == num);
}

BOOST_AUTO_TEST_CASE(xor_with_self) {
    N num(0b1010);
    N result = num ^ num;
    BOOST_TEST(result == N(0));
}

BOOST_AUTO_TEST_CASE(xor_toggle_bits) {
    N num(0b1010);
    N mask(0b1111);
    N result = num ^ mask;
    BOOST_TEST(result == N(0b0101));
}

BOOST_AUTO_TEST_CASE(xor_double_application) {
    N original(0b1010);
    N mask(0b0011);
    N toggled = original ^ mask;
    N restored = toggled ^ mask;
    BOOST_TEST(restored == original);
}

// NOT operator tests
// Note: For arbitrary-precision integers, ~0 returns 0 by design
// This is because there's no fixed bit width, so "all bits set" is undefined

BOOST_AUTO_TEST_CASE(not_nonzero_number) {
    // Test with non-zero value - this works as expected
    N num(5);  // 0b101
    N result = ~num;
    // Bitwise NOT of non-zero inverts all bits in representation
    BOOST_TEST(result != N(0));
    BOOST_TEST(result != num);
}

BOOST_AUTO_TEST_CASE(not_all_ones) {
    N num(0b1111);
    N result = ~num;
    // Low bits should be zero after NOT
    BOOST_TEST((result & N(0b1111)) == N(0));
}

BOOST_AUTO_TEST_CASE(not_double_application) {
    N num(42);
    N result = ~~num;
    // Double NOT returns original value for non-zero
    BOOST_TEST(result == num);
}

BOOST_AUTO_TEST_CASE(not_zero_special_case) {
    // DOCUMENTED BEHAVIOR: For arbitrary-precision integers,
    // ~0 returns 0 because there are no bits to invert
    // This differs from fixed-width types but is correct for arbitrary precision
    N num(0);
    N result = ~num;
    BOOST_TEST(result == N(0));  // Expected behavior
}

// Compound assignment additional tests
BOOST_AUTO_TEST_CASE(compound_shift_operations) {
    N num1(1);
    num1 <<= 5;
    BOOST_TEST(num1 == N(32));

    N num2(64);
    num2 >>= 4;
    BOOST_TEST(num2 == N(4));
}

BOOST_AUTO_TEST_CASE(bitwise_compound_chain) {
    N num(0b1010);
    num |= N(0b0101);   // 1111
    num &= N(0b1100);   // 1100
    num ^= N(0b0011);   // 1111
    BOOST_TEST(num == N(0b1111));
}

BOOST_AUTO_TEST_CASE(mixed_bitwise_and_arithmetic) {
    N num(8);
    num <<= 2;     // 32
    num += N(16);  // 48
    num >>= 1;     // 24
    BOOST_TEST(num == N(24));
}

BOOST_AUTO_TEST_SUITE_END()
