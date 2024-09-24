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
#include <type_traits>
#include <utility>
#include <vector>

#include "TMP.hpp"
#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "formatter.hpp"

// declarations of N and associated functions and types
namespace jmaths {

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::ostream & operator<<(std::ostream & os, const basic_N_type & n);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::istream & operator>>(std::istream & is, basic_N_type & n);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator+(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator-(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator*(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::pair<basic_N_type, basic_N_type> operator/(const basic_N_type & lhs,
                                                          const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator&(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator|(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator^(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr bool operator==(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr bool operator==(const basic_N_type & lhs, std::integral auto rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr bool operator==(std::integral auto lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::strong_ordering operator<=>(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::strong_ordering operator<=>(const basic_N_type & lhs, std::integral auto rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::strong_ordering operator<=>(std::integral auto lhs, const basic_N_type & rhs);

template <typename BaseInt, typename BaseIntBig, typename Allocator = allocator<BaseInt>>
class basic_N {
    using basic_Z_type = basic_Z<BaseInt, BaseIntBig, Allocator>;
    using basic_Q_type = basic_Q<BaseInt, BaseIntBig, Allocator>;

    using base_int_type = BaseInt;
    using base_int_big_type = BaseIntBig;
    using allocator_type = Allocator;
    static constexpr base_int_type max_digit = std::numeric_limits<base_int_type>::max();
    static constexpr base_int_big_type radix = static_cast<base_int_big_type>(max_digit) + 1;
    static constexpr std::size_t base_int_type_size = sizeof(base_int_type);
    static constexpr bitcount_t base_int_type_bits = base_int_type_size * bits_in_byte;

    friend struct calc;
    friend struct std::hash<basic_N>;
    friend struct std::hash<basic_Z_type>;
    friend struct std::hash<basic_Q_type>;

    friend class basic_Q<BaseInt, BaseIntBig, Allocator>;

    friend struct rand<basic_N>;
    friend struct rand<basic_Z_type>;

    friend struct format_output<basic_N>;

   public:
    class bit_reference;
    class const_bit_reference;

    struct detail;

    constexpr basic_N(const basic_N &) = default;
    constexpr basic_N(basic_N &&) = default;
    constexpr basic_N & operator=(const basic_N &) = default;
    constexpr basic_N & operator=(basic_N &&) = default;

    constexpr basic_N();
    explicit constexpr basic_N(std::string_view num_str, unsigned base = default_base);
    constexpr basic_N(std::integral auto num);

    [[nodiscard]] constexpr bool is_zero() const;
    [[nodiscard]] constexpr bool is_one() const;

    [[nodiscard]] constexpr bool is_even() const;
    [[nodiscard]] constexpr bool is_odd() const;

    [[nodiscard]] constexpr bitcount_t ctz() const;   // count trailing zeroes
    [[nodiscard]] constexpr bitcount_t bits() const;  // count number of base 2 digits

    [[nodiscard]] constexpr std::size_t size() const;  // size of this object in bytes

    [[nodiscard]] constexpr std::string to_str(
        unsigned base = default_base) const;  // convert to string in any base >= 2 and <= 64
    [[nodiscard]] constexpr std::string to_hex() const;  // convert to string in base 16 (assumes
                                                         // base is an integer power of 2)
    [[nodiscard]] constexpr std::string to_bin() const;  // convert to string in base 2

    explicit constexpr operator bool() const;
    template <std::unsigned_integral T> [[nodiscard]] constexpr std::optional<T> fits_into() const;
    template <std::signed_integral T> [[nodiscard]] constexpr std::optional<T> fits_into() const;

    [[nodiscard]] constexpr bit_reference operator[](bitpos_t pos) &;
    [[nodiscard]] constexpr const_bit_reference operator[](bitpos_t pos) const &;

    constexpr void set_zero();

    constexpr basic_N & operator++();
    constexpr basic_N & operator--();

    constexpr basic_N & operator+=(const basic_N & rhs);
    constexpr basic_N & operator-=(const basic_N & rhs);
    constexpr basic_N & operator*=(const basic_N & rhs);

    constexpr basic_N & operator&=(const basic_N & rhs);
    constexpr basic_N & operator|=(const basic_N & rhs);
    constexpr basic_N & operator^=(const basic_N & rhs);

    [[nodiscard]] constexpr basic_N operator~() const;
    [[nodiscard]] constexpr basic_N operator<<(bitcount_t pos) const;
    [[nodiscard]] constexpr basic_N operator>>(bitcount_t pos) const;

    constexpr basic_N & operator<<=(bitcount_t pos);
    constexpr basic_N & operator>>=(bitcount_t pos);

    constexpr basic_N & operator=(std::string_view num_str);
    constexpr basic_N & operator=(std::integral auto rhs);

   protected:
    [[nodiscard]] constexpr std::size_t dynamic_size_() const;

    constexpr void opr_incr_();
    constexpr void opr_decr_();

    constexpr void opr_add_assign_(const basic_N & rhs);
    constexpr void opr_subtr_assign_(const basic_N & rhs);
    constexpr void opr_mult_assign_(const basic_N & rhs);

    constexpr void opr_and_assign_(const basic_N & rhs);
    constexpr void opr_or_assign_(const basic_N & rhs);
    constexpr void opr_xor_assign_(const basic_N & rhs);

    [[nodiscard]] constexpr basic_N opr_compl_() const;
    [[nodiscard]] constexpr basic_N opr_bitshift_l_(bitcount_t pos) const;
    [[nodiscard]] constexpr basic_N opr_bitshift_r_(bitcount_t pos) const;

    constexpr void opr_bitshift_l_assign_(bitcount_t pos);
    constexpr void opr_bitshift_r_assign_(bitcount_t pos);

    constexpr void opr_assign_(std::string_view num_str);
    constexpr void opr_assign_(std::integral auto rhs);

    [[nodiscard]] constexpr std::string conv_to_base_(unsigned base = default_base) const;

   private:
    std::vector<base_int_type, allocator_type> digits_;

    constexpr void remove_leading_zeroes_();
    [[nodiscard]] constexpr base_int_type front_() const;
    constexpr void handle_str_(std::string_view num_str, unsigned base);
    constexpr void handle_int_(std::integral auto num);

    template <std::unsigned_integral T>
    [[nodiscard]] constexpr T fit_into_(std::size_t max_byte) const;

    [[nodiscard]] constexpr bool bit_(bitpos_t pos) const;
    constexpr void bit_(bitpos_t pos, bool val);

    template <typename T> class bit_reference_base_;
    // because of error: type constraint differs in template redeclaration
    // otherwise: template <<TMP::decays_to<basic_N> T> class bit_reference_base_;

    static const basic_N one_;
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <TMP::decays_to<basic_N<BaseInt, BaseIntBig, Allocator>> T>
class basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference_base_<T> {
    // because of error: type constraint differs in template redeclaration
    // otherwise: class basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference_base_ {
   public:
    bit_reference_base_() = delete;
    bit_reference_base_(T &&, bitpos_t) = delete;
    constexpr bit_reference_base_(T & num, bitpos_t pos);

    constexpr operator bool() const;
    explicit constexpr operator int() const;

   protected:
    T & num_;
    const bitpos_t pos_;
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
class basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference : public bit_reference_base_<basic_N> {
   public:
    using bit_reference_base_<basic_N>::bit_reference_base_;

    constexpr bit_reference(const bit_reference & ref);

    constexpr bit_reference & operator=(bool val);
    constexpr bit_reference & operator=(const bit_reference & ref);
    constexpr bit_reference & operator=(const const_bit_reference & ref);
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
class basic_N<BaseInt, BaseIntBig, Allocator>::const_bit_reference :
    public bit_reference_base_<const basic_N> {
   public:
    using bit_reference_base_<const basic_N>::bit_reference_base_;

    constexpr const_bit_reference(const const_bit_reference & ref);
    constexpr const_bit_reference(const bit_reference & ref);

    auto & operator=(bool) = delete;
    auto & operator=(const bit_reference &) = delete;
    auto & operator=(const const_bit_reference &) = delete;
};

}  // namespace jmaths

namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct basic_N<BaseInt, BaseIntBig, Allocator>::detail {
    static constexpr std::ostream & opr_ins(std::ostream & os, const basic_N & n);
    static constexpr std::istream & opr_extr(std::istream & is, basic_N & n);

    static constexpr basic_N opr_add(const basic_N & lhs, const basic_N & rhs);
    static constexpr basic_N opr_subtr(basic_N lhs, const basic_N & rhs);
    static constexpr basic_N opr_mult(const basic_N & lhs, const basic_N & rhs);
    static constexpr std::pair<basic_N, basic_N> opr_div(const basic_N & lhs, const basic_N & rhs);

    static constexpr basic_N opr_and(const basic_N & lhs, const basic_N & rhs);
    static constexpr basic_N opr_or(const basic_N & lhs, const basic_N & rhs);
    static constexpr basic_N opr_xor(const basic_N & lhs, const basic_N & rhs);

    static constexpr bool opr_eq(const basic_N & lhs, const basic_N & rhs);
    static constexpr bool opr_eq(const basic_N & lhs, std::integral auto rhs);

    static constexpr std::strong_ordering opr_comp(const basic_N & lhs, const basic_N & rhs);
    static constexpr std::strong_ordering opr_comp(const basic_N & lhs, std::integral auto rhs);
};

}  // namespace jmaths

namespace jmaths {

#if defined(UINT64_MAX) && defined(UINT32_MAX)

    #define JMATHS_TYPEDEF_N 32
using N = basic_N<std::uint32_t, std::uint64_t>;

#elif defined(UINT32_MAX) && defined(UINT16_MAX)

    #define JMATHS_TYPEDEF_N 16
using N = basic_N<std::uint16_t, std::uint32_t>;

#elif defined(UINT16_MAX) && defined(UINT8_MAX)

    #define JMATHS_TYPEDEF_N 8
using N = basic_N<std::uint8_t, std::uint16_t>;

#endif

}  // namespace jmaths

template <jmaths::TMP::instance_of<jmaths::basic_N> N>
struct std::formatter<N> : jmaths::format_output<N> {};

#include "basic_N_detail_impl.hpp"
#include "basic_N_impl.hpp"
