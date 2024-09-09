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

#include "declarations.hpp"

// declarations of sign_type and associated functions and types
namespace jmaths {

class sign_type {
    friend struct calc;

    friend struct rand<N>;
    friend struct rand<Z>;

   public:
    [[nodiscard]] bool is_positive() const;
    [[nodiscard]] bool is_negative() const;

    [[nodiscard]] virtual bool is_zero() const = 0;
    void flip_sign();

   protected:
    enum sign_bool : bool { positive = 0, negative = 1 } sign_;

    sign_type();
    sign_type(sign_bool sign);
    sign_type(std::string_view * num_str);
    sign_type(std::integral auto * num);

    [[nodiscard]] static sign_bool handle_string_(std::string_view * num_str);
    [[nodiscard]] static sign_bool handle_int_(std::integral auto * num);

    void set_sign_(std::convertible_to<std::underlying_type_t<sign_bool>> auto val);
};

}  // namespace jmaths

#include "tmpl_sign_type.hpp"
