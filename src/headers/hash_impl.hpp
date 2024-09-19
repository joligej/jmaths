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

#include <cstddef>
#include <string_view>

#include "basic_N.hpp"
#include "basic_Q.hpp"
#include "basic_Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "hash.hpp"

template <jmaths::TMP::instance_of<jmaths::basic_N> N>
inline std::size_t std::hash<N>::operator()(const N & n) {
    JMATHS_FUNCTION_TO_LOG;

    if (n.digits_.empty()) {
        static const auto empty_hash = std::hash<std::string_view>{}(std::string_view{});
        return empty_hash;
    }

    return std::hash<std::string_view>{}(
        std::string_view(reinterpret_cast<const char *>(n.digits_.data()),
                         n.digits_.size() * N::base_int_type_size));
}

template <jmaths::TMP::instance_of<jmaths::basic_Z> Z>
inline std::size_t std::hash<Z>::operator()(const Z & z) {
    JMATHS_FUNCTION_TO_LOG;

    using hash_N = std::hash<typename Z::N>;

    return hash_N{}(z) ^ (static_cast<std::size_t>(z.sign_)
                          << (z.front_() % (sizeof(std::size_t) * jmaths::bits_in_byte)));
}

template <jmaths::TMP::instance_of<jmaths::basic_Q> Q>
inline std::size_t std::hash<Q>::operator()(const Q & q) {
    JMATHS_FUNCTION_TO_LOG;

    using hash_N = std::hash<typename Q::N>;

    return (hash_N{}(q.num_) ^ hash_N{}(q.denom_)) ^
           (static_cast<std::size_t>(q.sign_) << ((q.num_.front_() ^ q.denom_.front_()) %
                                                  (sizeof(std::size_t) * jmaths::bits_in_byte)));
}
