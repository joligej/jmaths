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

#include <chrono>
#include <random>
#include <type_traits>

#include "TMP.hpp"
#include "declarations.hpp"

namespace jmaths::internal {

template <typename T> class rand_gen {
    using allowed_types = TMP::pack_container</*short,
                                              int,
                                              long,
                                              long long,*/
                                              unsigned short,
                                              unsigned int,
                                              unsigned long,
                                              unsigned long long>;

    using seed_type = std::random_device;
    using generator_type = std::mt19937;
    using clock_type = std::chrono::high_resolution_clock;
    using distribution_type = std::uniform_int_distribution<
        std::conditional_t<TMP::matches_any_v<T, std::is_same, allowed_types>, T, unsigned int>>;

   public:
    rand_gen(T min, T max);

    rand_gen();

    T operator()();

   private:
    distribution_type distrib_;

    static generator_type gen_;
    static clock_type::time_point last_seed_time_;

    static void update_();
    static void reseed_(generator_type::result_type seed = seed_type{}());
};

}  // namespace jmaths::internal

namespace jmaths {

template <TMP::instance_of<basic_N, basic_Z> T> struct rand {
    [[nodiscard]] static T generate(bitcount_t upper_bound_exponent);
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct rand<basic_N<BaseInt, BaseIntBig, Allocator>> {
    using basic_N_type = basic_N<BaseInt, BaseIntBig, Allocator>;

    [[nodiscard]] static basic_N_type generate(bitcount_t upper_bound_exponent);
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct rand<basic_Z<BaseInt, BaseIntBig, Allocator>> {
    using basic_Z_type = basic_Z<BaseInt, BaseIntBig, Allocator>;

    [[nodiscard]] static basic_Z_type generate(bitcount_t upper_bound_exponent);
};

}  // namespace jmaths

#include "rand_impl.hpp"
