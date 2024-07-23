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

#include "sign_type.hpp"

#include <string_view>

#include "constants_and_types.hpp"
#include "def.hh"

// functions for jmaths::sign_type
namespace jmaths {

sign_type::sign_type() : sign_(positive) {
    FUNCTION_TO_LOG;
}

sign_type::sign_type(sign_bool sign) : sign_(sign) {
    FUNCTION_TO_LOG;
}

sign_type::sign_type(std::string_view * num_str) : sign_(handle_string_(num_str)) {
    FUNCTION_TO_LOG;
}

sign_type::sign_bool sign_type::handle_string_(std::string_view * num_str) {
    FUNCTION_TO_LOG;

    if (!num_str->empty() && num_str->front() == negative_sign) {
        num_str->remove_prefix(1);
        return num_str->empty() ? positive : negative;
    } else {
        return positive;
    }
}

bool sign_type::is_positive() const {
    FUNCTION_TO_LOG;

    return (sign_ == positive);
}

bool sign_type::is_negative() const {
    FUNCTION_TO_LOG;

    return (sign_ == negative);
}

void sign_type::flip_sign() {
    FUNCTION_TO_LOG;

    if (is_zero()) return;
    set_sign_(!sign_);
}

}  // namespace jmaths

#include "undef.hh"
