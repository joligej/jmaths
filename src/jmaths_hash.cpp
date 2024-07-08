#include <cstddef>
#include <string_view>

#include "headers/jmaths_constants_and_types.hpp"
#include "headers/jmaths_hash.hpp"
#include "headers/jmaths_N.hpp"
#include "headers/jmaths_Z.hpp"
#include "headers/jmaths_Q.hpp"

#include "headers/jmaths_def.hh"

std::size_t std::hash<jmaths::N>::operator () (const jmaths::N & n) const {
    FUNCTION_TO_LOG;

    if (n.digits_.empty()) {
        static const auto empty_hash = std::hash<std::string_view>{}(std::string_view{});
        return empty_hash;
    }

    return std::hash<std::string_view>{}(std::string_view((const char*)n.digits_.data(), n.digits_.size() * jmaths::base_int_size));
}

std::size_t std::hash<jmaths::Z>::operator () (const jmaths::Z & z) const {
    FUNCTION_TO_LOG;

    return std::hash<jmaths::N>{}(z) ^ ((std::size_t)z.sign_ << (z.front_() % (sizeof(std::size_t) * jmaths::bits_in_byte)));
}

std::size_t std::hash<jmaths::Q>::operator () (const jmaths::Q & q) const {
    FUNCTION_TO_LOG;

    return (std::hash<jmaths::N>{}(q.num_) ^ std::hash<jmaths::N>{}(q.denom_)) ^ ((std::size_t)q.sign_ << ((q.num_.front_() ^ q.denom_.front_()) % (sizeof(std::size_t) * jmaths::bits_in_byte)));
}

#include "headers/jmaths_undef.hh"
