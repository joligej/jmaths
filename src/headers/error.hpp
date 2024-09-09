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

#include <exception>
#include <format>
#include <string>
#include <string_view>

// definitions of error and associated types
namespace jmaths {

class error : public std::exception {
   public:
    static constexpr char default_message[] = "No error message provided!";

    class division_by_zero;
    class invalid_base;

    error();
    explicit error(std::string_view message);
    [[nodiscard]] const char * what() const noexcept override;

   private:
    const std::string message_;
};

class error::division_by_zero : public error {
   public:
    static constexpr char default_message[] = "You tried to divide by zero!";

    division_by_zero();
    explicit division_by_zero(std::string_view message);

    static constexpr void check(const auto & num, std::string_view message = default_message);
};

constexpr void error::division_by_zero::check(const auto & num, std::string_view message) {
    if constexpr (requires { !num.is_zero(); }) {
        if (!num.is_zero()) { return; }
    } else {
        if (num != 0) { return; }
    }

    throw division_by_zero(message);
}

class error::invalid_base : public error {
   public:
    static constexpr char default_message[] = "You need to enter a base between 2 and 64!";

    invalid_base();
    explicit invalid_base(std::string_view message);

    static constexpr void check(unsigned base);

    static constexpr unsigned minimum_base = 2;
    static constexpr unsigned maximum_base = 64;
};

constexpr void error::invalid_base::check(unsigned base) {
    if (base >= minimum_base && base <= maximum_base) { return; }

    throw invalid_base(std::format("{} The base you entered was: {}.", default_message, base));
}

}  // namespace jmaths
