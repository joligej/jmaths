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
std::ostream & operator<<(std::ostream & os, const basic_N_type & n);

template <TMP::instance_of<basic_N> basic_N_type>
std::istream & operator>>(std::istream & is, basic_N_type & n);

template <TMP::instance_of<basic_N> basic_N_type>
basic_N_type operator+(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
basic_N_type operator-(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
basic_N_type operator*(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
std::pair<basic_N_type, basic_N_type> operator/(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
basic_N_type operator&(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
basic_N_type operator|(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
basic_N_type operator^(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
bool operator==(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
bool operator==(const basic_N_type & lhs, std::integral auto rhs);

template <TMP::instance_of<basic_N> basic_N_type>
bool operator==(std::integral auto lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
std::strong_ordering operator<=>(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
std::strong_ordering operator<=>(const basic_N_type & lhs, std::integral auto rhs);

template <TMP::instance_of<basic_N> basic_N_type>
std::strong_ordering operator<=>(std::integral auto lhs, const basic_N_type & rhs);

template <typename BaseInt, typename BaseIntBig, typename Allocator = allocator<BaseInt>>
class basic_N {
    using basic_Z_type = basic_Z<BaseInt, BaseIntBig, Allocator>;
    using basic_Q_type = basic_Q<BaseInt, BaseIntBig, Allocator>;

    using base_int_type = BaseInt;
    using base_int_big_type = BaseIntBig;
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

    basic_N();
    explicit basic_N(std::string_view num_str, unsigned base = default_base);
    basic_N(std::integral auto num);

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

    basic_N & operator++();
    basic_N & operator--();

    basic_N & operator+=(const basic_N & rhs);
    basic_N & operator-=(const basic_N & rhs);
    basic_N & operator*=(const basic_N & rhs);

    basic_N & operator&=(const basic_N & rhs);
    basic_N & operator|=(const basic_N & rhs);
    basic_N & operator^=(const basic_N & rhs);

    [[nodiscard]] basic_N operator~() const;
    [[nodiscard]] basic_N operator<<(bitcount_t pos) const;
    [[nodiscard]] basic_N operator>>(bitcount_t pos) const;

    basic_N & operator<<=(bitcount_t pos);
    basic_N & operator>>=(bitcount_t pos);

    basic_N & operator=(std::string_view num_str);
    basic_N & operator=(std::integral auto rhs);

   protected:
    [[nodiscard]] std::size_t dynamic_size_() const;

    void opr_incr_();
    void opr_decr_();

    void opr_add_assign_(const basic_N & rhs);
    void opr_subtr_assign_(const basic_N & rhs);
    void opr_mult_assign_(const basic_N & rhs);

    void opr_and_assign_(const basic_N & rhs);
    void opr_or_assign_(const basic_N & rhs);
    void opr_xor_assign_(const basic_N & rhs);

    [[nodiscard]] basic_N opr_compl_() const;
    [[nodiscard]] basic_N opr_bitshift_l_(bitcount_t pos) const;
    [[nodiscard]] basic_N opr_bitshift_r_(bitcount_t pos) const;

    void opr_bitshift_l_assign_(bitcount_t pos);
    void opr_bitshift_r_assign_(bitcount_t pos);

    void opr_assign_(std::string_view num_str);
    void opr_assign_(std::integral auto rhs);

    [[nodiscard]] std::string conv_to_base_(unsigned base = default_base) const;

   private:
    std::vector<base_int_type, allocator<base_int_type>> digits_;

    void remove_leading_zeroes_();
    [[nodiscard]] base_int_type front_() const;
    void handle_str_(std::string_view num_str, unsigned base);
    void handle_int_(std::integral auto num);

    template <std::unsigned_integral T> [[nodiscard]] T fit_into_(std::size_t max_byte) const;

    [[nodiscard]] bool bit_(bitpos_t pos) const;
    void bit_(bitpos_t pos, bool val);

    template <TMP::decays_to<basic_N> T> class bit_reference_base_;

    static const basic_N one_;
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <TMP::decays_to<basic_N<BaseInt, BaseIntBig, Allocator>> T>
class basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference_base_ {
    friend class bit_reference;
    friend class const_bit_reference;

   public:
    bit_reference_base_() = delete;
    bit_reference_base_(T &&, bitpos_t) = delete;
    bit_reference_base_(T & num, bitpos_t pos);

    operator bool() const;
    explicit operator int() const;

   public:
    T & num_;
    const bitpos_t pos_;
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
class basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference : public bit_reference_base_<basic_N> {
   public:
    using bit_reference_base_<basic_N>::bit_reference_base_;

    bit_reference(const bit_reference & ref);

    bit_reference & operator=(bool val);
    bit_reference & operator=(const bit_reference & ref);
    bit_reference & operator=(const const_bit_reference & ref);
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
class basic_N<BaseInt, BaseIntBig, Allocator>::const_bit_reference :
    public bit_reference_base_<const basic_N> {
   public:
    using bit_reference_base_<const basic_N>::bit_reference_base_;

    const_bit_reference(const const_bit_reference & ref);
    const_bit_reference(const bit_reference & ref);

    auto & operator=(bool) = delete;
    auto & operator=(const bit_reference &) = delete;
    auto & operator=(const const_bit_reference &) = delete;
};

}  // namespace jmaths

namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct basic_N<BaseInt, BaseIntBig, Allocator>::detail {
    static std::ostream & opr_ins(std::ostream & os, const basic_N & n);
    static std::istream & opr_extr(std::istream & is, basic_N & n);

    static basic_N opr_add(const basic_N & lhs, const basic_N & rhs);
    static basic_N opr_subtr(basic_N lhs, const basic_N & rhs);
    static basic_N opr_mult(const basic_N & lhs, const basic_N & rhs);
    static std::pair<basic_N, basic_N> opr_div(const basic_N & lhs, const basic_N & rhs);

    static basic_N opr_and(const basic_N & lhs, const basic_N & rhs);
    static basic_N opr_or(const basic_N & lhs, const basic_N & rhs);
    static basic_N opr_xor(const basic_N & lhs, const basic_N & rhs);

    static bool opr_eq(const basic_N & lhs, const basic_N & rhs);
    static bool opr_eq(const basic_N & lhs, std::integral auto rhs);

    static std::strong_ordering opr_comp(const basic_N & lhs, const basic_N & rhs);
    static std::strong_ordering opr_comp(const basic_N & lhs, std::integral auto rhs);
};

}  // namespace jmaths

template <jmaths::TMP::instance_of<jmaths::basic_N> N>
struct std::formatter<N> : jmaths::format_output<N> {};

#include "basic_N_detail_impl.hpp"
#include "basic_N_impl.hpp"
