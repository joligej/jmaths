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
#include <type_traits>
#include <utility>

#include "basic_N.hpp"
#include "basic_Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "error.hpp"
#include "sign_type.hpp"

// comparison functions for Z with integral types
namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_eq(const basic_Z & lhs,
                                                                       std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.template fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit == rhs;
    }

    return false;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::strong_ordering basic_Z<BaseInt, BaseIntBig, Allocator>::detail::opr_comp(
    const basic_Z & lhs,
    std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.template fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit <=> rhs;
    }

    return lhs.is_negative() ? std::strong_ordering::less : std::strong_ordering::greater;
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr bool operator==(const basic_Z_type & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_eq(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr bool operator==(std::integral auto lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_eq(rhs, lhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::strong_ordering operator<=>(const basic_Z_type & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_comp(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::strong_ordering operator<=>(std::integral auto lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return 0 <=> basic_Z_type::detail::opr_comp(rhs, lhs);
}

}  // namespace jmaths

// member function templates of Z
namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(std::integral auto num) :
    sign_type(&num), basic_N_type(num) {
    JMATHS_FUNCTION_TO_LOG;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <std::unsigned_integral T>
constexpr std::optional<T> basic_Z<BaseInt, BaseIntBig, Allocator>::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) { return std::nullopt; }

    return basic_N_type::template fits_into<T>();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <std::signed_integral T>
constexpr std::optional<T> basic_Z<BaseInt, BaseIntBig, Allocator>::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    static_assert(-1 == ~0, "Two's complement architecture required.");

    if (is_positive()) { return basic_N_type::template fits_into<T>(); }

    auto fits_into_unsigned = basic_N_type::template fits_into<std::make_unsigned_t<T>>();

    if (!fits_into_unsigned.has_value()) { return std::nullopt; }

    if (*fits_into_unsigned >
        static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::max()) + 1) {
        return std::nullopt;
    }

    return -*fits_into_unsigned;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator=(std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(sign_type::handle_int_(&rhs));
    basic_N_type::opr_assign_(rhs);
    return *this;
}

}  // namespace jmaths

// binary operators for Z
namespace jmaths {

/**********************************************************/
// forwarding functions

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::ostream & operator<<(std::ostream & os, const basic_Z_type & z) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_ins(os, z);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::istream & operator>>(std::istream & is, basic_Z_type & z) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_extr(is, z);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator+(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_add(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator-(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_subtr(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator*(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_mult(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::pair<basic_Z_type, basic_Z_type> operator/(const basic_Z_type & lhs,
                                                          const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(rhs);
    return basic_Z_type::detail::opr_div(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator&(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_and(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator|(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_or(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr basic_Z_type operator^(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_xor(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr bool operator==(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_eq(lhs, rhs);
}

template <TMP::instance_of<basic_Z> basic_Z_type>
constexpr std::strong_ordering operator<=>(const basic_Z_type & lhs, const basic_Z_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_Z_type::detail::opr_comp(lhs, rhs);
}

}  // namespace jmaths

// member functions of Z
namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(basic_N_type && n, sign_bool sign) :
    sign_type(sign), basic_N_type(std::move(n)) {
    JMATHS_FUNCTION_TO_LOG;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(const basic_N_type & n, sign_bool sign) :
    sign_type(sign), basic_N_type(n) {
    JMATHS_FUNCTION_TO_LOG;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Z<BaseInt, BaseIntBig, Allocator>::conv_to_base_(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) {
        return negative_sign + basic_N_type::conv_to_base_(base);
    } else {
        return basic_N_type::conv_to_base_(base);
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::size_t basic_Z<BaseInt, BaseIntBig, Allocator>::dynamic_size_() const {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::dynamic_size_();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z() = default;

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(std::string_view num_str, unsigned base) :
    sign_type(&num_str), basic_N_type(num_str, base) {
    JMATHS_FUNCTION_TO_LOG;

    if (basic_Z::is_zero()) { set_sign_(positive); }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(const basic_N_type & n) :
    basic_N_type(n) {
    JMATHS_FUNCTION_TO_LOG;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>::basic_Z(basic_N_type && n) :
    basic_N_type(std::move(n)) {
    JMATHS_FUNCTION_TO_LOG;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Z<BaseInt, BaseIntBig, Allocator>::abs() const & -> const basic_N_type & {
    JMATHS_FUNCTION_TO_LOG;

    return static_cast<const basic_N_type &>(*this);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_Z<BaseInt, BaseIntBig, Allocator>::abs() && -> basic_N_type && {
    JMATHS_FUNCTION_TO_LOG;

    return static_cast<basic_N_type &&>(*this);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Z<BaseInt, BaseIntBig, Allocator>::is_zero() const {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::is_zero();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Z<BaseInt, BaseIntBig, Allocator>::is_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return is_positive() && basic_N_type::is_one();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_Z<BaseInt, BaseIntBig, Allocator>::is_neg_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return is_negative() && basic_N_type::is_one();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::size_t basic_Z<BaseInt, BaseIntBig, Allocator>::size() const {
    JMATHS_FUNCTION_TO_LOG;

    return sizeof(*this) + dynamic_size_();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Z<BaseInt, BaseIntBig, Allocator>::to_str(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    error::invalid_base::check(base);

    return conv_to_base_(base);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Z<BaseInt, BaseIntBig, Allocator>::to_hex() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) {
        return negative_sign + basic_N_type::to_hex();
    } else {
        return basic_N_type::to_hex();
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_Z<BaseInt, BaseIntBig, Allocator>::to_bin() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) {
        return negative_sign + basic_N_type::to_bin();
    } else {
        return basic_N_type::to_bin();
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_Z<BaseInt, BaseIntBig, Allocator>::set_zero() {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::set_zero();
    set_sign_(positive);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator++() {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) {
        basic_N_type::opr_incr_();
    } else {
        basic_N_type::opr_decr_();

        if (basic_N_type::is_zero()) { set_sign_(positive); }
    }

    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator--() {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) {
        if (basic_N_type::is_zero()) {
            set_sign_(negative);
            basic_N_type::opr_incr_();
        } else {
            basic_N_type::opr_decr_();
        }
    } else {
        basic_N_type::opr_incr_();
    }

    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator+=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_positive()) {
        if (rhs.is_positive()) {
            basic_N_type::opr_add_assign_(rhs);
        } else {
            if (const auto difference = basic_N_type::detail::opr_comp(this->abs(), rhs.abs());
                difference == 0) {
                basic_N_type::set_zero();
            } else if (difference > 0) {
                basic_N_type::opr_subtr_assign_(rhs.abs());
            } else {
                basic_N_type::operator=(basic_N_type::detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(negative);
            }
        }
    } else {
        if (rhs.is_positive()) {
            if (const auto difference = basic_N_type::detail::opr_comp(this->abs(), rhs.abs());
                difference == 0) {
                basic_Z::set_zero();
            } else if (difference > 0) {
                basic_N_type::opr_subtr_assign_(rhs.abs());
            } else {
                basic_N_type::operator=(basic_N_type::detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(positive);
            }
        } else {
            basic_N_type::opr_add_assign_(rhs);
        }
    }

    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator-=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_positive()) {
        if (rhs.is_positive()) {
            if (const auto difference = basic_N_type::detail::opr_comp(this->abs(), rhs.abs());
                difference == 0) {
                basic_N_type::set_zero();
            } else if (difference > 0) {
                basic_N_type::opr_subtr_assign_(rhs.abs());
            } else {
                basic_N_type::operator=(basic_N_type::detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(negative);
            }
        } else {
            basic_N_type::opr_add_assign_(rhs);
        }
    } else {
        if (rhs.is_positive()) {
            basic_N_type::opr_add_assign_(rhs);
        } else {
            if (const auto difference = basic_N_type::detail::opr_comp(this->abs(), rhs.abs());
                difference == 0) {
                basic_Z::set_zero();
            } else if (difference > 0) {
                basic_N_type::opr_subtr_assign_(rhs.abs());
            } else {
                basic_N_type::operator=(basic_N_type::detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(positive);
            }
        }
    }

    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator*=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_mult_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator&=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_and_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ & rhs.sign_);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator|=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_or_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ | rhs.sign_);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator^=(const basic_Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_xor_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::operator-() const & {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return *this; }
    return {abs(), static_cast<sign_bool>(!sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &&
basic_Z<BaseInt, BaseIntBig, Allocator>::operator-() && {
    JMATHS_FUNCTION_TO_LOG;

    flip_sign();
    return std::move(*this);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::operator~() const {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type complemented = basic_N_type::opr_compl_();

    if (complemented.is_zero()) { return basic_Z{}; }

    return {std::move(complemented), static_cast<sign_bool>(!sign_)};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::operator<<(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    return {basic_N_type::opr_bitshift_l_(pos), sign_};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator>
basic_Z<BaseInt, BaseIntBig, Allocator>::operator>>(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type shifted = basic_N_type::opr_bitshift_r_(pos);

    if (shifted.is_zero()) { return basic_Z{}; }

    return {std::move(shifted), sign_};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator<<=(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_bitshift_l_assign_(pos);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator>>=(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::opr_bitshift_r_assign_(pos);

    if (basic_N_type::is_zero()) { set_sign_(positive); }

    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator=(std::string_view num_str) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(sign_type::handle_string_(&num_str));
    basic_N_type::opr_assign_(num_str);
    if (is_zero()) { set_sign_(positive); }
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator=(const basic_N_type & n) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::operator=(n);
    set_sign_(positive);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_Z<BaseInt, BaseIntBig, Allocator> &
basic_Z<BaseInt, BaseIntBig, Allocator>::operator=(basic_N_type && n) {
    JMATHS_FUNCTION_TO_LOG;

    basic_N_type::operator=(std::move(n));
    set_sign_(positive);
    return *this;
}

}  // namespace jmaths
