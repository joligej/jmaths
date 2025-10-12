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

#include <iostream>

#include "basic_Q.hpp"

int main(int, char *[]) {
    using jmaths::Q;
    using flt_type = double;
    static constexpr flt_type not_present = -1;

    const auto print = [](const Q & q) {
        std::cout << q << '\n';
        const auto into_float = q.template fits_into<flt_type>().value_or(not_present);
        std::cout << into_float << '\n';
    };

#define SHOW(x)                                 \
    std::cout << "Results for " << #x << ":\n"; \
    print(Q(x));

    SHOW("3/1");
    SHOW("-3/1");
    SHOW(3.765787);

    SHOW("-1/12");
    SHOW(0.18);
    SHOW(-234.24);

    SHOW("12/17");
}
