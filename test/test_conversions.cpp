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

BOOST_AUTO_TEST_SUITE(conversions_tests)

BOOST_AUTO_TEST_CASE(n_to_string) {
    N num(12345);
    std::string str = num.to_str();
    BOOST_TEST(str == "12345");
}

BOOST_AUTO_TEST_CASE(n_to_string_matches_standard) {
    // Compare jmaths string conversion to std::to_string
    std::vector<unsigned long long> test_values =
        {0, 1, 42, 100, 255, 256, 1000, 9999, 65535, 65536, 1000000, 4294967295ULL};

    for (auto val : test_values) {
        N num(val);
        std::string jmaths_str = num.to_str();
        std::string std_str = std::to_string(val);
        BOOST_TEST(jmaths_str == std_str);
    }
}

BOOST_AUTO_TEST_CASE(n_to_string_large_numbers) {
    N huge("123456789012345678901234567890");
    std::string str = huge.to_str();
    BOOST_TEST(str == "123456789012345678901234567890");
}

BOOST_AUTO_TEST_CASE(n_to_string_different_bases) {
    N num(255);

    BOOST_TEST(num.to_str(2) == "11111111");  // Binary
    BOOST_TEST(num.to_str(8) == "377");       // Octal
    BOOST_TEST(num.to_str(10) == "255");      // Decimal
    BOOST_TEST(num.to_str(16) == "FF");       // Hexadecimal (uppercase)
}

BOOST_AUTO_TEST_CASE(n_from_string) {
    N num("67890");
    BOOST_TEST(num == 67890);
}

BOOST_AUTO_TEST_CASE(n_from_string_matches_standard) {
    // Compare jmaths string parsing to standard library
    std::vector<std::string> test_strings =
        {"0", "1", "10", "42", "100", "999", "1234", "9999", "10000", "65535", "100000", "1000000"};

    for (const auto & str : test_strings) {
        N num(str);
        unsigned long long std_val = std::stoull(str);

        auto opt = num.fits_into<unsigned long long>();
        BOOST_TEST(opt.has_value());
        BOOST_TEST(opt.value() == std_val);
    }
}

BOOST_AUTO_TEST_CASE(n_fits_into_uint32) {
    N num(42);
    auto opt = num.fits_into<std::uint32_t>();
    BOOST_TEST(opt.has_value());
    BOOST_TEST(opt.value() == 42u);
}

BOOST_AUTO_TEST_CASE(n_fits_into_uint64) {
    N num("1234567890");
    auto opt = num.fits_into<std::uint64_t>();
    BOOST_TEST(opt.has_value());
    BOOST_TEST(opt.value() == 1234567890ull);
}

BOOST_AUTO_TEST_CASE(n_fits_into_boundary_values) {
    // Test boundary values for safe integer types (32-bit and 64-bit only)

    // uint32_t max = 4294967295
    N n32(4294967295U);
    auto opt32 = n32.fits_into<std::uint32_t>();
    BOOST_TEST(opt32.has_value());
    BOOST_TEST(opt32.value() == 4294967295U);

    // uint64_t works fine
    N n64("12345678901234567890");
    auto opt64 = n64.fits_into<std::uint64_t>();
    BOOST_TEST(opt64.has_value());
}

BOOST_AUTO_TEST_CASE(n_doesnt_fit) {
    N huge("99999999999999999999999999999999");
    auto opt = huge.fits_into<std::uint32_t>();
    BOOST_TEST(!opt.has_value());
}

BOOST_AUTO_TEST_CASE(n_doesnt_fit_overflow) {
    // Value just over uint32_t max
    N overflow("4294967296");  // uint32_t max + 1
    auto opt = overflow.fits_into<std::uint32_t>();
    BOOST_TEST(!opt.has_value());
}

BOOST_AUTO_TEST_CASE(z_to_string_positive) {
    Z num(123);
    std::string str = num.to_str();
    BOOST_TEST(str == "123");
}

BOOST_AUTO_TEST_CASE(z_to_string_negative) {
    Z num(-123);
    std::string str = num.to_str();
    BOOST_TEST(str == "-123");
}

BOOST_AUTO_TEST_CASE(z_to_string_matches_standard_positive) {
    // Compare jmaths to std::to_string for positive values
    std::vector<long long> test_values = {0, 1, 42, 100, 999, 1234, 9999, 65535, 1000000};

    for (auto val : test_values) {
        Z num(val);
        std::string jmaths_str = num.to_str();
        std::string std_str = std::to_string(val);
        BOOST_TEST(jmaths_str == std_str);
    }
}

BOOST_AUTO_TEST_CASE(z_to_string_matches_standard_negative) {
    // Compare jmaths to std::to_string for negative values
    std::vector<long long> test_values = {-1, -42, -100, -999, -1234, -9999, -65535, -1000000};

    for (auto val : test_values) {
        Z num(val);
        std::string jmaths_str = num.to_str();
        std::string std_str = std::to_string(val);
        BOOST_TEST(jmaths_str == std_str);
    }
}

BOOST_AUTO_TEST_CASE(z_from_string_positive) {
    Z num("456");
    BOOST_TEST(num == 456);
}

BOOST_AUTO_TEST_CASE(z_from_string_negative) {
    Z num("-456");
    BOOST_TEST(num == -456);
}

BOOST_AUTO_TEST_CASE(z_from_string_matches_standard) {
    std::vector<std::string> test_strings =
        {"0", "1", "42", "-42", "100", "-100", "999", "-999", "123456", "-123456"};

    for (const auto & str : test_strings) {
        Z num(str);
        long long std_val = std::stoll(str);

        auto opt = num.fits_into<long long>();
        BOOST_TEST(opt.has_value());
        BOOST_TEST(opt.value() == std_val);
    }
}

BOOST_AUTO_TEST_CASE(z_fits_into_int32) {
    Z num(42);
    auto opt = num.fits_into<std::int32_t>();
    BOOST_TEST(opt.has_value());
    BOOST_TEST(opt.value() == 42);
}

BOOST_AUTO_TEST_CASE(z_fits_into_int32_negative) {
    Z num(-42);
    auto opt = num.fits_into<std::int32_t>();
    BOOST_TEST(opt.has_value());
    BOOST_TEST(opt.value() == -42);
}

BOOST_AUTO_TEST_CASE(z_fits_into_boundary_values) {
    // Test with safe integer types only (32-bit and 64-bit)

    // int32_t boundaries
    Z n32_min(std::int32_t(-2147483648));
    Z n32_max(std::int32_t(2147483647));
    BOOST_TEST(n32_min.fits_into<std::int32_t>().value() == -2147483648);
    BOOST_TEST(n32_max.fits_into<std::int32_t>().value() == 2147483647);
}

BOOST_AUTO_TEST_CASE(z_doesnt_fit_too_large) {
    Z huge("9999999999999999999999");
    BOOST_TEST(!huge.fits_into<std::int32_t>().has_value());
}

BOOST_AUTO_TEST_CASE(z_doesnt_fit_too_small) {
    Z huge_neg("-9999999999999999999999");
    BOOST_TEST(!huge_neg.fits_into<std::int32_t>().has_value());
}

BOOST_AUTO_TEST_CASE(q_to_string_proper_fraction) {
    Q num("3/4");
    std::string str = num.to_str();
    BOOST_TEST(str == "3/4");
}

BOOST_AUTO_TEST_CASE(q_to_string_integer) {
    Q num("6/2");  // Should normalize to 3/1
    std::string str = num.to_str();
    BOOST_TEST(str == "3/1");
}

BOOST_AUTO_TEST_CASE(n_z_conversion) {
    N n_val(42);
    Z z_val = Z(n_val);
    BOOST_TEST(z_val == 42);
}

BOOST_AUTO_TEST_CASE(z_n_conversion_positive) {
    Z z_val(42);
    N n_val = z_val.abs();
    BOOST_TEST(n_val == 42);
}

BOOST_AUTO_TEST_CASE(n_q_conversion) {
    N n_val(42);
    Q q_val = Q(n_val);
    BOOST_TEST(q_val.to_str() == "42/1");
}

// Additional fits_into tests for N (4+ per type)
BOOST_AUTO_TEST_CASE(n_fits_into_uint8_small) {
    N num(127);
    auto opt = num.fits_into<uint8_t>();
    BOOST_TEST(opt.has_value());
    BOOST_TEST(opt.value() == 127);
}

BOOST_AUTO_TEST_CASE(n_fits_into_uint8_max) {
    N num(255);
    auto opt = num.fits_into<uint8_t>();
    BOOST_TEST(opt.has_value());
    BOOST_TEST(opt.value() == 255);
}

BOOST_AUTO_TEST_CASE(n_doesnt_fit_uint8_too_large) {
    N num(256);
    auto opt = num.fits_into<uint8_t>();
    BOOST_TEST(!opt.has_value());
}

BOOST_AUTO_TEST_CASE(n_fits_into_uint8_zero) {
    N num(0);
    auto opt = num.fits_into<uint8_t>();
    BOOST_TEST(opt.has_value());
    BOOST_TEST(opt.value() == 0);
}

// uint16_t tests
BOOST_AUTO_TEST_CASE(n_fits_into_uint16_small) {
    N num(12345);
    auto opt = num.fits_into<uint16_t>();
    BOOST_TEST(opt.has_value());
    BOOST_TEST(opt.value() == 12345);
}

BOOST_AUTO_TEST_CASE(n_fits_into_uint16_max) {
    N num(65535);
    auto opt = num.fits_into<uint16_t>();
    BOOST_TEST(opt.has_value());
    BOOST_TEST(opt.value() == 65535);
}

BOOST_AUTO_TEST_CASE(n_doesnt_fit_uint16_too_large) {
    N num(65536);
    auto opt = num.fits_into<uint16_t>();
    BOOST_TEST(!opt.has_value());
}

BOOST_AUTO_TEST_CASE(n_fits_into_uint16_boundary) {
    N num(32768);
    auto opt = num.fits_into<uint16_t>();
    BOOST_TEST(opt.has_value());
}

// uint32_t tests
BOOST_AUTO_TEST_CASE(n_fits_into_uint32_medium) {
    N num(1000000);
    auto opt = num.fits_into<uint32_t>();
    BOOST_TEST(opt.has_value());
    BOOST_TEST(opt.value() == 1000000);
}

BOOST_AUTO_TEST_CASE(n_fits_into_uint32_max) {
    N num(4294967295u);
    auto opt = num.fits_into<uint32_t>();
    BOOST_TEST(opt.has_value());
}

BOOST_AUTO_TEST_CASE(n_doesnt_fit_uint32_too_large) {
    N num("4294967296");
    auto opt = num.fits_into<uint32_t>();
    BOOST_TEST(!opt.has_value());
}

BOOST_AUTO_TEST_CASE(n_fits_into_uint32_power_of_two) {
    N num(1073741824);  // 2^30
    auto opt = num.fits_into<uint32_t>();
    BOOST_TEST(opt.has_value());
}

// uint64_t tests
BOOST_AUTO_TEST_CASE(n_fits_into_uint64_large) {
    N num("123456789012345");
    auto opt = num.fits_into<uint64_t>();
    BOOST_TEST(opt.has_value());
}

BOOST_AUTO_TEST_CASE(n_fits_into_uint64_max) {
    N num("18446744073709551615");
    auto opt = num.fits_into<uint64_t>();
    BOOST_TEST(opt.has_value());
}

BOOST_AUTO_TEST_CASE(n_doesnt_fit_uint64_too_large) {
    N num("18446744073709551616");
    auto opt = num.fits_into<uint64_t>();
    BOOST_TEST(!opt.has_value());
}

BOOST_AUTO_TEST_CASE(n_fits_into_uint64_boundary) {
    N num("9223372036854775808");  // 2^63
    auto opt = num.fits_into<uint64_t>();
    BOOST_TEST(opt.has_value());
}

// Additional Z conversion tests
BOOST_AUTO_TEST_CASE(z_to_q_positive) {
    Z z_val(42);
    Q q_val(z_val);
    BOOST_TEST(q_val.to_str() == "42/1");
    BOOST_TEST(q_val == Q("42/1"));
}

BOOST_AUTO_TEST_CASE(z_to_q_negative) {
    Z z_val(-42);
    Q q_val(z_val);
    BOOST_TEST(q_val.to_str() == "-42/1");
    BOOST_TEST(q_val == Q("-42/1"));
}

BOOST_AUTO_TEST_CASE(z_to_q_zero) {
    Z z_val(0);
    Q q_val(z_val);
    BOOST_TEST(q_val.to_str() == "0/1");
    BOOST_TEST(q_val == Q("0/1"));
}

BOOST_AUTO_TEST_CASE(z_from_n_large) {
    N n_val("9999999999999999999");
    Z z_val(n_val);
    BOOST_TEST(z_val.to_str() == "9999999999999999999");
}

// String conversion round-trip tests
BOOST_AUTO_TEST_CASE(n_string_round_trip_small) {
    N original(123);
    std::string str = original.to_str();
    N reconstructed(str);
    BOOST_TEST(original == reconstructed);
}

BOOST_AUTO_TEST_CASE(n_string_round_trip_large) {
    N original("123456789012345678901234567890");
    std::string str = original.to_str();
    N reconstructed(str);
    BOOST_TEST(original == reconstructed);
}

BOOST_AUTO_TEST_CASE(z_string_round_trip_positive) {
    Z original(987654);
    std::string str = original.to_str();
    Z reconstructed(str);
    BOOST_TEST(original == reconstructed);
}

BOOST_AUTO_TEST_CASE(z_string_round_trip_negative) {
    Z original(-987654);
    std::string str = original.to_str();
    Z reconstructed(str);
    BOOST_TEST(original == reconstructed);
}

BOOST_AUTO_TEST_SUITE_END()
