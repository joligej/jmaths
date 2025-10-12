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
constexpr std::ostream & operator<<(std::ostream & os, const basic_Z_type & z);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::istream & operator>>(std::istream & is, basic_Z_type & z);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator+(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator-(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator*(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::pair<basic_Z_type, basic_Z_type> operator/(const basic_Z_type & lhs,
                                                          const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator&(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator|(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator^(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr bool operator==(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr bool operator==(const basic_Z_type & lhs, std::integral auto rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr bool operator==(std::integral auto lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::strong_ordering operator<=>(const basic_Z_type & lhs, const basic_Z_type & rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::strong_ordering operator<=>(const basic_Z_type & lhs, std::integral auto rhs);

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::strong_ordering operator<=>(std::integral auto lhs, const basic_Z_type & rhs);

template <typename BaseInt, typename BaseIntBig, typename Allocator = allocator<BaseInt>>
class basic_Z final : public sign_type, private basic_N<BaseInt, BaseIntBig, Allocator> {
    using basic_N_type = basic_N<BaseInt, BaseIntBig, Allocator>;
    using basic_Q_type = basic_Q<BaseInt, BaseIntBig, Allocator>;

    using base_int_type = BaseInt;
    using base_int_big_type = BaseIntBig;
    using allocator_type = Allocator;
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
    using basic_N_type::ctz, basic_N_type::bits, basic_N_type::operator bool,
        basic_N_type::operator[];
    using basic_N_type::is_even, basic_N_type::is_odd;
    using typename basic_N_type::bit_reference, typename basic_N_type::const_bit_reference;

    struct detail;

    constexpr basic_Z(const basic_Z &) = default;
    constexpr basic_Z(basic_Z &&) = default;
    constexpr basic_Z & operator=(const basic_Z &) = default;
    constexpr basic_Z & operator=(basic_Z &&) = default;

    constexpr basic_Z();
    explicit constexpr basic_Z(std::string_view num_str, unsigned base = default_base);
    constexpr basic_Z(std::integral auto num);

    explicit constexpr basic_Z(const basic_N_type & n);
    explicit constexpr basic_Z(basic_N_type && n);

    [[nodiscard]] constexpr const basic_N_type & abs() const &;
    [[nodiscard]] constexpr basic_N_type && abs() &&;

    [[nodiscard]] constexpr bool is_zero() const override;
    [[nodiscard]] constexpr bool is_one() const;
    [[nodiscard]] constexpr bool is_neg_one() const;

    [[nodiscard]] constexpr std::size_t size() const;  // size of this object in bytes

    [[nodiscard]] constexpr std::string to_str(
        unsigned base = default_base) const;  // convert to string in any base >= 2 and <= 64
    [[nodiscard]] constexpr std::string to_hex() const;  // convert to string in base 16 (assumes
                                                         // base is an integer power of 2)
    [[nodiscard]] constexpr std::string to_bin() const;  // convert to string in base 2

    template <std::unsigned_integral T> [[nodiscard]] constexpr std::optional<T> fits_into() const;
    template <std::signed_integral T> [[nodiscard]] constexpr std::optional<T> fits_into() const;

    constexpr void set_zero();

    constexpr basic_Z & operator++();
    constexpr basic_Z & operator--();

    constexpr basic_Z & operator+=(const basic_Z & rhs);
    constexpr basic_Z & operator-=(const basic_Z & rhs);
    constexpr basic_Z & operator*=(const basic_Z & rhs);

    constexpr basic_Z & operator&=(const basic_Z & rhs);
    constexpr basic_Z & operator|=(const basic_Z & rhs);
    constexpr basic_Z & operator^=(const basic_Z & rhs);

    [[nodiscard]] constexpr basic_Z operator-() const &;
    [[nodiscard]] constexpr basic_Z && operator-() &&;
    [[nodiscard]] constexpr basic_Z operator~() const;
    [[nodiscard]] constexpr basic_Z operator<<(bitcount_t pos) const;
    [[nodiscard]] constexpr basic_Z operator>>(bitcount_t pos) const;

    constexpr basic_Z & operator<<=(bitcount_t pos);
    constexpr basic_Z & operator>>=(bitcount_t pos);

    constexpr basic_Z & operator=(std::string_view num_str);
    constexpr basic_Z & operator=(std::integral auto rhs);

    constexpr basic_Z & operator=(const basic_N_type & n);
    constexpr basic_Z & operator=(basic_N_type && n);

   private:
    constexpr basic_Z(basic_N_type && n, sign_bool sign);
    constexpr basic_Z(const basic_N_type & n, sign_bool sign);

    [[nodiscard]] constexpr std::string conv_to_base_(unsigned base = default_base) const;

    [[nodiscard]] constexpr std::size_t dynamic_size_() const;
};

}  // namespace jmaths

namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct basic_Z<BaseInt, BaseIntBig, Allocator>::detail {
    static constexpr std::ostream & opr_ins(std::ostream & os, const basic_Z & z);
    static constexpr std::istream & opr_extr(std::istream & is, basic_Z & z);

    static constexpr basic_Z opr_add(const basic_Z & lhs, const basic_Z & rhs);
    static constexpr basic_Z opr_subtr(const basic_Z & lhs, const basic_Z & rhs);
    static constexpr basic_Z opr_mult(const basic_Z & lhs, const basic_Z & rhs);
    static constexpr std::pair<basic_Z, basic_Z> opr_div(const basic_Z & lhs, const basic_Z & rhs);

    static constexpr basic_Z opr_and(const basic_Z & lhs, const basic_Z & rhs);
    static constexpr basic_Z opr_or(const basic_Z & lhs, const basic_Z & rhs);
    static constexpr basic_Z opr_xor(const basic_Z & lhs, const basic_Z & rhs);

    static constexpr bool opr_eq(const basic_Z & lhs, const basic_Z & rhs);
    static constexpr bool opr_eq(const basic_Z & lhs, std::integral auto rhs);

    static constexpr std::strong_ordering opr_comp(const basic_Z & lhs, const basic_Z & rhs);
    static constexpr std::strong_ordering opr_comp(const basic_Z & lhs, std::integral auto rhs);
};

}  // namespace jmaths

namespace jmaths {

#if defined(UINT64_MAX) && defined(UINT32_MAX)

    #define JMATHS_TYPEDEF_Z 32
using Z = basic_Z<std::uint32_t, std::uint64_t>;

#elif defined(UINT32_MAX) && defined(UINT16_MAX)

    #define JMATHS_TYPEDEF_Z 16
using Z = basic_Z<std::uint16_t, std::uint32_t>;

#elif defined(UINT16_MAX) && defined(UINT8_MAX)

    #define JMATHS_TYPEDEF_Z 8
using Z = basic_Z<std::uint8_t, std::uint16_t>;

#endif

}  // namespace jmaths

template <jmaths::TMP::instance_of<jmaths::basic_Z> Z>
struct std::formatter<Z> : jmaths::format_output<Z> {};

#include "basic_Z_detail_impl.hpp"
#include "basic_Z_impl.hpp"
