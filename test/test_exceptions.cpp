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
#include <format>
#include <string>

#include "all.hpp"

using namespace jmaths;

/**
 * @file test_exceptions.cpp
 * @brief Comprehensive tests for all exception-throwing functions in jmaths library
 * 
 * This test suite verifies that all public library functions that can throw
 * exceptions actually do throw the expected exceptions under the appropriate
 * conditions.
 * 
 * EXCEPTIONS TESTED:
 * 1. error::division_by_zero - Thrown when dividing by zero
 * 2. error::invalid_base - Thrown when base is out of range [2, 64]
 * 3. std::format_error - Thrown when format string is invalid
 */

BOOST_AUTO_TEST_SUITE(exception_tests)

// =============================================================================
// error::division_by_zero tests
// =============================================================================

BOOST_AUTO_TEST_SUITE(division_by_zero_tests)

// Test basic_N division by zero
BOOST_AUTO_TEST_CASE(basic_N_division_operator) {
    N a(100);
    N zero(0);
    
    BOOST_CHECK_THROW(a / zero, error::division_by_zero);
}

// Test basic_N modulo by zero (division returns pair, second element is modulo)
BOOST_AUTO_TEST_CASE(basic_N_modulo_operator) {
    N a(100);
    N zero(0);
    
    // Division operator returns std::pair, division by zero throws
    BOOST_CHECK_THROW(a / zero, error::division_by_zero);
}

// Test basic_Z division by zero
BOOST_AUTO_TEST_CASE(basic_Z_division_operator) {
    Z a(100);
    Z zero(0);
    
    BOOST_CHECK_THROW(a / zero, error::division_by_zero);
}

BOOST_AUTO_TEST_CASE(basic_Z_division_negative) {
    Z a(-100);
    Z zero(0);
    
    BOOST_CHECK_THROW(a / zero, error::division_by_zero);
}

// Test basic_Q division by zero
BOOST_AUTO_TEST_CASE(basic_Q_division_operator) {
    Q a(N(100));
    Q zero(N(0));
    
    BOOST_CHECK_THROW(a / zero, error::division_by_zero);
}

BOOST_AUTO_TEST_CASE(basic_Q_division_operator_rational) {
    Q a(N(3), N(4));  // 3/4
    Q zero(N(0));
    
    BOOST_CHECK_THROW(a / zero, error::division_by_zero);
}

// Test basic_Q constructor with zero denominator
BOOST_AUTO_TEST_CASE(basic_Q_constructor_NN_zero_denom) {
    N numerator(5);
    N zero_denom(0);
    
    BOOST_CHECK_THROW(Q(numerator, zero_denom), error::division_by_zero);
}

BOOST_AUTO_TEST_CASE(basic_Q_constructor_NN_zero_denom_move) {
    N numerator(5);
    N zero_denom(0);
    
    BOOST_CHECK_THROW(Q(std::move(numerator), std::move(zero_denom)), error::division_by_zero);
}

BOOST_AUTO_TEST_CASE(basic_Q_constructor_ZZ_zero_denom) {
    Z numerator(5);
    Z zero_denom(0);
    
    BOOST_CHECK_THROW(Q(numerator, zero_denom), error::division_by_zero);
}

BOOST_AUTO_TEST_CASE(basic_Q_constructor_ZZ_zero_denom_negative) {
    Z numerator(-5);
    Z zero_denom(0);
    
    BOOST_CHECK_THROW(Q(numerator, zero_denom), error::division_by_zero);
}

BOOST_AUTO_TEST_CASE(basic_Q_constructor_ZZ_zero_denom_move) {
    Z numerator(5);
    Z zero_denom(0);
    
    BOOST_CHECK_THROW(Q(std::move(numerator), std::move(zero_denom)), error::division_by_zero);
}

// Test basic_Q string constructor with zero denominator
BOOST_AUTO_TEST_CASE(basic_Q_string_constructor_zero_denom) {
    BOOST_CHECK_THROW(Q("5/0"), error::division_by_zero);
}

BOOST_AUTO_TEST_CASE(basic_Q_string_constructor_zero_denom_base16) {
    BOOST_CHECK_THROW(Q("A/0", 16), error::division_by_zero);
}

// Test basic_Q inverse with zero
BOOST_AUTO_TEST_CASE(basic_Q_inverse_zero) {
    Q zero(N(0));
    Q result;
    
    BOOST_CHECK_THROW(result = zero.inverse(), error::division_by_zero);
}

BOOST_AUTO_TEST_CASE(basic_Q_inverse_zero_lvalue) {
    const Q zero(N(0));
    Q result;
    
    BOOST_CHECK_THROW(result = zero.inverse(), error::division_by_zero);
}

// Test basic_Q bitwise AND with zero denominator result
BOOST_AUTO_TEST_CASE(basic_Q_bitwise_and_zero_denom) {
    Q a(N(1), N(15));  // denom = 15
    Q b(N(1), N(240)); // denom = 240
    
    // 15 & 240 = 0, which would create zero denominator
    BOOST_CHECK_THROW(a & b, error::division_by_zero);
}

// Test basic_Q bitwise XOR with zero denominator result
BOOST_AUTO_TEST_CASE(basic_Q_bitwise_xor_zero_denom) {
    Q a(N(1), N(5));  // denom = 5
    Q b(N(1), N(5));  // denom = 5
    
    // 5 XOR 5 = 0, which would create zero denominator
    BOOST_CHECK_THROW(a ^ b, error::division_by_zero);
}

// Test calc::pow_mod with zero modulus
BOOST_AUTO_TEST_CASE(calc_pow_mod_zero_modulus) {
    N base(5);
    N exponent(3);
    N zero_mod(0);
    
    BOOST_CHECK_THROW(calc::pow_mod(base, exponent, zero_mod), error::division_by_zero);
}

BOOST_AUTO_TEST_CASE(calc_pow_mod_zero_modulus_large) {
    N base(12345);
    N exponent(67890);
    N zero_mod(0);
    
    BOOST_CHECK_THROW(calc::pow_mod(base, exponent, zero_mod), error::division_by_zero);
}

// Test error::division_by_zero::check with N
BOOST_AUTO_TEST_CASE(error_check_N) {
    N zero(0);
    
    BOOST_CHECK_THROW(error::division_by_zero::check(zero), error::division_by_zero);
}

// Test error::division_by_zero::check with Z
BOOST_AUTO_TEST_CASE(error_check_Z) {
    Z zero(0);
    
    BOOST_CHECK_THROW(error::division_by_zero::check(zero), error::division_by_zero);
}

// Test error::division_by_zero::check with Q
BOOST_AUTO_TEST_CASE(error_check_Q) {
    Q zero(N(0));
    
    BOOST_CHECK_THROW(error::division_by_zero::check(zero), error::division_by_zero);
}

// Test error::division_by_zero::check with integral types
BOOST_AUTO_TEST_CASE(error_check_int) {
    int zero = 0;
    
    BOOST_CHECK_THROW(error::division_by_zero::check(zero), error::division_by_zero);
}

BOOST_AUTO_TEST_CASE(error_check_unsigned) {
    unsigned zero = 0U;
    
    BOOST_CHECK_THROW(error::division_by_zero::check(zero), error::division_by_zero);
}

// Test error::division_by_zero::check with custom message
BOOST_AUTO_TEST_CASE(error_check_custom_message) {
    N zero(0);
    
    try {
        error::division_by_zero::check(zero, "Custom error message");
        BOOST_FAIL("Expected exception not thrown");
    } catch (const error::division_by_zero& e) {
        std::string msg = e.what();
        BOOST_CHECK(msg.find("Custom error message") != std::string::npos);
    }
}

// Test that non-zero values do NOT throw
BOOST_AUTO_TEST_CASE(error_check_non_zero_N) {
    N one(1);
    
    BOOST_CHECK_NO_THROW(error::division_by_zero::check(one));
}

BOOST_AUTO_TEST_CASE(error_check_non_zero_Z) {
    Z minus_one(-1);
    
    BOOST_CHECK_NO_THROW(error::division_by_zero::check(minus_one));
}

BOOST_AUTO_TEST_SUITE_END()  // division_by_zero_tests

// =============================================================================
// error::invalid_base tests
// =============================================================================

BOOST_AUTO_TEST_SUITE(invalid_base_tests)

// Test basic_N constructor with invalid base
BOOST_AUTO_TEST_CASE(basic_N_constructor_base_too_small) {
    BOOST_CHECK_THROW(N("101", 1), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_N_constructor_base_zero) {
    BOOST_CHECK_THROW(N("0", 0), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_N_constructor_base_too_large) {
    BOOST_CHECK_THROW(N("123", 65), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_N_constructor_base_way_too_large) {
    BOOST_CHECK_THROW(N("123", 1000), error::invalid_base);
}

// Test basic_N::to_str with invalid base
BOOST_AUTO_TEST_CASE(basic_N_to_str_base_too_small) {
    N num(42);
    std::string result;
    
    BOOST_CHECK_THROW(result = num.to_str(1), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_N_to_str_base_zero) {
    N num(42);
    std::string result;
    
    BOOST_CHECK_THROW(result = num.to_str(0), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_N_to_str_base_too_large) {
    N num(42);
    std::string result;
    
    BOOST_CHECK_THROW(result = num.to_str(65), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_N_to_str_base_way_too_large) {
    N num(42);
    std::string result;
    
    BOOST_CHECK_THROW(result = num.to_str(255), error::invalid_base);
}

// Test basic_Z with invalid base
BOOST_AUTO_TEST_CASE(basic_Z_constructor_base_too_small) {
    BOOST_CHECK_THROW(Z("-101", 1), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_Z_constructor_base_too_large) {
    BOOST_CHECK_THROW(Z("123", 65), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_Z_to_str_base_too_small) {
    Z num(-42);
    std::string result;
    
    BOOST_CHECK_THROW(result = num.to_str(1), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_Z_to_str_base_too_large) {
    Z num(-42);
    std::string result;
    
    BOOST_CHECK_THROW(result = num.to_str(65), error::invalid_base);
}

// Test basic_Q with invalid base
BOOST_AUTO_TEST_CASE(basic_Q_constructor_base_too_small) {
    BOOST_CHECK_THROW(Q("3/4", 1), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_Q_constructor_base_too_large) {
    BOOST_CHECK_THROW(Q("3/4", 65), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_Q_to_str_base_too_small) {
    Q num(N(3), N(4));
    std::string result;
    
    BOOST_CHECK_THROW(result = num.to_str(1), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(basic_Q_to_str_base_too_large) {
    Q num(N(3), N(4));
    std::string result;
    
    BOOST_CHECK_THROW(result = num.to_str(65), error::invalid_base);
}

// Test error::invalid_base::check directly
BOOST_AUTO_TEST_CASE(error_invalid_base_check_zero) {
    BOOST_CHECK_THROW(error::invalid_base::check(0), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(error_invalid_base_check_one) {
    BOOST_CHECK_THROW(error::invalid_base::check(1), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(error_invalid_base_check_65) {
    BOOST_CHECK_THROW(error::invalid_base::check(65), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(error_invalid_base_check_100) {
    BOOST_CHECK_THROW(error::invalid_base::check(100), error::invalid_base);
}

// Test that valid bases do NOT throw
BOOST_AUTO_TEST_CASE(error_invalid_base_check_valid_2) {
    BOOST_CHECK_NO_THROW(error::invalid_base::check(2));
}

BOOST_AUTO_TEST_CASE(error_invalid_base_check_valid_10) {
    BOOST_CHECK_NO_THROW(error::invalid_base::check(10));
}

BOOST_AUTO_TEST_CASE(error_invalid_base_check_valid_16) {
    BOOST_CHECK_NO_THROW(error::invalid_base::check(16));
}

BOOST_AUTO_TEST_CASE(error_invalid_base_check_valid_64) {
    BOOST_CHECK_NO_THROW(error::invalid_base::check(64));
}

// Test boundary cases
BOOST_AUTO_TEST_CASE(basic_N_constructor_base_2_valid) {
    BOOST_CHECK_NO_THROW(N("101", 2));
}

BOOST_AUTO_TEST_CASE(basic_N_constructor_base_64_valid) {
    BOOST_CHECK_NO_THROW(N("ZzZ", 64));
}

BOOST_AUTO_TEST_CASE(basic_N_to_str_base_2_valid) {
    N num(42);
    std::string result;
    BOOST_CHECK_NO_THROW(result = num.to_str(2));
}

BOOST_AUTO_TEST_CASE(basic_N_to_str_base_64_valid) {
    N num(42);
    std::string result;
    BOOST_CHECK_NO_THROW(result = num.to_str(64));
}

BOOST_AUTO_TEST_SUITE_END()  // invalid_base_tests

// =============================================================================
// std::format_error tests (from formatter)
// =============================================================================

BOOST_AUTO_TEST_SUITE(format_error_tests)

// NOTE: std::format validates format strings at compile time in C++20 and later.
// This means that invalid format strings cause compilation errors rather than
// runtime exceptions. Therefore, we cannot test for std::format_error being thrown
// at runtime with invalid format specifiers.
//
// The following test cases are commented out because they would not compile:
/*
// Test invalid format string with non-digit characters
BOOST_AUTO_TEST_CASE(formatter_invalid_non_digit) {
    N num(42);
    
    BOOST_CHECK_THROW(std::format("{:abc}", num), std::format_error);
}

BOOST_AUTO_TEST_CASE(formatter_invalid_letter) {
    Z num(-42);
    
    BOOST_CHECK_THROW(std::format("{:x}", num), std::format_error);
}

BOOST_AUTO_TEST_CASE(formatter_invalid_special_char) {
    Q num(3, 4);
    
    BOOST_CHECK_THROW(std::format("{:@}", num), std::format_error);
}
*/

// Test format with invalid base (should throw invalid_base, not format_error)
// NOTE: These are commented out because std::format validates format strings at
// compile time, so invalid bases cause compilation errors rather than runtime exceptions
/*
BOOST_AUTO_TEST_CASE(formatter_base_too_small) {
    N num(42);
    
    // Base 1 is invalid, but parser will create it from "1"
    BOOST_CHECK_THROW(std::format("{:1}", num), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(formatter_base_too_large) {
    N num(42);
    
    // Base 65 is invalid
    BOOST_CHECK_THROW(std::format("{:65}", num), error::invalid_base);
}

BOOST_AUTO_TEST_CASE(formatter_base_way_too_large) {
    N num(42);
    
    // Base 999 is invalid
    BOOST_CHECK_THROW(std::format("{:999}", num), error::invalid_base);
}
*/

// Test valid format strings do NOT throw
BOOST_AUTO_TEST_CASE(formatter_valid_default) {
    N num(42);
    std::string result;
    
    BOOST_CHECK_NO_THROW(result = std::format("{}", num));
}

BOOST_AUTO_TEST_CASE(formatter_valid_base_10) {
    N num(42);
    std::string result;
    
    BOOST_CHECK_NO_THROW(result = std::format("{:10}", num));
}

BOOST_AUTO_TEST_CASE(formatter_valid_base_16) {
    N num(255);
    std::string result;
    
    BOOST_CHECK_NO_THROW(result = std::format("{:16}", num));
}

BOOST_AUTO_TEST_CASE(formatter_valid_base_2) {
    Z num(-8);
    std::string result;
    
    BOOST_CHECK_NO_THROW(result = std::format("{:2}", num));
}

BOOST_AUTO_TEST_CASE(formatter_valid_base_64) {
    Q num(N(1234), N(5678));
    std::string result;
    
    BOOST_CHECK_NO_THROW(result = std::format("{:64}", num));
}

BOOST_AUTO_TEST_SUITE_END()  // format_error_tests

// =============================================================================
// Exception hierarchy and properties tests
// =============================================================================

BOOST_AUTO_TEST_SUITE(exception_hierarchy_tests)

// Test that jmaths exceptions inherit from std::exception
BOOST_AUTO_TEST_CASE(error_inherits_from_std_exception) {
    try {
        throw error("test");
    } catch (const std::exception& e) {
        // Should catch as std::exception
        BOOST_CHECK(true);
        BOOST_CHECK(std::string(e.what()).find("test") != std::string::npos);
    } catch (...) {
        BOOST_FAIL("Should catch as std::exception");
    }
}

BOOST_AUTO_TEST_CASE(division_by_zero_inherits_from_error) {
    try {
        throw error::division_by_zero();
    } catch (const error& e) {
        // Should catch as error
        BOOST_CHECK(true);
    } catch (...) {
        BOOST_FAIL("Should catch as error");
    }
}

BOOST_AUTO_TEST_CASE(invalid_base_inherits_from_error) {
    try {
        throw error::invalid_base();
    } catch (const error& e) {
        // Should catch as error
        BOOST_CHECK(true);
    } catch (...) {
        BOOST_FAIL("Should catch as error");
    }
}

// Test exception messages
BOOST_AUTO_TEST_CASE(division_by_zero_default_message) {
    try {
        throw error::division_by_zero();
    } catch (const error::division_by_zero& e) {
        std::string msg = e.what();
        BOOST_CHECK(!msg.empty());
        BOOST_CHECK(msg.find("divide by zero") != std::string::npos ||
                   msg.find("divide") != std::string::npos);
    }
}

BOOST_AUTO_TEST_CASE(division_by_zero_custom_message) {
    try {
        throw error::division_by_zero("Custom division error");
    } catch (const error::division_by_zero& e) {
        std::string msg = e.what();
        BOOST_CHECK(msg.find("Custom division error") != std::string::npos);
    }
}

BOOST_AUTO_TEST_CASE(invalid_base_default_message) {
    try {
        throw error::invalid_base();
    } catch (const error::invalid_base& e) {
        std::string msg = e.what();
        BOOST_CHECK(!msg.empty());
        BOOST_CHECK(msg.find("base") != std::string::npos ||
                   msg.find("2") != std::string::npos ||
                   msg.find("64") != std::string::npos);
    }
}

BOOST_AUTO_TEST_CASE(invalid_base_custom_message) {
    try {
        throw error::invalid_base("Custom base error");
    } catch (const error::invalid_base& e) {
        std::string msg = e.what();
        BOOST_CHECK(msg.find("Custom base error") != std::string::npos);
    }
}

// Test error constants
BOOST_AUTO_TEST_CASE(invalid_base_constants) {
    BOOST_CHECK_EQUAL(error::invalid_base::minimum_base, 2U);
    BOOST_CHECK_EQUAL(error::invalid_base::maximum_base, 64U);
}

BOOST_AUTO_TEST_SUITE_END()  // exception_hierarchy_tests

BOOST_AUTO_TEST_SUITE_END()  // exception_tests
