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

BOOST_AUTO_TEST_SUITE(operators_tests)

BOOST_AUTO_TEST_CASE(compound_assignment_add) {
    N num(10);
    num += N(5);
    BOOST_TEST(num == 15);
}

BOOST_AUTO_TEST_CASE(compound_assignment_subtract) {
    N num(20);
    num -= N(5);
    BOOST_TEST(num == 15);
}

BOOST_AUTO_TEST_CASE(compound_assignment_multiply) {
    N num(5);
    num *= N(3);
    BOOST_TEST(num == 15);
}

BOOST_AUTO_TEST_CASE(pre_increment) {
    N num(10);
    N& result = ++num;
    BOOST_TEST(num == 11);
    BOOST_TEST(result == 11);
}

BOOST_AUTO_TEST_CASE(pre_decrement) {
    N num(10);
    N& result = --num;
    BOOST_TEST(num == 9);
    BOOST_TEST(result == 9);
}

BOOST_AUTO_TEST_CASE(chained_operations) {
    N num(10);
    N result = ((num + N(5)) * N(2)) - N(10);
    // (10 + 5) * 2 - 10 = 15 * 2 - 10 = 30 - 10 = 20
    BOOST_TEST(result == 20);
}

BOOST_AUTO_TEST_CASE(comparison_chain) {
    N a(10);
    N b(20);
    N c(30);
    
    BOOST_TEST(a < b);
    BOOST_TEST(b < c);
    BOOST_TEST(a < c);
}

BOOST_AUTO_TEST_CASE(z_negation) {
    Z num(42);
    Z neg = -num;
    BOOST_TEST(neg == -42);
    Z pos = -neg;
    BOOST_TEST(pos == 42);
}

BOOST_AUTO_TEST_CASE(z_compound_with_negatives) {
    Z num(-10);
    num += Z(5);
    BOOST_TEST(num == -5);
    num -= Z(-3);
    BOOST_TEST(num == -2);
}

BOOST_AUTO_TEST_CASE(mixed_type_comparisons) {
    N n_val(42);
    BOOST_TEST(n_val == 42);
    BOOST_TEST(42 == n_val);
    BOOST_TEST(n_val < 50);
    BOOST_TEST(40 < n_val);
}

BOOST_AUTO_TEST_CASE(comparison_consistency) {
    N a(10);
    N b(20);
    N c(10);
    
    // Reflexive: a == a
    BOOST_TEST(a == a);
    
    // Symmetric: a == c implies c == a
    BOOST_TEST(a == c);
    BOOST_TEST(c == a);
    
    // Transitive properties
    BOOST_TEST(a < b);
    BOOST_TEST(b > a);
    BOOST_TEST(a <= b);
    BOOST_TEST(b >= a);
    BOOST_TEST(a <= c);
    BOOST_TEST(a >= c);
}

BOOST_AUTO_TEST_CASE(arithmetic_properties) {
    N a(5);
    N b(3);
    N c(2);
    
    // Commutativity: a + b = b + a
    BOOST_TEST(a + b == b + a);
    BOOST_TEST(a * b == b * a);
    
    // Associativity: (a + b) + c = a + (b + c)
    BOOST_TEST((a + b) + c == a + (b + c));
    BOOST_TEST((a * b) * c == a * (b * c));
    
    // Distributivity: a * (b + c) = a * b + a * c
    BOOST_TEST(a * (b + c) == a * b + a * c);
}

BOOST_AUTO_TEST_CASE(identity_elements) {
    N num(42);
    N zero(0);
    N one(1);
    
    // Additive identity
    BOOST_TEST(num + zero == num);
    BOOST_TEST(zero + num == num);
    
    // Multiplicative identity
    BOOST_TEST(num * one == num);
    BOOST_TEST(one * num == num);
}

BOOST_AUTO_TEST_CASE(compound_assignment_chain) {
    N num(10);
    num += N(5);  // 15
    BOOST_TEST(num == 15);
    num -= N(3);  // 12
    BOOST_TEST(num == 12);
    num *= N(2);  // 24
    BOOST_TEST(num == 24);
}

BOOST_AUTO_TEST_CASE(operator_precedence_test) {
    N a(2);
    N b(3);
    N c(4);
    
    // Test that operators follow expected precedence
    N result = a + b * c;  // Should be 2 + (3 * 4) = 14
    BOOST_TEST(result == 14);
}

BOOST_AUTO_TEST_SUITE_END()
