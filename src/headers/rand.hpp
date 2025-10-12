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
#include <random>
#include <type_traits>

#include "TMP.hpp"
#include "declarations.hpp"

namespace jmaths::internal {

/**
 * @class rand_gen
 * @brief Random number generator wrapper for various integer types
 * @tparam T The type of random numbers to generate
 *
 * DESIGN: Wraps std::mt19937 (Mersenne Twister) with std::uniform_int_distribution
 * for high-quality random number generation. Optionally reseeds periodically
 * for long-running applications (controlled by JMATHS_PERIODICALLY_RESEED_RAND).
 *
 * NOTE: Uses static generator and seed time to share state across all instances
 * of the same type, ensuring consistent random sequence.
 */
template <typename T> class rand_gen {
    // TODO: Currently only unsigned types are in allowed_types. Consider if signed types needed.
    using allowed_types = TMP::pack_container</*short,
                                              int,
                                              long,
                                              long long,*/
                                              unsigned short,
                                              unsigned int,
                                              unsigned long,
                                              unsigned long long>;

    using seed_type = std::random_device;
    using generator_type = std::mt19937;  // Mersenne Twister 19937
    using clock_type = std::chrono::high_resolution_clock;
    using distribution_type = std::uniform_int_distribution<
        std::conditional_t<TMP::matches_any_v<T, std::is_same, allowed_types>, T, unsigned int>>;

   public:
    /**
     * @brief Construct generator with custom range [min, max]
     */
    rand_gen(T min, T max);

    /**
     * @brief Construct generator with full range [0, max value of T]
     */
    rand_gen();

    /**
     * @brief Generate a random number
     * @return Random value in the configured range
     *
     * NOTE: May reseed the generator if too much time has passed since last seed
     * (controlled by JMATHS_PERIODICALLY_RESEED_RAND compile-time flag)
     */
    T operator()();

   private:
    distribution_type distrib_;

    static generator_type gen_;  // Shared generator state
    static clock_type::time_point last_seed_time_;  // Shared last seed time

    /**
     * @brief Check if reseeding is needed and reseed if so
     * Reseeds after 2 months of continuous generation
     */
    static void update_();

    /**
     * @brief Reseed the random number generator
     * @param seed New seed value (default: from std::random_device)
     */
    static void reseed_(generator_type::result_type seed = seed_type{}());
};

}  // namespace jmaths::internal

namespace jmaths {

/**
 * @struct rand
 * @brief Random number generator for arbitrary-precision types
 * @tparam T The type to generate (basic_N or basic_Z)
 *
 * Primary template - specialized for basic_N and basic_Z below.
 */
template <TMP::instance_of<basic_N, basic_Z> T> struct rand {
    /**
     * @brief Generate random number with given bit count
     * @param upper_bound_exponent Maximum number of bits in result
     * @return Random number in range [0, 2^upper_bound_exponent - 1]
     */
    [[nodiscard]] static T generate(bitcount_t upper_bound_exponent);
};

/**
 * @struct rand<basic_N>
 * @brief Specialization for unsigned arbitrary-precision integers
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct rand<basic_N<BaseInt, BaseIntBig, Allocator>> {
    using basic_N_type = basic_N<BaseInt, BaseIntBig, Allocator>;

    /**
     * @brief Generate random unsigned arbitrary-precision integer
     * @param upper_bound_exponent Maximum number of bits
     * @return Random N with at most upper_bound_exponent bits
     *
     * ALGORITHM: Generates random digits using internal rand_gen,
     * assembles into arbitrary-precision number. See rand_impl.hpp for details.
     */
    [[nodiscard]] static basic_N_type generate(bitcount_t upper_bound_exponent);
};

/**
 * @struct rand<basic_Z>
 * @brief Specialization for signed arbitrary-precision integers
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct rand<basic_Z<BaseInt, BaseIntBig, Allocator>> {
    using basic_Z_type = basic_Z<BaseInt, BaseIntBig, Allocator>;

    /**
     * @brief Generate random signed arbitrary-precision integer
     * @param upper_bound_exponent Maximum number of bits (magnitude)
     * @return Random Z with random sign and at most upper_bound_exponent bits
     *
     * ALGORITHM: Generates magnitude using basic_N generator, then randomly
     * assigns positive or negative sign. Zero always positive.
     */
    [[nodiscard]] static basic_Z_type generate(bitcount_t upper_bound_exponent);
};

}  // namespace jmaths

#include "rand_impl.hpp"
