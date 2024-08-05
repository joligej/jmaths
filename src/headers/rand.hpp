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

#include "TMP.hpp"
#include "def.hh"

namespace jmaths::internal {

template <typename T>
concept allowed_rand_type = TMP::any_of<T,
                                        short,
                                        int,
                                        long,
                                        long long,
                                        unsigned short,
                                        unsigned int,
                                        unsigned long,
                                        unsigned long long>;

template <allowed_rand_type T> class rand {
   public:
    rand(T min, T max) : distrib(min, max) {
        FUNCTION_TO_LOG;
    }

    rand() : rand(0, std::numeric_limits<T>::max()) {
        FUNCTION_TO_LOG;
    }

    T operator()() {
        FUNCTION_TO_LOG;

        return distrib(gen);
    }

    template <typename SEED> static void reseed(SEED seed = seed_type{}()) {
        FUNCTION_TO_LOG;

        gen.seed(seed);
    }

   private:
    using seed_type = std::random_device;

    inline static std::mt19937 gen{seed_type{}()};

    std::uniform_int_distribution<T> distrib;
};

}  // namespace jmaths::internal

namespace jmaths {

template <TMP::any_of<N, Z> T> class rand {
   public:
    [[nodiscard]] static T generate(bitcount_t upper_bound_exponent);

   private:
    inline static internal::rand<base_int> random_base_int{};
    inline static internal::rand<unsigned int> random_bool{0U, 1U};
};

}  // namespace jmaths

#include "undef.hh"
