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
using namespace jmaths::literals;

BOOST_AUTO_TEST_SUITE(literals_tests)

BOOST_AUTO_TEST_CASE(n_literal_small) {
    auto num = 42_N;
    BOOST_TEST(num == 42);
}

BOOST_AUTO_TEST_CASE(n_literal_large) {
    auto num = 123456789012345678901234567890_N;
    BOOST_TEST(num.to_str() == "123456789012345678901234567890");
}

BOOST_AUTO_TEST_CASE(n_literal_zero) {
    auto num = 0_N;
    BOOST_TEST(num == 0);
}

BOOST_AUTO_TEST_CASE(z_literal_positive) {
    auto num = 42_Z;
    BOOST_TEST(num == 42);
    BOOST_TEST(num > 0);
}

BOOST_AUTO_TEST_CASE(z_literal_negative) {
    auto num = -42_Z;
    BOOST_TEST(num == -42);
    BOOST_TEST(num < 0);
}

BOOST_AUTO_TEST_CASE(z_literal_zero) {
    auto num = 0_Z;
    BOOST_TEST(num == 0);
}

BOOST_AUTO_TEST_CASE(q_literal_integer) {
    auto num = 42_Q;
    BOOST_TEST(num.to_str() == "42/1");
}

BOOST_AUTO_TEST_CASE(q_literal_negative) {
    auto num = -42_Q;
    BOOST_TEST(num.to_str() == "-42/1");
}

BOOST_AUTO_TEST_CASE(literal_arithmetic) {
    auto result = 10_N + 20_N;
    BOOST_TEST(result == 30);

    auto result2 = 50_Z - 20_Z;
    BOOST_TEST(result2 == 30);
}

BOOST_AUTO_TEST_SUITE_END()
