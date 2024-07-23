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
#include <concepts>
#include <cstddef>
#include <istream>
#include <limits>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>

#include "N.hpp"
#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "hash.hpp"
#include "sign_type.hpp"

// declarations of Q and associated functions and types
namespace jmaths {

std::ostream & operator<<(std::ostream & os, const Q & q);
std::istream & operator>>(std::istream & is, Q & q);

Q operator+(const Q & lhs, const Q & rhs);
Q operator-(const Q & lhs, const Q & rhs);
Q operator*(const Q & lhs, const Q & rhs);
Q operator/(const Q & lhs, const Q & rhs);

Q operator&(const Q & lhs, const Q & rhs);
Q operator|(const Q & lhs, const Q & rhs);
Q operator^(const Q & lhs, const Q & rhs);

bool operator==(const Q & lhs, const Q & rhs);
bool operator==(const Q & lhs, std::floating_point auto rhs);
bool operator==(std::floating_point auto lhs, const Q & rhs);

std::strong_ordering operator<=>(const Q & lhs, const Q & rhs);
std::strong_ordering operator<=>(const Q & lhs, std::floating_point auto rhs);
std::strong_ordering operator<=>(std::floating_point auto lhs, const Q & rhs);

class Q : public sign_type {
    friend struct detail;
    friend struct calc;
    friend struct std::hash<Q>;

   private:
    N num_, denom_;

    Q(N && num, N && denom, sign_bool sign);
    Q(const N & num, const N & denom, sign_bool sign);
    Q(std::tuple<N, N, sign_bool> && fraction_info);

    static std::string_view handle_fraction_string_(std::string_view * num_str);

    void canonicalise_();

    static std::tuple<N, N, sign_bool> handle_float_(std::floating_point auto num);

    std::size_t dynamic_size_() const;

   public:
    Q();
    Q(std::string_view num_str, unsigned base = default_base);
    Q(std::floating_point auto num);

    Q(const N & n);
    Q(N && n);

    Q(const Z & z);
    Q(Z && z);

    Q(const N & num, const N & denom);
    Q(const N & num, N && denom);
    Q(N && num, const N & denom);
    Q(N && num, N && denom);

    Q(const Z & num, const Z & denom);
    Q(const Z & num, Z && denom);
    Q(Z && num, const Z & denom);
    Q(Z && num, Z && denom);

    bool is_zero() const override;
    bool is_one() const;
    bool is_neg_one() const;

    Q abs() const &;
    Q && abs() &&;

    Q inverse() const &;
    Q && inverse() &&;

    std::size_t size() const;  // size of this object in bytes

    std::string to_str(unsigned base = default_base) const;  // convert to string in any base >= 2 and <= 64
    std::string to_hex() const;                              // convert to string in base 16 (assumes base
                                                             // is an integer power of 2)
    explicit operator bool() const;
    template <std::floating_point T>
        requires std::numeric_limits<T>::is_iec559
    std::optional<T> fits_into() const;

    Q & operator++();
    Q & operator--();

    Q & operator+=(const Q & rhs);
    Q & operator-=(const Q & rhs);
    Q & operator*=(const Q & rhs);
    Q & operator/=(const Q & rhs);

    Q & operator&=(const Q & rhs);
    Q & operator|=(const Q & rhs);
    Q & operator^=(const Q & rhs);

    Q operator-() const &;
    Q && operator-() &&;
    Q operator~() const;
    Q operator<<(bit_type pos) const;
    Q operator>>(bit_type pos) const;

    Q & operator<<=(bit_type pos);
    Q & operator>>=(bit_type pos);

    Q & operator=(std::string_view num_str);
    Q & operator=(std::floating_point auto rhs);

    Q & operator=(const N & n);
    Q & operator=(N && n);

    Q & operator=(const Z & z);
    Q & operator=(Z && z);
};

}  // namespace jmaths

#include "tmpl_Q.hpp"
