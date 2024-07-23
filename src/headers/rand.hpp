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

#include "def.hh"

namespace jmaths::internal {

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

}  // namespace jmaths::internal

#include "undef.hh"
