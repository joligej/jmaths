// The jmaths library for C++
// Copyright (C) 2024  Jasper de Smaele

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

#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "N.hpp"
#include "def.hh"

int main(int argc, char * argv[]) {
    const bool csv_format = argc > 1 && !std::strcmp(argv[1], "csv");

    using jmaths::N;
    using ull = unsigned long long;

    using random_type = std::uint8_t;
    constexpr auto random_type_exp = sizeof(random_type) * CHAR_BIT;
    constexpr auto max_value = (ull)1 << random_type_exp;

    ull values[max_value]{};

    REPEAT(25'000) {
        const auto random_value =
            *N::rand(random_type_exp).fits_into<random_type>();
        ++values[random_value];
    }

    if (csv_format) {
        for (std::size_t i = 0; i < max_value; ++i) {
            std::cout << i << ',' << values[i] << '\n';
        }
    } else {
        for (std::size_t i = 0; i < max_value; ++i) {
            std::cout << i << ": ";
            REPEAT(values[i]) {
                std::cout << '|';
            }
            std::cout << '\n';
        }
    }
}

#include "undef.hh"
