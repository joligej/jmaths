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
#include <format>
#include <istream>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

#include "N.hpp"
#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "formatter.hpp"
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
bool operator==(const Z & lhs, std::integral auto rhs);
bool operator==(std::integral auto lhs, const Z & rhs);

std::strong_ordering operator<=>(const Z & lhs, const Z & rhs);
std::strong_ordering operator<=>(const Z & lhs, std::integral auto rhs);
std::strong_ordering operator<=>(std::integral auto lhs, const Z & rhs);

class Z final : public sign_type, private N {
    friend struct calc;
    friend struct std::hash<Z>;

    friend class Q;

    friend struct rand<N>;
    friend struct rand<Z>;

    friend struct format_output<Z>;

   public:
    using N::bit_reference, N::const_bit_reference;
    using N::ctz, N::bits, N::operator bool, N::operator[];
    using N::is_even, N::is_odd;

    struct detail;

    Z();
    explicit Z(std::string_view num_str, unsigned base = default_base);
    Z(std::integral auto num);

    Z(const N & n);
    Z(N && n);

    [[nodiscard]] const N & abs() const &;
    [[nodiscard]] N && abs() &&;

    [[nodiscard]] bool is_zero() const override;
    [[nodiscard]] bool is_one() const;
    [[nodiscard]] bool is_neg_one() const;

    [[nodiscard]] std::size_t size() const;  // size of this object in bytes

    [[nodiscard]] std::string to_str(
        unsigned base = default_base) const;   // convert to string in any base >= 2 and <= 64
    [[nodiscard]] std::string to_hex() const;  // convert to string in base 16 (assumes base
                                               // is an integer power of 2)
    [[nodiscard]] std::string to_bin() const;  // convert to string in base 2

    template <std::unsigned_integral T> [[nodiscard]] std::optional<T> fits_into() const;
    template <std::signed_integral T> [[nodiscard]] std::optional<T> fits_into() const;

    void set_zero();

    Z & operator++();
    Z & operator--();

    Z & operator+=(const Z & rhs);
    Z & operator-=(const Z & rhs);
    Z & operator*=(const Z & rhs);

    Z & operator&=(const Z & rhs);
    Z & operator|=(const Z & rhs);
    Z & operator^=(const Z & rhs);

    [[nodiscard]] Z operator-() const &;
    [[nodiscard]] Z && operator-() &&;
    [[nodiscard]] Z operator~() const;
    [[nodiscard]] Z operator<<(bitcount_t pos) const;
    [[nodiscard]] Z operator>>(bitcount_t pos) const;

    Z & operator<<=(bitcount_t pos);
    Z & operator>>=(bitcount_t pos);

    Z & operator=(std::string_view num_str);
    Z & operator=(std::integral auto rhs);

    Z & operator=(const N & n);
    Z & operator=(N && n);

   private:
    Z(N && n, sign_bool sign);
    Z(const N & n, sign_bool sign);

    [[nodiscard]] std::string conv_to_base_(unsigned base = default_base) const;

    [[nodiscard]] std::size_t dynamic_size_() const;
};

}  // namespace jmaths

namespace jmaths {

struct Z::detail {
    static std::ostream & opr_ins(std::ostream & os, const Z & z);
    static std::istream & opr_extr(std::istream & is, Z & z);

    static Z opr_add(const Z & lhs, const Z & rhs);
    static Z opr_subtr(const Z & lhs, const Z & rhs);
    static Z opr_mult(const Z & lhs, const Z & rhs);
    static std::pair<Z, Z> opr_div(const Z & lhs, const Z & rhs);

    static Z opr_and(const Z & lhs, const Z & rhs);
    static Z opr_or(const Z & lhs, const Z & rhs);
    static Z opr_xor(const Z & lhs, const Z & rhs);

    static bool opr_eq(const Z & lhs, const Z & rhs);
    static bool opr_eq(const Z & lhs, std::integral auto rhs);

    static std::strong_ordering opr_comp(const Z & lhs, const Z & rhs);
    static std::strong_ordering opr_comp(const Z & lhs, std::integral auto rhs);
};

}  // namespace jmaths

template <> struct std::formatter<jmaths::Z> : jmaths::format_output<jmaths::Z> {};

#include "tmpl_Z.hpp"
