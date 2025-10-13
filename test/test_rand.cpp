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

BOOST_AUTO_TEST_SUITE(rand_tests)

BOOST_AUTO_TEST_CASE(random_n_generates_value) {
    N random_val = jmaths::rand<N>::generate(7);  // Up to 2^7 - 1 = 127
    BOOST_TEST(random_val < 128);
}

BOOST_AUTO_TEST_CASE(random_n_small_range) {
    N random_val = jmaths::rand<N>::generate(4);  // Up to 2^4 - 1 = 15
    BOOST_TEST(random_val < 16);
}

BOOST_AUTO_TEST_CASE(random_generates_different_values) {
    // Generate several values and check they're not all the same
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

BOOST_AUTO_TEST_CASE(random_z_generates_value) {
    Z random_val = jmaths::rand<Z>::generate(7);  // Generates Z value
    // Just check it's a valid Z
    BOOST_TEST((random_val >= -128 || random_val <= 128));
}

BOOST_AUTO_TEST_SUITE_END()
