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
#include <type_traits>
#include <utility>
#include <vector>

#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "formatter.hpp"
#include "hash.hpp"

// declarations of N and associated functions and types
namespace jmaths {

std::ostream & operator<<(std::ostream & os, const N & n);
std::istream & operator>>(std::istream & is, N & n);

N operator+(const N & lhs, const N & rhs);
N operator-(const N & lhs, const N & rhs);
N operator*(const N & lhs, const N & rhs);
std::pair<N, N> operator/(const N & lhs, const N & rhs);

N operator&(const N & lhs, const N & rhs);
N operator|(const N & lhs, const N & rhs);
N operator^(const N & lhs, const N & rhs);

bool operator==(const N & lhs, const N & rhs);
bool operator==(const N & lhs, std::integral auto rhs);
bool operator==(std::integral auto lhs, const N & rhs);

std::strong_ordering operator<=>(const N & lhs, const N & rhs);
std::strong_ordering operator<=>(const N & lhs, std::integral auto rhs);
std::strong_ordering operator<=>(std::integral auto lhs, const N & rhs);

class N {
    friend struct detail;
    friend struct calc;
    friend struct std::hash<N>;
    friend struct std::hash<Z>;
    friend struct std::hash<Q>;

    friend class Q;

    friend struct rand<N>;
    friend struct rand<Z>;

    friend struct format_output<N>;

   public:
    class bit_reference;
    class const_bit_reference;

    N();
    explicit N(std::string_view num_str, unsigned base = default_base);
    N(std::integral auto num);

    [[nodiscard]] bool is_zero() const;
    [[nodiscard]] bool is_one() const;

    [[nodiscard]] bool is_even() const;
    [[nodiscard]] bool is_odd() const;

    [[nodiscard]] bitcount_t ctz() const;   // count trailing zeroes
    [[nodiscard]] bitcount_t bits() const;  // count number of base 2 digits

    [[nodiscard]] std::size_t size() const;  // size of this object in bytes

    [[nodiscard]] std::string to_str(
        unsigned base = default_base) const;   // convert to string in any base >= 2 and <= 64
    [[nodiscard]] std::string to_hex() const;  // convert to string in base 16 (assumes base
                                               // is an integer power of 2)
    [[nodiscard]] std::string to_bin() const;  // convert to string in base 2

    explicit operator bool() const;
    template <std::unsigned_integral T> [[nodiscard]] std::optional<T> fits_into() const;
    template <std::signed_integral T> [[nodiscard]] std::optional<T> fits_into() const;

    [[nodiscard]] bit_reference operator[](bitpos_t pos);
    [[nodiscard]] const_bit_reference operator[](bitpos_t pos) const;

    void set_zero();

    N & operator++();
    N & operator--();

    N & operator+=(const N & rhs);
    N & operator-=(const N & rhs);
    N & operator*=(const N & rhs);

    N & operator&=(const N & rhs);
    N & operator|=(const N & rhs);
    N & operator^=(const N & rhs);

    [[nodiscard]] N operator~() const;
    [[nodiscard]] N operator<<(bitcount_t pos) const;
    [[nodiscard]] N operator>>(bitcount_t pos) const;

    N & operator<<=(bitcount_t pos);
    N & operator>>=(bitcount_t pos);

    N & operator=(std::string_view num_str);
    N & operator=(std::integral auto rhs);

   protected:
    [[nodiscard]] std::size_t dynamic_size_() const;

    void opr_incr_();
    void opr_decr_();

    void opr_add_assign_(const N & rhs);
    void opr_subtr_assign_(const N & rhs);
    void opr_mult_assign_(const N & rhs);

    void opr_and_assign_(const N & rhs);
    void opr_or_assign_(const N & rhs);
    void opr_xor_assign_(const N & rhs);

    [[nodiscard]] N opr_compl_() const;
    [[nodiscard]] N opr_bitshift_l_(bitcount_t pos) const;
    [[nodiscard]] N opr_bitshift_r_(bitcount_t pos) const;

    void opr_bitshift_l_assign_(bitcount_t pos);
    void opr_bitshift_r_assign_(bitcount_t pos);

    void opr_assign_(std::string_view num_str);
    void opr_assign_(std::integral auto rhs);

    [[nodiscard]] std::string conv_to_base_(unsigned base = default_base) const;

   private:
    std::vector<base_int, allocator<base_int>> digits_;

    void remove_leading_zeroes_();
    [[nodiscard]] base_int front_() const;
    void handle_str_(std::string_view num_str, unsigned base);
    void handle_int_(std::integral auto num);

    template <std::unsigned_integral T> [[nodiscard]] T fit_into_(std::size_t max_byte) const;

    [[nodiscard]] bool bit_(bitpos_t pos) const;
    void bit_(bitpos_t pos, bool val);

    template <typename T>
        requires std::same_as<N, std::decay_t<T>>
    class bit_reference_base_;

    static const N one_;
};

template <typename T>
    requires std::same_as<N, std::decay_t<T>>
class N::bit_reference_base_ {
    friend class bit_reference;
    friend class const_bit_reference;

   public:
    bit_reference_base_() = delete;
    bit_reference_base_(T &&, bitpos_t) = delete;
    bit_reference_base_(T & num, bitpos_t pos);

    operator bool() const;
    explicit operator int() const;

   private:
    T & num_;
    const bitpos_t pos_;
};

class N::bit_reference : public N::bit_reference_base_<N> {
   public:
    using bit_reference_base_::bit_reference_base_;

    bit_reference(const bit_reference & ref);

    bit_reference & operator=(bool val);
    bit_reference & operator=(const bit_reference & ref);
    bit_reference & operator=(const const_bit_reference & ref);
};

class N::const_bit_reference : public bit_reference_base_<const N> {
   public:
    using bit_reference_base_::bit_reference_base_;

    const_bit_reference(const const_bit_reference & ref);
    const_bit_reference(const bit_reference & ref);

    auto operator=(bool) = delete;
    auto operator=(const bit_reference &) = delete;
    auto operator=(const const_bit_reference &) = delete;
};

}  // namespace jmaths

template <> struct std::formatter<jmaths::N> : jmaths::format_output<jmaths::N> {};

#include "tmpl_N.hpp"
