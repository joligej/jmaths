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

#pragma once

#include <chrono>
#include <cstddef>
#include <limits>
#include <random>
#include <utility>

#include "basic_N.hpp"
#include "basic_Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "rand.hpp"

// member functions of internal::rand_gen
namespace jmaths::internal {

template <typename T> typename rand_gen<T>::generator_type rand_gen<T>::gen_(seed_type{}());

template <typename T>
typename rand_gen<T>::clock_type::time_point rand_gen<T>::last_seed_time_(clock_type::now());

template <typename T> rand_gen<T>::rand_gen(T min, T max) : distrib_(min, max) {
    JMATHS_FUNCTION_TO_LOG;
}

template <typename T> rand_gen<T>::rand_gen() : rand_gen(0U, std::numeric_limits<T>::max()) {
    JMATHS_FUNCTION_TO_LOG;
}

template <typename T> T rand_gen<T>::operator()() {
    JMATHS_FUNCTION_TO_LOG;

#ifndef JMATHS_PERIODICALLY_RESEED_RAND
    #error "JMATHS_PERIODICALLY_RESEED_RAND is not defined, please define it when compiling!"
#endif

#if JMATHS_PERIODICALLY_RESEED_RAND
    update_();
#endif

    return distrib_(gen_);
}

template <typename T> void rand_gen<T>::update_() {
    JMATHS_FUNCTION_TO_LOG;

    static constexpr auto max_unseeded_duration = std::chrono::months(2);

    if (const auto current_time = clock_type::now();
        current_time - last_seed_time_ > max_unseeded_duration) {
        reseed_();
        last_seed_time_ = current_time;
    }
}

template <typename T> void rand_gen<T>::reseed_(generator_type::result_type seed) {
    JMATHS_FUNCTION_TO_LOG;

    gen_.seed(seed);
}

}  // namespace jmaths::internal

// member functions of rand
namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
auto rand<basic_N<BaseInt, BaseIntBig, Allocator>>::generate(bitcount_t upper_bound_exponent)
    -> basic_N_type {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Random arbitrary-precision unsigned integer generation
    // Generates a random number with at most 'upper_bound_exponent' bits.
    //
    // Strategy:
    // 1. Calculate how many full digits are needed (upper_bound_exponent / bits_per_digit)
    // 2. Generate random values for each full digit using the base random generator
    // 3. If there are remaining bits (upper_bound_exponent % bits_per_digit), generate
    //    a partial digit with only those bits set randomly
    // 4. Remove any leading zeros that may have resulted
    //
    // This ensures uniform distribution across the range [0, 2^upper_bound_exponent - 1]
    //
    // Example: For upper_bound_exponent = 100 with 32-bit digits:
    // - Need 3 full digits (96 bits) + 4 additional bits
    // - Generate 3 random 32-bit values + 1 random 4-bit value

    static internal::rand_gen<typename basic_N_type::base_int_type> random_base_int;

    const std::size_t pos_whole = upper_bound_exponent / basic_N_type::base_int_type_bits;

    basic_N_type random_number;
    random_number.digits_.reserve(pos_whole + 1U);

    const bitpos_t pos_mod = upper_bound_exponent % basic_N_type::base_int_type_bits;

    // Generate full random digits
    for (std::size_t i = 0U; i < pos_whole; ++i) {
        random_number.digits_.emplace_back(random_base_int());
    }

    // Generate partial digit for remaining bits if needed
    if (pos_mod > 0U) {
        random_number.digits_.emplace_back(random_base_int() >>
                                           (basic_N_type::base_int_type_bits - pos_mod));
    }

    random_number.remove_leading_zeroes_();

    return random_number;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
auto rand<basic_Z<BaseInt, BaseIntBig, Allocator>>::generate(bitcount_t upper_bound_exponent)
    -> basic_Z_type {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Random arbitrary-precision signed integer generation
    // Generates a random signed integer with at most 'upper_bound_exponent' bits.
    //
    // Strategy:
    // 1. Generate the magnitude using the unsigned random generator
    // 2. Randomly choose the sign (positive or negative) with equal probability
    // 3. Handle zero specially (always positive)
    //
    // This provides uniform distribution over both positive and negative values.

    static internal::rand_gen<bool> random_bool;

    auto random_number = rand<typename basic_Z_type::basic_N_type>::generate(upper_bound_exponent);

    if (random_number.is_zero()) { return basic_Z_type{}; }

    return {std::move(random_number), static_cast<sign_type::sign_bool>(random_bool())};
}

}  // namespace jmaths
