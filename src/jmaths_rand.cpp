#include "headers/jmaths.hpp"

#include "headers/jmaths_def.cfg"

namespace jmaths {

N N::rand (bit_type upper_bound_exponent) {
    FUNCTION_TO_LOG;

    static std::mt19937 gen (std::random_device{}());
    static std::uniform_int_distribution<base_int> distrib;

    const std::size_t pos_whole = upper_bound_exponent / base_int_bits;

    N random_number;
    random_number.digits_.reserve(pos_whole + 1);

    const bit_type pos_mod = upper_bound_exponent % base_int_bits;

    for (std::size_t i = 0; i < pos_whole; ++i) {
        random_number.digits_.emplace_back(distrib(gen));
    }

    if (pos_mod > 0) {
        random_number.digits_.emplace_back(distrib(gen) >> (base_int_bits - pos_mod));
    }

    random_number.remove_leading_zeroes_();

    return random_number;
}

Z Z::rand (bit_type upper_bound_exponent) {
    FUNCTION_TO_LOG;

    static std::mt19937 gen (std::random_device{}());
    static std::uniform_int_distribution<int> distrib (0, 1);

    return Z(N::rand(upper_bound_exponent), static_cast<sign_bool>(distrib(gen)));
}

} // /namespace jmaths

#include "headers/jmaths_undef.cfg"
