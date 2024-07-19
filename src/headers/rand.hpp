#pragma once

#include <limits>
#include <random>

#include "def.hh"

namespace jmaths {

namespace internal {

template <typename INT> class rand {
   private:
    using seed_type = std::random_device;

    inline static std::mt19937 gen{seed_type{}()};

    std::uniform_int_distribution<INT> distrib;

   public:
    rand(INT min, INT max) : distrib(min, max) {
        FUNCTION_TO_LOG;
    }

    rand() : rand(0, std::numeric_limits<INT>::max()) {
        FUNCTION_TO_LOG;
    }

    INT operator()() {
        FUNCTION_TO_LOG;

        return distrib(gen);
    }

    template <typename SEED> static void reseed(SEED seed = seed_type{}()) {
        FUNCTION_TO_LOG;

        gen.seed(seed);
    }
};

}  // namespace internal

}  // namespace jmaths

#include "undef.hh"
