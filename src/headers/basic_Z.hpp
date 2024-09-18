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
#include <utility>

#include "TMP.hpp"
#include "basic_N.hpp"
#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "formatter.hpp"
#include "sign_type.hpp"

// declarations of Z and associated functions and types
namespace jmaths {

template <TMP::instance_of<basic_Z> basic_Z_type>
std::ostream & operator<<(std::ostream & os, const basic_Z_type & z);

template <TMP::instance_of<basic_Z> basic_Z_type>
std::istream & operator>>(std::istream & is, basic_Z_type & z);

template <TMP::instance_of<basic_Z> basic_Z_type>
basic_Z_type operator+(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
basic_Z_type operator-(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
basic_Z_type operator*(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
std::pair<basic_Z_type, basic_Z_type> operator/(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
basic_Z_type operator&(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
basic_Z_type operator|(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
basic_Z_type operator^(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
bool operator==(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
bool operator==(const basic_Z_type & lhs, std::integral auto rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
bool operator==(std::integral auto lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
std::strong_ordering operator<=>(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
std::strong_ordering operator<=>(const basic_Z_type & lhs, std::integral auto rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
std::strong_ordering operator<=>(std::integral auto lhs, const basic_Z_type & rhs);

template <typename BaseInt, typename BaseIntBig, typename Allocator = allocator<BaseInt>>
class basic_Z final : public sign_type, private basic_N<BaseInt, BaseIntBig, Allocator> {
    using basic_N_type = basic_N<BaseInt, BaseIntBig, Allocator>;
    using basic_Q_type = basic_Q<BaseInt, BaseIntBig, Allocator>;

    using base_int_type = BaseInt;
    using base_int_big_type = BaseIntBig;
    static constexpr base_int_type max_digit = std::numeric_limits<base_int_type>::max();
    static constexpr base_int_big_type radix = static_cast<base_int_big_type>(max_digit) + 1;
    static constexpr std::size_t base_int_type_size = sizeof(base_int_type);
    static constexpr bitcount_t base_int_type_bits = base_int_type_size * bits_in_byte;

    friend struct calc;
    friend struct std::hash<basic_Z>;

    friend class basic_Q<BaseInt, BaseIntBig, Allocator>;

    friend struct rand<basic_N_type>;
    friend struct rand<basic_Z>;

    friend struct format_output<basic_Z>;

   public:
    using basic_N_type::bit_reference, basic_N_type::const_bit_reference;
    using basic_N_type::ctz, basic_N_type::bits, basic_N_type::operator bool,
        basic_N_type::operator[];
    using basic_N_type::is_even, basic_N_type::is_odd;

    struct detail;

    basic_Z();
    explicit basic_Z(std::string_view num_str, unsigned base = default_base);
    basic_Z(std::integral auto num);

    basic_Z(const basic_N_type & n);
    basic_Z(basic_N_type && n);

    [[nodiscard]] const basic_N_type & abs() const &;
    [[nodiscard]] basic_N_type && abs() &&;

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

    basic_Z & operator++();
    basic_Z & operator--();

    basic_Z & operator+=(const basic_Z & rhs);
    basic_Z & operator-=(const basic_Z & rhs);
    basic_Z & operator*=(const basic_Z & rhs);

    basic_Z & operator&=(const basic_Z & rhs);
    basic_Z & operator|=(const basic_Z & rhs);
    basic_Z & operator^=(const basic_Z & rhs);

    [[nodiscard]] basic_Z operator-() const &;
    [[nodiscard]] basic_Z && operator-() &&;
    [[nodiscard]] basic_Z operator~() const;
    [[nodiscard]] basic_Z operator<<(bitcount_t pos) const;
    [[nodiscard]] basic_Z operator>>(bitcount_t pos) const;

    basic_Z & operator<<=(bitcount_t pos);
    basic_Z & operator>>=(bitcount_t pos);

    basic_Z & operator=(std::string_view num_str);
    basic_Z & operator=(std::integral auto rhs);

    basic_Z & operator=(const basic_N_type & n);
    basic_Z & operator=(basic_N_type && n);

   private:
    basic_Z(basic_N_type && n, sign_bool sign);
    basic_Z(const basic_N_type & n, sign_bool sign);

    [[nodiscard]] std::string conv_to_base_(unsigned base = default_base) const;

    [[nodiscard]] std::size_t dynamic_size_() const;
};

}  // namespace jmaths

namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct basic_Z<BaseInt, BaseIntBig, Allocator>::detail {
    static std::ostream & opr_ins(std::ostream & os, const basic_Z & z);
    static std::istream & opr_extr(std::istream & is, basic_Z & z);

    static basic_Z opr_add(const basic_Z & lhs, const basic_Z & rhs);
    static basic_Z opr_subtr(const basic_Z & lhs, const basic_Z & rhs);
    static basic_Z opr_mult(const basic_Z & lhs, const basic_Z & rhs);
    static std::pair<basic_Z, basic_Z> opr_div(const basic_Z & lhs, const basic_Z & rhs);

    static basic_Z opr_and(const basic_Z & lhs, const basic_Z & rhs);
    static basic_Z opr_or(const basic_Z & lhs, const basic_Z & rhs);
    static basic_Z opr_xor(const basic_Z & lhs, const basic_Z & rhs);

    static bool opr_eq(const basic_Z & lhs, const basic_Z & rhs);
    static bool opr_eq(const basic_Z & lhs, std::integral auto rhs);

    static std::strong_ordering opr_comp(const basic_Z & lhs, const basic_Z & rhs);
    static std::strong_ordering opr_comp(const basic_Z & lhs, std::integral auto rhs);
};

}  // namespace jmaths

namespace jmaths {

#if defined(UINT64_MAX) && defined(UINT32_MAX)

using Z = basic_Z<std::uint32_t, std::uint64_t>;

#elif defined(UINT32_MAX) && defined(UINT16_MAX)

using Z = basic_Z<std::uint16_t, std::uint32_t>;

#elif defined(UINT16_MAX) && defined(UINT8_MAX)

using Z = basic_Z<std::uint8_t, std::uint16_t>;

#endif

}  // namespace jmaths

template <jmaths::TMP::instance_of<jmaths::basic_Z> Z>
struct std::formatter<Z> : jmaths::format_output<Z> {};

#include "basic_Z_detail_impl.hpp"
#include "basic_Z_impl.hpp"
