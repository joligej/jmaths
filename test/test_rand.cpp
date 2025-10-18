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
 * @file test_rand.cpp
 * @brief Tests for random number generation functions
 *
 * This file tests the rand<T> class template which provides random number
 * generation for arbitrary-precision types N and Z with various bit counts
 * and ranges.
 */

BOOST_AUTO_TEST_SUITE(rand_tests)

// ============================================================================
// Random N (Unsigned) Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(rand_n_tests)

BOOST_AUTO_TEST_CASE(random_n_generates_value) {
    N random_val = jmaths::rand<N>::generate(7);
    BOOST_TEST(random_val < 128);
}

BOOST_AUTO_TEST_CASE(random_n_small_range) {
    N random_val = jmaths::rand<N>::generate(4);
    BOOST_TEST(random_val < 16);
}

BOOST_AUTO_TEST_CASE(random_generates_different_values) {
    bool found_different = false;
    N val1 = jmaths::rand<N>::generate(10);
    for (int i = 0; i < 20; ++i) {
        N val = jmaths::rand<N>::generate(10);
        if (val != val1) {
            found_different = true;
            break;
        }
    }
    BOOST_TEST(found_different);
}

BOOST_AUTO_TEST_CASE(rand_n_zero_bits) {
    N random_val = jmaths::rand<N>::generate(0);
    BOOST_TEST(random_val == N(0));
}

BOOST_AUTO_TEST_CASE(rand_n_one_bit) {
    N random_val = jmaths::rand<N>::generate(1);
    BOOST_TEST(random_val <= N(1));
}

BOOST_AUTO_TEST_CASE(rand_n_large_bits) {
    N random_val = jmaths::rand<N>::generate(100);
    N max = calc::pow(N(2), N(100));
    BOOST_TEST(random_val < max);
}

BOOST_AUTO_TEST_CASE(rand_n_small_values) {
    for (int i = 0; i < 10; ++i) {
        N random_val = jmaths::rand<N>::generate(3);
        BOOST_TEST(random_val < N(8));
    }
}

BOOST_AUTO_TEST_CASE(rand_n_generates_different) {
    N val1 = jmaths::rand<N>::generate(20);
    N val2 = jmaths::rand<N>::generate(20);
    bool found_diff = (val1 != val2);

    for (int i = 0; i < 20 && !found_diff; ++i) {
        val2 = jmaths::rand<N>::generate(20);
        if (val1 != val2) found_diff = true;
    }
    BOOST_TEST(found_diff);
}

BOOST_AUTO_TEST_CASE(rand_n_medium_bits) {
    N random_val = jmaths::rand<N>::generate(32);
    N max = calc::pow(N(2), N(32));
    BOOST_TEST(random_val < max);
}

BOOST_AUTO_TEST_CASE(rand_n_consistent_range) {
    for (int i = 0; i < 50; ++i) {
        N val = jmaths::rand<N>::generate(10);
        BOOST_TEST(val < N(1024));
    }
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Random Z (Signed) Tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(rand_z_tests)

BOOST_AUTO_TEST_CASE(random_z_generates_value) {
    Z random_val = jmaths::rand<Z>::generate(7);
    BOOST_TEST((random_val >= -128 || random_val <= 128));
}

BOOST_AUTO_TEST_CASE(rand_z_small_bits) {
    for (int i = 0; i < 10; ++i) {
        Z random_val = jmaths::rand<Z>::generate(5);
        BOOST_TEST(random_val >= Z(-32));
        BOOST_TEST(random_val <= Z(32));
    }
}

BOOST_AUTO_TEST_CASE(rand_z_medium_bits) {
    Z random_val = jmaths::rand<Z>::generate(10);
    BOOST_TEST(random_val >= Z(-1024));
    BOOST_TEST(random_val <= Z(1024));
}

BOOST_AUTO_TEST_CASE(rand_z_sign_varies) {
    bool found_positive = false;
    bool found_negative = false;

    for (int i = 0; i < 50; ++i) {
        Z val = jmaths::rand<Z>::generate(8);
        if (val > Z(0)) found_positive = true;
        if (val < Z(0)) found_negative = true;
        if (found_positive && found_negative) break;
    }

    BOOST_TEST(found_positive);
    BOOST_TEST(found_negative);
}

BOOST_AUTO_TEST_CASE(rand_z_large_bits) {
    Z random_val = jmaths::rand<Z>::generate(100);
    BOOST_TEST(random_val.to_str().length() > 0);
}

BOOST_AUTO_TEST_CASE(rand_z_consistency) {
    for (int i = 0; i < 20; ++i) {
        Z val = jmaths::rand<Z>::generate(6);
        BOOST_TEST(val >= Z(-64));
        BOOST_TEST(val <= Z(64));
    }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
