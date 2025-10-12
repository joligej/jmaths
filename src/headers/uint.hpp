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

#include <concepts>
#include <cstddef>

#include "TMP.hpp"
#include "constants_and_types.hpp"

namespace jmaths {

template <bitcount_t V>
    requires TMP::is_power_of_2<V>
class uint {
    // using element_type = unsigned char;  // base_int_big ???

   public:
    using element_type = TMP::matches_which_t<TMP::type_size<V / bits_in_byte>,
                                              TMP::leq_size,
                                              TMP::unsigned_integral_types>;

   public:
    uint();

    uint(std::integral auto num);

   private:
    element_type digits_[V / (sizeof(element_type) * bits_in_byte)];
};

}  // namespace jmaths

#include "uint_impl.hpp"
