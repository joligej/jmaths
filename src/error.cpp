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

#include <string_view>

#include "error.hpp"

#include "def.hh"

namespace jmaths {

error::error() : error(default_message) {
    JMATHS_FUNCTION_TO_LOG;
}

error::error(std::string_view message) : message_(message) {
    JMATHS_FUNCTION_TO_LOG;
};

const char * error::what() const noexcept {
    JMATHS_FUNCTION_TO_LOG;

    return message_.c_str();
}

error::division_by_zero::division_by_zero() : error(default_message) {
    JMATHS_FUNCTION_TO_LOG;
}

error::division_by_zero::division_by_zero(std::string_view message) : error(message) {
    JMATHS_FUNCTION_TO_LOG;
};

error::invalid_base::invalid_base() : error(default_message) {
    JMATHS_FUNCTION_TO_LOG;
}

error::invalid_base::invalid_base(std::string_view message) : error(message) {
    JMATHS_FUNCTION_TO_LOG;
};

}  // namespace jmaths
