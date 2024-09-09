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

#include "TMP.hpp"
#include "constants_and_types.hpp"

// all the types
namespace jmaths {

struct calc;

class N;
class sign_type;
class Z;
class Q;
class error;

template <TMP::any_of<N, Z> T> struct rand;

template <bitcount_t V> requires TMP::is_power_of_2<V> class uint;

}  // namespace jmaths
