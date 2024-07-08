#pragma once

#include <random>
#include <limits>

#include "constants_and_types.hpp"

namespace jmaths {

namespace internal {

template <typename INT>
class rand {
    private:
        using seed_type = std::random_device;

        inline static std::mt19937 gen {seed_type{}()};

        std::uniform_int_distribution<INT> distrib;
 
    public:
        rand (INT min, INT max) : distrib(min, max) {}
        rand() : rand(0, std::numeric_limits<INT>::max()) {}

        INT operator () () {
            return distrib(gen);
        }

        template <typename SEED>
        static void reseed (SEED seed = seed_type{}()) {
            gen.seed(seed);
        }
};

} // /namespace internal

} // /namespace jmaths