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

#include <functional>
#include <iostream>
#include <random>

#include "basic_Z.hpp"
#include "rand.hpp"

int main(int, char *[]) {
    using jmaths::Z, jmaths::rand, jmaths::bitcount_t;

    auto exponent_generator = std::bind(std::uniform_int_distribution<bitcount_t>(0, 32),
                                        std::mt19937(std::random_device{}()));

    while (std::cin.get() == '\n') {
        std::cout << rand<Z>::generate(exponent_generator()) << '\n';
    }

    return 0;
}
