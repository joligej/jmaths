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

#include "error.hpp"

#include "def.hh"

namespace jmaths {

constexpr char error::default_message[] = "No error message provided!";

constexpr char error::division_by_zero::default_message[] = "You tried to divide by zero!";

constexpr char error::invalid_base::default_message[] = "You tried to use an invalid base!";

error::error() : error(default_message) {
    JMATHS_FUNCTION_TO_LOG;
}

error::error(const char * message) : message_{message} {
    JMATHS_FUNCTION_TO_LOG;
};

const char * error::what() const noexcept {
    JMATHS_FUNCTION_TO_LOG;

    return message_;
}

error::division_by_zero::division_by_zero() : error(default_message) {
    JMATHS_FUNCTION_TO_LOG;
}

error::division_by_zero::division_by_zero(const char * message) : error(message) {
    JMATHS_FUNCTION_TO_LOG;
};

error::invalid_base::invalid_base() : error(default_message) {
    JMATHS_FUNCTION_TO_LOG;
}

error::invalid_base::invalid_base(const char * message) : error(message) {
    JMATHS_FUNCTION_TO_LOG;
};

}  // namespace jmaths
