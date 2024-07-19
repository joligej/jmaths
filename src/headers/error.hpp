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

// definitions of error and associated types
namespace jmaths {

class error : public std::exception {
   private:
    const char * const message_;
    static const char default_message_[];

   public:
    class division_by_zero;
    class invalid_base;

    error();
    explicit error(const char * message);
    virtual const char * what() const noexcept;
};

class error::division_by_zero : public error {
   private:
    static const char default_message_[];

   public:
    division_by_zero();
    explicit division_by_zero(const char * message);
};

class error::invalid_base : public error {
   private:
    static const char default_message_[];

   public:
    invalid_base();
    explicit invalid_base(const char * message);
};

}  // namespace jmaths
