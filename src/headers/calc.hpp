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

#include "TMP.hpp"
#include "declarations.hpp"

namespace jmaths {

struct calc {
    template <TMP::instance_of<basic_N> basic_N_type_1,
              TMP::decays_to<basic_N_type_1> basic_N_type_2>
    static constexpr auto gcd(basic_N_type_1 && a, basic_N_type_2 && b)
        -> std::decay_t<basic_N_type_1>;

    template <TMP::instance_of<basic_N> basic_N_type>
    static constexpr auto sqrt(basic_N_type && num)
        -> std::pair<std::decay_t<basic_N_type>, std::decay_t<basic_N_type>>;

    template <TMP::instance_of<basic_N> basic_N_type>
    static constexpr auto sqrt_whole(basic_N_type && num) -> std::decay_t<basic_N_type>;

    template <TMP::instance_of<basic_N> basic_N_type_1,
              TMP::decays_to<basic_N_type_1> basic_N_type_2>
    static constexpr auto pow(basic_N_type_1 && base, basic_N_type_2 && exponent)
        -> std::decay_t<basic_N_type_1>;

    template <TMP::instance_of<basic_N> basic_N_type_1,
              TMP::decays_to<basic_N_type_1> basic_N_type_2>
    static constexpr auto pow_mod(basic_N_type_1 && base,
                                  basic_N_type_2 && exponent,
                                  const std::decay_t<basic_N_type_1> & mod)
        -> std::decay_t<basic_N_type_1>;

    template <TMP::instance_of<basic_Z> basic_Z_type, TMP::instance_of<basic_N> basic_N_type>
    static constexpr auto pow(basic_Z_type && base, basic_N_type && exponent)
        -> std::decay_t<basic_Z_type>;
};  // namespace struct calc

}  // namespace jmaths

#include "calc_impl.hpp"
