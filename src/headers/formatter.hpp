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

#include <algorithm>
#include <format>

#include "constants_and_types.hpp"
#include "error.hpp"

namespace jmaths {

struct format_parser {
    unsigned base = default_base;

    constexpr auto parse(auto & ctx) {
        auto it = ctx.begin();

        if (it == ctx.end()) { return it; }

        if (*it == '}') { return it; }

        base = 0;

        do {
            base *= 10;

            if (!(*it >= '0' && *it <= '9')) {
                throw std::format_error("Invalid format arguments.");
            }

            base += *it - '0';

            ++it;
        } while (*it != '}');

        if (base < 2U || base > 64U) {
            throw error::invalid_base("You need to enter a base between 2 and 64!");
        }

        return it;
    }
};

template <typename T> struct format_output : format_parser {
    auto format(const T & obj, auto & ctx) const {
        return std::ranges::copy(obj.conv_to_base_(base), ctx.out()).out;
    }
};

}  // namespace jmaths
