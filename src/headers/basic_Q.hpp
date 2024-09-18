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
#include <cstdint>
#include <format>
#include <istream>
#include <limits>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>

#include "TMP.hpp"
#include "basic_N.hpp"
#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "formatter.hpp"
#include "sign_type.hpp"

// declarations of Q and associated functions and types
namespace jmaths {

template <TMP::instance_of<basic_Q> basic_Q_type>
std::ostream & operator<<(std::ostream & os, const basic_Q_type & q);

template <TMP::instance_of<basic_Q> basic_Q_type>
std::istream & operator>>(std::istream & is, basic_Q_type & q);

template <TMP::instance_of<basic_Q> basic_Q_type>
basic_Q_type operator+(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
basic_Q_type operator-(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
basic_Q_type operator*(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
basic_Q_type operator/(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
basic_Q_type operator&(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
basic_Q_type operator|(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
basic_Q_type operator^(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
bool operator==(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
bool operator==(const basic_Q_type & lhs, std::floating_point auto rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
bool operator==(std::floating_point auto lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
std::strong_ordering operator<=>(const basic_Q_type & lhs, const basic_Q_type & rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
std::strong_ordering operator<=>(const basic_Q_type & lhs, std::floating_point auto rhs);

template <TMP::instance_of<basic_Q> basic_Q_type>
std::strong_ordering operator<=>(std::floating_point auto lhs, const basic_Q_type & rhs);

template <typename BaseInt, typename BaseIntBig, typename Allocator = allocator<BaseInt>>
class basic_Q final : public sign_type {
    using basic_N_type = basic_N<BaseInt, BaseIntBig, Allocator>;
    using basic_Z_type = basic_Z<BaseInt, BaseIntBig, Allocator>;

    using base_int_type = BaseInt;
    using base_int_big_type = BaseIntBig;
    static constexpr base_int_type max_digit = std::numeric_limits<base_int_type>::max();
    static constexpr base_int_big_type radix = static_cast<base_int_big_type>(max_digit) + 1;
    static constexpr std::size_t base_int_type_size = sizeof(base_int_type);
    static constexpr bitcount_t base_int_type_bits = base_int_type_size * bits_in_byte;

    friend struct calc;
    friend struct std::hash<basic_Q>;

    friend struct format_output<basic_Q>;

   public:
    struct detail;

    basic_Q();
    explicit basic_Q(std::string_view num_str, unsigned base = default_base);
    basic_Q(std::floating_point auto num);

    basic_Q(const basic_N_type & n);
    basic_Q(basic_N_type && n);

    basic_Q(const basic_Z_type & z);
    basic_Q(basic_Z_type && z);

    basic_Q(const basic_N_type & num, const basic_N_type & denom);
    basic_Q(const basic_N_type & num, basic_N_type && denom);
    basic_Q(basic_N_type && num, const basic_N_type & denom);
    basic_Q(basic_N_type && num, basic_N_type && denom);

    basic_Q(const basic_Z_type & num, const basic_Z_type & denom);
    basic_Q(const basic_Z_type & num, basic_Z_type && denom);
    basic_Q(basic_Z_type && num, const basic_Z_type & denom);
    basic_Q(basic_Z_type && num, basic_Z_type && denom);

    [[nodiscard]] bool is_zero() const override;
    [[nodiscard]] bool is_one() const;
    [[nodiscard]] bool is_neg_one() const;

    [[nodiscard]] basic_Q abs() const &;
    [[nodiscard]] basic_Q && abs() &&;

    [[nodiscard]] basic_Q inverse() const &;
    [[nodiscard]] basic_Q && inverse() &&;

    [[nodiscard]] std::size_t size() const;  // size of this object in bytes

    [[nodiscard]] std::string to_str(
        unsigned base = default_base) const;   // convert to string in any base >= 2 and <= 64
    [[nodiscard]] std::string to_hex() const;  // convert to string in base 16 (assumes base
                                               // is an integer power of 2)
    [[nodiscard]] std::string to_bin() const;  // convert to string in base 2

    explicit operator bool() const;
    template <std::floating_point T>
        requires std::numeric_limits<T>::is_iec559
    [[nodiscard]] std::optional<T> fits_into() const;

    basic_Q & operator++();
    basic_Q & operator--();

    basic_Q & operator+=(const basic_Q & rhs);
    basic_Q & operator-=(const basic_Q & rhs);
    basic_Q & operator*=(const basic_Q & rhs);
    basic_Q & operator/=(const basic_Q & rhs);

    basic_Q & operator&=(const basic_Q & rhs);
    basic_Q & operator|=(const basic_Q & rhs);
    basic_Q & operator^=(const basic_Q & rhs);

    [[nodiscard]] basic_Q operator-() const &;
    [[nodiscard]] basic_Q && operator-() &&;
    [[nodiscard]] basic_Q operator~() const;
    [[nodiscard]] basic_Q operator<<(bitcount_t pos) const;
    [[nodiscard]] basic_Q operator>>(bitcount_t pos) const;

    basic_Q & operator<<=(bitcount_t pos);
    basic_Q & operator>>=(bitcount_t pos);

    basic_Q & operator=(std::string_view num_str);
    basic_Q & operator=(std::floating_point auto rhs);

    basic_Q & operator=(const basic_N_type & n);
    basic_Q & operator=(basic_N_type && n);

    basic_Q & operator=(const basic_Z_type & z);
    basic_Q & operator=(basic_Z_type && z);

   private:
    basic_N_type num_, denom_;

    basic_Q(basic_N_type && num, basic_N_type && denom, sign_bool sign);
    basic_Q(const basic_N_type & num, const basic_N_type & denom, sign_bool sign);
    basic_Q(std::tuple<basic_N_type, basic_N_type, sign_bool> && fraction_info);

    [[nodiscard]] static std::string_view handle_fraction_string_(std::string_view * num_str);

    void canonicalise_();
    [[nodiscard]] std::string conv_to_base_(unsigned base = default_base) const;

    [[nodiscard]] static std::tuple<basic_N_type, basic_N_type, sign_bool> handle_float_(
        std::floating_point auto num);

    [[nodiscard]] std::size_t dynamic_size_() const;
};

}  // namespace jmaths

namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct basic_Q<BaseInt, BaseIntBig, Allocator>::detail {
    static std::ostream & opr_ins(std::ostream & os, const basic_Q & q);
    static std::istream & opr_extr(std::istream & is, basic_Q & q);

    static basic_Q opr_add(const basic_Q & lhs, const basic_Q & rhs);
    static basic_Q opr_subtr(const basic_Q & lhs, const basic_Q & rhs);
    static basic_Q opr_mult(const basic_Q & lhs, const basic_Q & rhs);
    static basic_Q opr_div(const basic_Q & lhs, const basic_Q & rhs);

    static basic_Q opr_and(const basic_Q & lhs, const basic_Q & rhs);
    static basic_Q opr_or(const basic_Q & lhs, const basic_Q & rhs);
    static basic_Q opr_xor(const basic_Q & lhs, const basic_Q & rhs);

    static bool opr_eq(const basic_Q & lhs, const basic_Q & rhs);
    static bool opr_eq(const basic_Q & lhs, std::floating_point auto rhs);

    static std::strong_ordering opr_comp(const basic_Q & lhs, const basic_Q & rhs);
    static std::strong_ordering opr_comp(const basic_Q & lhs, std::floating_point auto rhs);
};

}  // namespace jmaths

namespace jmaths {

#if defined(UINT64_MAX) && defined(UINT32_MAX)

using Q = basic_Q<std::uint32_t, std::uint64_t>;

#elif defined(UINT32_MAX) && defined(UINT16_MAX)

using Q = basic_Q<std::uint16_t, std::uint32_t>;

#elif defined(UINT16_MAX) && defined(UINT8_MAX)

using Q = basic_Q<std::uint8_t, std::uint16_t>;

#endif

}  // namespace jmaths

template <jmaths::TMP::instance_of<jmaths::basic_Q> basic_Q_type>
struct std::formatter<basic_Q_type> : jmaths::format_output<basic_Q_type> {};

#include "basic_Q_detail_impl.hpp"
#include "basic_Q_impl.hpp"
