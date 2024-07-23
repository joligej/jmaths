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

#include "hash.hpp"

#include <cstddef>
#include <string_view>

#include "N.hpp"
#include "Q.hpp"
#include "Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"

std::size_t std::hash<jmaths::N>::operator()(const jmaths::N & n) const {
    FUNCTION_TO_LOG;

    if (n.digits_.empty()) {
        static const auto empty_hash = std::hash<std::string_view>{}(std::string_view{});
        return empty_hash;
    }

    return std::hash<std::string_view>{}(
        std::string_view((const char *)n.digits_.data(), n.digits_.size() * jmaths::base_int_size));
}

std::size_t std::hash<jmaths::Z>::operator()(const jmaths::Z & z) const {
    FUNCTION_TO_LOG;

    return std::hash<jmaths::N>{}(z) ^
           ((std::size_t)z.sign_ << (z.front_() % (sizeof(std::size_t) * jmaths::bits_in_byte)));
}

std::size_t std::hash<jmaths::Q>::operator()(const jmaths::Q & q) const {
    FUNCTION_TO_LOG;

    return (std::hash<jmaths::N>{}(q.num_) ^ std::hash<jmaths::N>{}(q.denom_)) ^
           ((std::size_t)q.sign_ << ((q.num_.front_() ^ q.denom_.front_()) %
                                     (sizeof(std::size_t) * jmaths::bits_in_byte)));
}

#include "undef.hh"
