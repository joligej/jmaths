#include "../src/jmaths.hpp"

#include "../config/jmaths_def.cfg"

namespace jmaths {

N N::rand (std::size_t upper_bound_exponent) {
    FUNCTION_TO_LOG;

    static std::mt19937 gen (std::random_device{}());
    static std::uniform_int_distribution<base_int> distrib;

    N random_number;
    random_number.digits_.reserve(upper_bound_exponent);

    for (std::size_t i = 0; i < upper_bound_exponent; ++i) {
        random_number.digits_.emplace_back(distrib(gen));
    }

    random_number.remove_leading_zeroes_();

    return random_number;
}

Z Z::rand (std::size_t upper_bound_exponent) {
    FUNCTION_TO_LOG;

    static std::mt19937 gen (std::random_device{}());
    static std::uniform_int_distribution<int> distrib (0, 1);

    return Z(N::rand(upper_bound_exponent), static_cast<sign_bool>(distrib(gen)));
}

} // /namespace jmaths

#include "../config/jmaths_undef.cfg"
