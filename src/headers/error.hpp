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

#include <cstring>
#include <exception>
#include <format>

// definitions of error and associated types
namespace jmaths {

class error : public std::exception {
   public:
    static constexpr char default_message[] = "No error message provided!";
    class division_by_zero;
    class invalid_base;

    error();
    explicit error(const char * message);
    [[nodiscard]] const char * what() const noexcept override;

   private:
    const char * const message_;
};

class error::division_by_zero : public error {
   public:
    static constexpr char default_message[] = "You tried to divide by zero!";
    division_by_zero();
    explicit division_by_zero(const char * message);
};

class error::invalid_base : public error {
   public:
    static constexpr char default_message[] = "You need to enter a base between 2 and 64!";
    invalid_base();
    explicit invalid_base(const char * message);

    static constexpr void check(unsigned base);

    static constexpr unsigned minimum_base = 2;
    static constexpr unsigned maximum_base = 64;
};

constexpr void error::invalid_base::check(unsigned base) {
    if (base < minimum_base || base > maximum_base) {
#if 0
        static struct message_t {
            char buffer[std::size(default_message) + 50];

            message_t() {
                std::memcpy(buffer, default_message, std::size(default_message) - 1);
            }
        } message;

        std::format_to(message.buffer + std::size(default_message) - 1,
                       " The base you entered was: {}.",
                       base);

        throw invalid_base(message.buffer);
#endif

#if 1
        static char message_buffer[std::size(default_message) + 50];
        std::format_to(message_buffer, "{} The base you entered was: {}.", default_message, base);
        throw invalid_base(message_buffer);
#endif
    }
}

}  // namespace jmaths
