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

#include <concepts>
#include <string_view>
#include <type_traits>

#include "constants_and_types.hpp"
#include "def.hh"
#include "sign_type.hpp"

// member function templates of sign_type
namespace jmaths {

constexpr sign_type::sign_type(std::integral auto * num) : sign_{handle_int_(num)} {
    JMATHS_FUNCTION_TO_LOG;
}

constexpr sign_type::sign_bool sign_type::handle_int_(std::integral auto * num) {
    JMATHS_FUNCTION_TO_LOG;

    if constexpr (std::unsigned_integral<decltype(*num)>) {
        return positive;
    } else {
        if (*num < 0) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
            *num *= -1;
#pragma GCC diagnostic pop
            return negative;
        } else {
            return positive;
        }
    }
}

constexpr void sign_type::set_sign_(
    std::convertible_to<std::underlying_type_t<sign_bool>> auto val) {
    JMATHS_FUNCTION_TO_LOG;

    sign_ = static_cast<sign_bool>(val);
}

}  // namespace jmaths

// member functions of sign_type
namespace jmaths {

constexpr sign_type::sign_type() : sign_{positive} {
    JMATHS_FUNCTION_TO_LOG;
}

constexpr sign_type::sign_type(sign_bool sign) : sign_{sign} {
    JMATHS_FUNCTION_TO_LOG;
}

constexpr sign_type::sign_type(std::string_view * num_str) : sign_{handle_string_(num_str)} {
    JMATHS_FUNCTION_TO_LOG;
}

constexpr sign_type::sign_bool sign_type::handle_string_(std::string_view * num_str) {
    JMATHS_FUNCTION_TO_LOG;

    if (!num_str->empty() && num_str->front() == negative_sign) {
        num_str->remove_prefix(1U);
        return num_str->empty() ? positive : negative;
    } else {
        return positive;
    }
}

constexpr bool sign_type::is_positive() const {
    JMATHS_FUNCTION_TO_LOG;

    return sign_ == positive;
}

constexpr bool sign_type::is_negative() const {
    JMATHS_FUNCTION_TO_LOG;

    return sign_ == negative;
}

constexpr void sign_type::flip_sign() {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return; }
    set_sign_(!sign_);
}

}  // namespace jmaths
