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

#include <string_view>
#include <type_traits>

#include "declarations.hpp"

// declarations of sign_type and associated functions and types
namespace jmaths {

class sign_type {
    friend struct detail;
    friend struct calc;

   protected:
    enum sign_bool : bool { positive = 0, negative = 1 } sign_;

    sign_type();
    sign_type(sign_bool sign);
    sign_type(std::string_view & num_str);
    template <typename INT>
        requires std::is_integral_v<INT>
    sign_type(INT & num);

    static sign_bool handle_string_(std::string_view & num_str);
    static std::string_view handle_fraction_string_(std::string_view & num_str);
    template <typename INT>
        requires std::is_integral_v<INT>
    static sign_bool handle_int_(INT & num);

    template <typename BOOL>
        requires std::is_convertible_v<BOOL, std::underlying_type_t<sign_bool>>
    void set_sign_(BOOL val);

   public:
    bool is_positive() const;
    bool is_negative() const;

    virtual bool is_zero() const = 0;
    void flip_sign();
};

}  // namespace jmaths

#include "tmpl_sign_type.hpp"
