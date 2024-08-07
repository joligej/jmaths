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

namespace std {

size_t hash<jmaths::N>::operator()(const jmaths::N & n) const {
    JMATHS_FUNCTION_TO_LOG;

    if (n.digits_.empty()) {
        static const auto empty_hash = hash<string_view>{}(string_view{});
        return empty_hash;
    }

    return hash<string_view>{}(string_view(reinterpret_cast<const char *>(n.digits_.data()),
                                           n.digits_.size() * jmaths::base_int_size));
}

size_t hash<jmaths::Z>::operator()(const jmaths::Z & z) const {
    JMATHS_FUNCTION_TO_LOG;

    return hash<jmaths::N>{}(z) ^
           (static_cast<size_t>(z.sign_) << (z.front_() % (sizeof(size_t) * jmaths::bits_in_byte)));
}

size_t hash<jmaths::Q>::operator()(const jmaths::Q & q) const {
    JMATHS_FUNCTION_TO_LOG;

    return (hash<jmaths::N>{}(q.num_) ^ hash<jmaths::N>{}(q.denom_)) ^
           (static_cast<size_t>(q.sign_)
            << ((q.num_.front_() ^ q.denom_.front_()) % (sizeof(size_t) * jmaths::bits_in_byte)));
}

}  // namespace std

#include "undef.hh"
