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

#include <limits>
#include <random>
#include <type_traits>

#include "TMP.hpp"
#include "def.hh"

namespace jmaths::internal {

template <typename T> class rand_gen {
    using allowed_types = TMP::pack_container<short,
                                              int,
                                              long,
                                              long long,
                                              unsigned short,
                                              unsigned int,
                                              unsigned long,
                                              unsigned long long>;

   public:
    rand_gen(T min, T max) : distrib_(min, max) {
        JMATHS_FUNCTION_TO_LOG;
    }

    rand_gen() : rand_gen(0, std::numeric_limits<T>::max()) {
        JMATHS_FUNCTION_TO_LOG;
    }

    T operator()() {
        JMATHS_FUNCTION_TO_LOG;

        return distrib_(gen_);
    }

    template <typename SeedType> static void reseed(SeedType seed = seed_type{}()) {
        JMATHS_FUNCTION_TO_LOG;

        gen_.seed(seed);
    }

   private:
    using seed_type = std::random_device;

    inline static std::mt19937 gen_{seed_type{}()};

    std::uniform_int_distribution<
        std::conditional_t<TMP::matches_any_v<T, std::is_same, allowed_types>, T, unsigned int>>
        distrib_;
};

}  // namespace jmaths::internal

namespace jmaths {

template <TMP::any_of<N, Z> T> struct rand {
    [[nodiscard]] static T generate(bitcount_t upper_bound_exponent);
};

}  // namespace jmaths

#include "undef.hh"
