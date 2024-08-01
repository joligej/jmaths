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

#include "rand.hpp"

#include <cstddef>
#include <random>
#include <type_traits>

#include "N.hpp"
#include "Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"

namespace jmaths {

template N N::rand<true>(bitcount_t upper_bound_exponent);
template Z Z::rand<true>(bitcount_t upper_bound_exponent);

template <bool> N N::rand(bitcount_t upper_bound_exponent) {
    FUNCTION_TO_LOG;

    using rand_type = std::conditional_t<rand_enabled, base_int, int>;
    static internal::rand<rand_type> rand_gen;

    const std::size_t pos_whole = upper_bound_exponent / base_int_bits;

    N random_number;
    random_number.digits_.reserve(pos_whole + 1U);

    const bitpos_t pos_mod = upper_bound_exponent % base_int_bits;

    for (std::size_t i = 0U; i < pos_whole; ++i) {
        random_number.digits_.emplace_back(rand_gen());
    }

    if (pos_mod > 0U) {
        random_number.digits_.emplace_back(rand_gen() >> (base_int_bits - pos_mod));
    }

    random_number.remove_leading_zeroes_();

    return random_number;
}

template <bool> Z Z::rand(bitcount_t upper_bound_exponent) {
    FUNCTION_TO_LOG;

    static internal::rand<unsigned int> rand_gen(0U, 1U);

    N random_number = N::rand<true>(upper_bound_exponent);

    if (random_number.is_zero()) { return Z{}; }

    return {std::move(random_number), static_cast<sign_bool>(rand_gen())};
}

}  // namespace jmaths

#include "undef.hh"
