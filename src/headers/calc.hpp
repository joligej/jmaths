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

#pragma once

#include <utility>

#include "declarations.hpp"

namespace jmaths {

struct calc {
    static N gcd(N a, N b);
    static std::pair<N, N> sqrt(const N & num);
    static N sqrt_whole(const N & num);
    static N pow(N base, N exponent);
    static N pow_mod(N base, N exponent, const N & mod);
    static Z pow(Z base, N exponent);
};  // namespace struct calc

}  // namespace jmaths
