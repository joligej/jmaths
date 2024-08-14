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

#include "Z.hpp"

#include <compare>
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

#include "N.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "detail.hpp"
#include "sign_type.hpp"

namespace jmaths {

/**********************************************************/
// forwarding functions

std::ostream & operator<<(std::ostream & os, const Z & z) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_ins(os, z);
}

std::istream & operator>>(std::istream & is, Z & z) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_extr(is, z);
}

Z operator+(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_add(lhs, rhs);
}

Z operator-(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_subtr(lhs, rhs);
}

Z operator*(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_mult(lhs, rhs);
}

std::pair<Z, Z> operator/(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_div(lhs, rhs);
}

Z operator&(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_and(lhs, rhs);
}

Z operator|(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_or(lhs, rhs);
}

Z operator^(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_xor(lhs, rhs);
}

bool operator==(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_eq(lhs, rhs);
}

std::strong_ordering operator<=>(const Z & lhs, const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_comp(lhs, rhs);
}

}  // namespace jmaths

namespace jmaths {

Z::Z(N && n, sign_bool sign) : sign_type(sign), N(std::move(n)) {
    JMATHS_FUNCTION_TO_LOG;
}

Z::Z(const N & n, sign_bool sign) : sign_type(sign), N(n) {
    JMATHS_FUNCTION_TO_LOG;
}

std::size_t Z::dynamic_size_() const {
    JMATHS_FUNCTION_TO_LOG;

    return N::dynamic_size_();
}

Z::Z() = default;

Z::Z(std::string_view num_str, unsigned base) : sign_type(&num_str), N(num_str, base) {
    JMATHS_FUNCTION_TO_LOG;

    if (Z::is_zero()) { set_sign_(positive); }
}

Z::Z(const N & n) : N(n) {
    JMATHS_FUNCTION_TO_LOG;
}

Z::Z(N && n) : N(std::move(n)) {
    JMATHS_FUNCTION_TO_LOG;
}

const N & Z::abs() const & {
    JMATHS_FUNCTION_TO_LOG;

    return static_cast<const N &>(*this);
}

N && Z::abs() && {
    JMATHS_FUNCTION_TO_LOG;

    return static_cast<N &&>(*this);
}

bool Z::is_zero() const {
    JMATHS_FUNCTION_TO_LOG;

    return N::is_zero();
}

bool Z::is_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return is_positive() && N::is_one();
}

bool Z::is_neg_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return is_negative() && N::is_one();
}

std::size_t Z::size() const {
    JMATHS_FUNCTION_TO_LOG;

    return sizeof(*this) + dynamic_size_();
}

std::string Z::to_str(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) {
        return negative_sign + N::to_str(base);
    } else {
        return N::to_str(base);
    }
}

std::string Z::to_hex() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_negative()) {
        return negative_sign + N::to_hex();
    } else {
        return N::to_hex();
    }
}

void Z::set_zero() {
    JMATHS_FUNCTION_TO_LOG;

    N::set_zero();
    set_sign_(positive);
}

Z & Z::operator++() {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) {
        N::opr_incr_();
    } else {
        N::opr_decr_();

        if (N::is_zero()) { set_sign_(positive); }
    }

    return *this;
}

Z & Z::operator--() {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) {
        if (N::is_zero()) {
            set_sign_(negative);
            N::opr_incr_();
        } else {
            N::opr_decr_();
        }
    } else {
        N::opr_incr_();
    }

    return *this;
}

Z & Z::operator+=(const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_positive()) {
        if (rhs.is_positive()) {
            N::opr_add_assign_(rhs);
        } else {
            if (const auto difference = detail::opr_comp(this->abs(), rhs.abs()); difference == 0) {
                N::set_zero();
            } else if (difference > 0) {
                N::opr_subtr_assign_(rhs.abs());
            } else {
                N::operator=(detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(negative);
            }
        }
    } else {
        if (rhs.is_positive()) {
            if (const auto difference = detail::opr_comp(this->abs(), rhs.abs()); difference == 0) {
                Z::set_zero();
            } else if (difference > 0) {
                N::opr_subtr_assign_(rhs.abs());
            } else {
                N::operator=(detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(positive);
            }
        } else {
            N::opr_add_assign_(rhs);
        }
    }

    return *this;
}

Z & Z::operator-=(const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_positive()) {
        if (rhs.is_positive()) {
            if (const auto difference = detail::opr_comp(this->abs(), rhs.abs()); difference == 0) {
                N::set_zero();
            } else if (difference > 0) {
                N::opr_subtr_assign_(rhs.abs());
            } else {
                N::operator=(detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(negative);
            }
        } else {
            N::opr_add_assign_(rhs);
        }
    } else {
        if (rhs.is_positive()) {
            N::opr_add_assign_(rhs);
        } else {
            if (const auto difference = detail::opr_comp(this->abs(), rhs.abs()); difference == 0) {
                Z::set_zero();
            } else if (difference > 0) {
                N::opr_subtr_assign_(rhs.abs());
            } else {
                N::operator=(detail::opr_subtr(rhs.abs(), this->abs()));
                set_sign_(positive);
            }
        }
    }

    return *this;
}

Z & Z::operator*=(const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    N::opr_mult_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
    return *this;
}

Z & Z::operator&=(const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    N::opr_and_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ & rhs.sign_);
    return *this;
}

Z & Z::operator|=(const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    N::opr_or_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ | rhs.sign_);
    return *this;
}

Z & Z::operator^=(const Z & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    N::opr_xor_assign_(rhs);
    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
    return *this;
}

Z Z::operator-() const & {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return *this; }
    return {abs(), static_cast<sign_bool>(!sign_)};
}

Z && Z::operator-() && {
    JMATHS_FUNCTION_TO_LOG;

    flip_sign();
    return std::move(*this);
}

Z Z::operator~() const {
    JMATHS_FUNCTION_TO_LOG;

    N complemented = N::opr_compl_();

    if (complemented.is_zero()) { return Z{}; }

    return {std::move(complemented), static_cast<sign_bool>(!sign_)};
}

Z Z::operator<<(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    return {N::opr_bitshift_l_(pos), sign_};
}

Z Z::operator>>(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    N shifted = N::opr_bitshift_r_(pos);

    if (shifted.is_zero()) { return Z{}; }

    return {std::move(shifted), sign_};
}

Z & Z::operator<<=(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    N::opr_bitshift_l_assign_(pos);
    return *this;
}

Z & Z::operator>>=(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    N::opr_bitshift_r_assign_(pos);

    if (N::is_zero()) { set_sign_(positive); }

    return *this;
}

Z & Z::operator=(std::string_view num_str) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(sign_type::handle_string_(&num_str));
    N::opr_assign_(num_str);
    if (is_zero()) { set_sign_(positive); }
    return *this;
}

Z & Z::operator=(const N & n) {
    JMATHS_FUNCTION_TO_LOG;

    N::operator=(n);
    set_sign_(positive);
    return *this;
}

Z & Z::operator=(N && n) {
    JMATHS_FUNCTION_TO_LOG;

    N::operator=(std::move(n));
    set_sign_(positive);
    return *this;
}

}  // namespace jmaths
