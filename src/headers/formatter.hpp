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

#include "TMP.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "error.hpp"

namespace jmaths {

struct format_parser {
    unsigned base = default_base;

    constexpr auto parse(auto & ctx) {
        auto it = ctx.begin();

        if (it == ctx.end()) { return it; }

        if (*it == '}') { return it; }

        base = 0U;

        do {
            base *= 10U;

            if (!(*it >= '0' && *it <= '9')) {
                throw std::format_error("Invalid format arguments.");
            }

            base += static_cast<unsigned>(*it - '0');

            ++it;
        } while (*it != '}');

        error::invalid_base::check(base);

        return it;
    }
};

template <TMP::instance_of<basic_N, basic_Z, basic_Q> T> struct format_output : format_parser {
    auto format(const T & obj, auto & ctx) const {
        JMATHS_FUNCTION_TO_LOG;

        switch (base) {
            case 2U:
                return std::ranges::copy(obj.to_bin(), ctx.out()).out;
            case 16U:
                return std::ranges::copy(obj.to_hex(), ctx.out()).out;
            default:
                return std::ranges::copy(obj.conv_to_base_(base), ctx.out()).out;
        }
    }
};

}  // namespace jmaths
