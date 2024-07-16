#include <iostream>
#include <cstdint>
#include <climits>
#include <cstring>

#include "all.hpp"
#include "def.hh"

int main (int argc, char * argv[]) {
    const bool csv_format = argc > 1 && !std::strcmp(argv[1], "csv");

    using jmaths::N;
    using ull = unsigned long long;

    using random_type = std::uint8_t;
    constexpr auto random_type_exp = sizeof(random_type) * CHAR_BIT;
    constexpr auto max_value = (ull)1 << random_type_exp;

    ull values[max_value]{};

    REPEAT(25'000) {
        const auto random_value = *N::rand(random_type_exp).fits_into<random_type>();
        ++values[random_value];
    }

    if (csv_format) {
        for (std::size_t i = 0; i < max_value; ++i) {
            std::cout << i << ',' << values[i] << '\n';
        }
    } else {
        for (std::size_t i = 0; i < max_value; ++i) {
            std::cout << i << ": ";
            REPEAT(values[i]) {
                std::cout << '|';
            }
            std::cout << '\n';
        }
    }
}

#include "undef.hh"
