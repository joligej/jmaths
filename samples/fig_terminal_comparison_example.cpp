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

#include <iostream>
#include <string>

#include "basic_Z.hpp"
#include "calc.hpp"
#include "literals.hpp"

using namespace jmaths::literals;

int main(int, char *[]) {
    std::string input_str;
    std::cout << "Enter your number:\n";
    std::cin >> input_str;

    using jmaths::Z, jmaths::calc;

    Z z(input_str);
    std::cout << "z == " << z << '\n';
    ++z;
    std::cout << "z + 1 == " << z << '\n';
    z = calc::pow(z, 2_N);  // or z *= z;
    std::cout << "(z + 1)^2 == " << z << '\n';
}
