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

#include <compare>
#include <cstddef>
#include <istream>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>

#include "N.hpp"
#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "hash.hpp"
#include "sign_type.hpp"

// declarations of Z and associated functions and types
namespace jmaths {

std::ostream & operator<<(std::ostream & os, const Z & z);
std::istream & operator>>(std::istream & is, Z & z);

Z operator+(const Z & lhs, const Z & rhs);
Z operator-(const Z & lhs, const Z & rhs);
Z operator*(const Z & lhs, const Z & rhs);
std::pair<Z, Z> operator/(const Z & lhs, const Z & rhs);

Z operator&(const Z & lhs, const Z & rhs);
Z operator|(const Z & lhs, const Z & rhs);
Z operator^(const Z & lhs, const Z & rhs);

bool operator==(const Z & lhs, const Z & rhs);
template <typename INT>
    requires std::is_integral_v<INT>
bool operator==(const Z & lhs, INT rhs);
template <typename INT>
    requires std::is_integral_v<INT>
bool operator==(INT lhs, const Z & rhs);

std::strong_ordering operator<=>(const Z & lhs, const Z & rhs);
template <typename INT>
    requires std::is_integral_v<INT>
std::strong_ordering operator<=>(const Z & lhs, INT rhs);
template <typename INT>
    requires std::is_integral_v<INT>
std::strong_ordering operator<=>(INT lhs, const Z & rhs);

class Z : public sign_type, private N {
    friend struct detail;
    friend struct calc;
    friend struct std::hash<Z>;

    friend class Q;

   private:
    Z(N && n, sign_bool sign);
    Z(const N & n, sign_bool sign);

    std::size_t dynamic_size_() const;

   public:
    using N::bit_reference, N::const_bit_reference;
    using N::ctz, N::bits, N::operator bool, N::operator[];
    using N::is_even, N::is_odd;

    Z();
    Z(std::string_view num_str, unsigned base = default_base);
    template <typename INT>
        requires std::is_integral_v<INT>
    Z(INT num);

    Z(const N & n);
    Z(N && n);

    const N & abs() const &;
    N && abs() &&;

    virtual bool is_zero() const;
    bool is_one() const;
    bool is_neg_one() const;

    std::size_t size() const;  // size of this object in bytes

    std::string to_str(unsigned base = default_base)
        const;                   // convert to string in any base >= 2 and <= 64
    std::string to_hex() const;  // convert to string in base 16 (assumes base
                                 // is an integer power of 2)
    template <typename INT>
        requires std::is_integral_v<INT>
    std::optional<INT> fits_into() const;

    Z & operator++();
    Z & operator--();

    Z & operator+=(const Z & rhs);
    Z & operator-=(const Z & rhs);
    Z & operator*=(const Z & rhs);

    Z & operator&=(const Z & rhs);
    Z & operator|=(const Z & rhs);
    Z & operator^=(const Z & rhs);

    Z operator-() const &;
    Z && operator-() &&;
    Z operator~() const;
    Z operator<<(bit_type pos) const;
    Z operator>>(bit_type pos) const;

    Z & operator<<=(bit_type pos);
    Z & operator>>=(bit_type pos);

    Z & operator=(std::string_view num_str);
    template <typename INT>
        requires std::is_integral_v<INT>
    Z & operator=(INT rhs);

    Z & operator=(const N & n);
    Z & operator=(N && n);

    static Z rand(bit_type upper_bound_exponent);
};

}  // namespace jmaths

#include "tmpl_Z.hpp"
