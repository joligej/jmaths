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

    template <typename U, typename R> struct result_type_requirements {
        static constexpr bool value =
            std::unsigned_integral<R> &&
            std::numeric_limits<R>::max() >= std::numeric_limits<U>::max() &&
            std::numeric_limits<R>::min() <= std::numeric_limits<U>::min();
    };

   public:
    using result_type =
        std::conditional_t<TMP::matches_any_v<T, std::is_same, allowed_types>,
                           T,
                           TMP::matches_which_t<T, result_type_requirements, allowed_types>>;

    rand_gen(T min, T max) : distrib(min, max) {
        FUNCTION_TO_LOG;
    }

    rand_gen() : rand_gen(0, std::numeric_limits<T>::max()) {
        FUNCTION_TO_LOG;
    }

    result_type operator()() {
        FUNCTION_TO_LOG;

        return distrib(gen);
    }

    template <typename SeedType> static void reseed(SeedType seed = seed_type{}()) {
        FUNCTION_TO_LOG;

        gen.seed(seed);
    }

   private:
    using seed_type = std::random_device;

    inline static std::mt19937 gen{seed_type{}()};

    std::uniform_int_distribution<result_type> distrib;
};

}  // namespace jmaths::internal

namespace jmaths {

template <TMP::any_of<N, Z> T> class rand {
   public:
    [[nodiscard]] static T generate(bitcount_t upper_bound_exponent);

   private:
    inline static internal::rand_gen<base_int> random_base_int{};
    inline static internal::rand_gen<unsigned int> random_bool{0U, 1U};
};

}  // namespace jmaths

#include "undef.hh"
