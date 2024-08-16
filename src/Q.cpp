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

#include "Q.hpp"

#include <algorithm>
#include <compare>
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include "N.hpp"
#include "Z.hpp"
#include "calc.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "detail.hpp"
#include "error.hpp"
#include "sign_type.hpp"

namespace jmaths {
/**********************************************************/
// forwarding functions

std::ostream & operator<<(std::ostream & os, const Q & q) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_ins(os, q);
}

std::istream & operator>>(std::istream & is, Q & q) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_extr(is, q);
}

Q operator+(const Q & lhs, const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_add(lhs, rhs);
}

Q operator-(const Q & lhs, const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_subtr(lhs, rhs);
}

Q operator*(const Q & lhs, const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_mult(lhs, rhs);
}

Q operator/(const Q & lhs, const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (rhs.is_zero()) { throw error::division_by_zero{}; }
    return detail::opr_div(lhs, rhs);
}

Q operator&(const Q & lhs, const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_and(lhs, rhs);
}

Q operator|(const Q & lhs, const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_or(lhs, rhs);
}

Q operator^(const Q & lhs, const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_xor(lhs, rhs);
}

bool operator==(const Q & lhs, const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_eq(lhs, rhs);
}

std::strong_ordering operator<=>(const Q & lhs, const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_comp(lhs, rhs);
}

}  // namespace jmaths

namespace jmaths {

Q::Q(N && num, N && denom, sign_bool sign) :
    sign_type(sign), num_(std::move(num)), denom_(std::move(denom)) {
    JMATHS_FUNCTION_TO_LOG;

    canonicalise_();
}

Q::Q(const N & num, const N & denom, sign_bool sign) : sign_type(sign), num_(num), denom_(denom) {
    JMATHS_FUNCTION_TO_LOG;

    canonicalise_();
}

Q::Q(std::tuple<N, N, sign_bool> && fraction_info) :
    Q(std::move(std::get<0>(fraction_info)),
      std::move(std::get<1>(fraction_info)),
      std::get<2>(fraction_info)) {
    JMATHS_FUNCTION_TO_LOG;
}

std::string_view Q::handle_fraction_string_(std::string_view * num_str) {
    JMATHS_FUNCTION_TO_LOG;

    const auto fraction_bar = num_str->find(vinculum);
    const std::string_view numerator = num_str->substr(0U, fraction_bar);
    num_str->remove_prefix(fraction_bar + 1U);
    return numerator;
}

void Q::canonicalise_() {
    JMATHS_FUNCTION_TO_LOG;

    const N gcd = calc::gcd(num_, denom_);
    num_ = detail::opr_div(num_, gcd).first;
    denom_ = detail::opr_div(denom_, gcd).first;
}

std::string Q::conv_to_base_(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) { return num_.conv_to_base_(base) + vinculum + denom_.conv_to_base_(base); }

    return negative_sign + num_.conv_to_base_(base) + vinculum + denom_.conv_to_base_(base);
}

std::size_t Q::dynamic_size_() const {
    JMATHS_FUNCTION_TO_LOG;

    return num_.dynamic_size_() + denom_.dynamic_size_();
}

Q::Q() : denom_(N::one_) {
    JMATHS_FUNCTION_TO_LOG;
}

Q::Q(std::string_view num_str, unsigned base) :
    sign_type(&num_str), num_(handle_fraction_string_(&num_str), base), denom_(num_str, base) {
    JMATHS_FUNCTION_TO_LOG;

    if (denom_.is_zero()) { throw error::division_by_zero("Denominator cannot be zero!"); }

    canonicalise_();
    if (Q::is_zero()) { set_sign_(positive); }
}

Q::Q(const N & n) : num_(n), denom_(N::one_) {
    JMATHS_FUNCTION_TO_LOG;
}

Q::Q(N && n) : num_(std::move(n)), denom_(N::one_) {
    JMATHS_FUNCTION_TO_LOG;
}

Q::Q(const Z & z) : sign_type(z.sign_), num_(z.abs()), denom_(N::one_) {
    JMATHS_FUNCTION_TO_LOG;
}

Q::Q(Z && z) : sign_type(z.sign_), num_(std::move(std::move(z).abs())), denom_(N::one_) {
    JMATHS_FUNCTION_TO_LOG;
}

Q::Q(const N & num, const N & denom) : num_(num), denom_(denom) {
    JMATHS_FUNCTION_TO_LOG;

    if (denom_.is_zero()) { throw error::division_by_zero("Denominator cannot be zero!"); }

    canonicalise_();
}

Q::Q(const N & num, N && denom) : num_(num), denom_(std::move(denom)) {
    JMATHS_FUNCTION_TO_LOG;

    if (denom_.is_zero()) { throw error::division_by_zero("Denominator cannot be zero!"); }

    canonicalise_();
}

Q::Q(N && num, const N & denom) : num_(std::move(num)), denom_(denom) {
    JMATHS_FUNCTION_TO_LOG;

    if (denom_.is_zero()) { throw error::division_by_zero("Denominator cannot be zero!"); }

    canonicalise_();
}

Q::Q(N && num, N && denom) : num_(std::move(num)), denom_(std::move(denom)) {
    JMATHS_FUNCTION_TO_LOG;

    if (denom_.is_zero()) { throw error::division_by_zero("Denominator cannot be zero!"); }

    canonicalise_();
}

Q::Q(const Z & num, const Z & denom) :
    sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)),
    num_(num.abs()),
    denom_(denom.abs()) {
    JMATHS_FUNCTION_TO_LOG;

    if (denom_.is_zero()) { throw error::division_by_zero("Denominator cannot be zero!"); }

    canonicalise_();
}

Q::Q(const Z & num, Z && denom) :
    sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)),
    num_(num.abs()),
    denom_(std::move(std::move(denom).abs())) {
    JMATHS_FUNCTION_TO_LOG;

    if (denom_.is_zero()) { throw error::division_by_zero("Denominator cannot be zero!"); }

    canonicalise_();
}

Q::Q(Z && num, const Z & denom) :
    sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)),
    num_(std::move(std::move(num).abs())),
    denom_(denom.abs()) {
    JMATHS_FUNCTION_TO_LOG;

    if (denom_.is_zero()) { throw error::division_by_zero("Denominator cannot be zero!"); }

    canonicalise_();
}

Q::Q(Z && num, Z && denom) :
    sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)),
    num_(std::move(std::move(num).abs())),
    denom_(std::move(std::move(denom).abs())) {
    JMATHS_FUNCTION_TO_LOG;

    if (denom_.is_zero()) { throw error::division_by_zero("Denominator cannot be zero!"); }

    canonicalise_();
}

bool Q::is_zero() const {
    JMATHS_FUNCTION_TO_LOG;

    return num_.is_zero();
}

bool Q::is_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return is_positive() && num_.is_one() && denom_.is_one();
}

bool Q::is_neg_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return is_negative() && num_.is_one() && denom_.is_one();
}

Q Q::abs() const & {
    JMATHS_FUNCTION_TO_LOG;

    return {num_, denom_, positive};
}

Q && Q::abs() && {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(positive);
    return std::move(*this);
}

Q Q::inverse() const & {
    JMATHS_FUNCTION_TO_LOG;

    if (num_.is_zero()) { throw error::division_by_zero("Cannot take the inverse of zero!"); }

    return {denom_, num_, sign_};
}

Q && Q::inverse() && {
    JMATHS_FUNCTION_TO_LOG;

    if (num_.is_zero()) { throw error::division_by_zero("Denominator cannot be zero!"); }

    std::swap(num_, denom_);
    return std::move(*this);
}

std::size_t Q::size() const {
    JMATHS_FUNCTION_TO_LOG;

    return sizeof(*this) + dynamic_size_();
}

std::string Q::to_str(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    error::invalid_base::check(base);

    return conv_to_base_(base);
}

std::string Q::to_hex() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) { return num_.to_hex() + vinculum + denom_.to_hex(); }

    return negative_sign + num_.to_hex() + vinculum + denom_.to_hex();
}

Q::operator bool() const {
    JMATHS_FUNCTION_TO_LOG;

    return !is_zero();
}

Q & Q::operator++() {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) {
        num_.opr_add_assign_(denom_);
    } else {
        if (const auto difference = detail::opr_comp(num_, denom_); difference == 0) {
            num_.set_zero();
            set_sign_(positive);
        } else if (difference > 0) {
            num_.opr_subtr_assign_(denom_);
        } else {
            num_ = detail::opr_subtr(denom_, num_);
            set_sign_(positive);
        }
    }

    return *this;
}

Q & Q::operator--() {
    JMATHS_FUNCTION_TO_LOG;

    if (is_positive()) {
        if (const auto difference = detail::opr_comp(num_, denom_); difference == 0) {
            num_.set_zero();
        } else if (difference > 0) {
            num_.opr_subtr_assign_(denom_);
        } else {
            num_ = detail::opr_subtr(denom_, num_);
            set_sign_(negative);
        }

    } else {
        num_.opr_add_assign_(denom_);
    }

    return *this;
}

Q & Q::operator+=(const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_positive()) {
        if (rhs.is_positive()) {
            N first_product = detail::opr_mult(num_, rhs.denom_);
            first_product.opr_add_assign_(detail::opr_mult(denom_, rhs.num_));
            num_ = std::move(first_product);
        } else {
            N first_product = detail::opr_mult(num_, rhs.denom_);
            N second_product = detail::opr_mult(denom_, rhs.num_);

            if (const auto difference = detail::opr_comp(first_product, second_product);
                difference == 0) {
                num_.set_zero();
                denom_ = N::one_;
                return *this;
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                num_ = std::move(first_product);
            } else {
                second_product.opr_subtr_assign_(first_product);
                num_ = std::move(second_product);
                set_sign_(negative);
            }
        }
    } else {
        if (rhs.is_positive()) {
            N first_product = detail::opr_mult(num_, rhs.denom_);
            N second_product = detail::opr_mult(denom_, rhs.num_);

            if (const auto difference = detail::opr_comp(first_product, second_product);
                difference == 0) {
                num_.set_zero();
                denom_ = N::one_;
                set_sign_(positive);
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                num_ = std::move(first_product);
            } else {
                second_product.opr_subtr_assign_(first_product);
                num_ = std::move(second_product);
                set_sign_(positive);
            }
        } else {
            N first_product = detail::opr_mult(num_, rhs.denom_);
            first_product.opr_add_assign_(detail::opr_mult(denom_, rhs.num_));
            num_ = std::move(first_product);
        }
    }

    denom_.opr_mult_assign_(rhs.denom_);

    canonicalise_();

    return *this;
}

Q & Q::operator-=(const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_positive()) {
        if (rhs.is_positive()) {
            N first_product = detail::opr_mult(num_, rhs.denom_);
            N second_product = detail::opr_mult(denom_, rhs.num_);

            if (const auto difference = detail::opr_comp(first_product, second_product);
                difference == 0) {
                num_.set_zero();
                denom_ = N::one_;
                return *this;
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                num_ = std::move(first_product);
            } else {
                second_product.opr_subtr_assign_(first_product);
                num_ = std::move(second_product);
                set_sign_(negative);
            }
        } else {
            N first_product = detail::opr_mult(num_, rhs.denom_);
            first_product.opr_add_assign_(detail::opr_mult(denom_, rhs.num_));
            num_ = std::move(first_product);
        }
    } else {
        if (rhs.is_positive()) {
            N first_product = detail::opr_mult(num_, rhs.denom_);
            first_product.opr_add_assign_(detail::opr_mult(denom_, rhs.num_));
            num_ = std::move(first_product);
        } else {
            N first_product = detail::opr_mult(num_, rhs.denom_);
            N second_product = detail::opr_mult(denom_, rhs.num_);

            if (const auto difference = detail::opr_comp(first_product, second_product);
                difference == 0) {
                num_.set_zero();
                denom_ = N::one_;
                set_sign_(positive);
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                num_ = std::move(first_product);
            } else {
                second_product.opr_subtr_assign_(first_product);
                num_ = std::move(second_product);
                set_sign_(positive);
            }
        }
    }

    denom_.opr_mult_assign_(rhs.denom_);

    canonicalise_();

    return *this;
}

Q & Q::operator*=(const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    num_.opr_mult_assign_(rhs.num_);
    denom_.opr_mult_assign_(rhs.denom_);

    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);

    canonicalise_();

    return *this;
}

Q & Q::operator/=(const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (rhs.is_zero()) { throw error::division_by_zero{}; }

    num_.opr_mult_assign_(rhs.denom_);
    denom_.opr_mult_assign_(rhs.num_);

    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);

    canonicalise_();

    return *this;
}

Q & Q::operator&=(const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    denom_.opr_and_assign_(rhs.denom_);

    if (denom_.is_zero()) { throw error::division_by_zero{}; }

    num_.opr_and_assign_(rhs.num_);

    set_sign_(is_zero() ? positive : this->sign_ & rhs.sign_);

    canonicalise_();

    return *this;
}

Q & Q::operator|=(const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    num_.opr_or_assign_(rhs.num_);
    denom_.opr_or_assign_(rhs.denom_);

    set_sign_(is_zero() ? positive : this->sign_ | rhs.sign_);

    canonicalise_();

    return *this;
}

Q & Q::operator^=(const Q & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    denom_.opr_xor_assign_(rhs.denom_);

    if (denom_.is_zero()) { throw error::division_by_zero{}; }

    num_.opr_xor_assign_(rhs.num_);

    set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);

    canonicalise_();

    return *this;
}

Q Q::operator-() const & {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return *this; }
    return {num_, denom_, static_cast<sign_bool>(!sign_)};
}

Q && Q::operator-() && {
    JMATHS_FUNCTION_TO_LOG;

    flip_sign();
    return std::move(*this);
}

Q Q::operator~() const {
    JMATHS_FUNCTION_TO_LOG;

    N num_complemented = num_.opr_compl_();

    if (num_complemented.is_zero()) { return Q{}; }

    N denom_complemented = denom_.opr_compl_();

    if (denom_complemented.is_zero()) {
        throw error::division_by_zero("Denominator of complemented fraction cannot be zero!");
    }

    return {std::move(num_complemented),
            std::move(denom_complemented),
            static_cast<sign_bool>(!sign_)};
}

Q Q::operator<<(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    return {num_.opr_bitshift_l_(pos), denom_, sign_};
}

Q Q::operator>>(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return Q{}; }
    return {num_, denom_.opr_bitshift_l_(pos), sign_};
}

Q & Q::operator<<=(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    num_.opr_bitshift_l_assign_(pos);
    canonicalise_();
    return *this;
}

Q & Q::operator>>=(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return *this; }
    denom_.opr_bitshift_l_assign_(pos);
    canonicalise_();
    return *this;
}

Q & Q::operator=(std::string_view num_str) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(sign_type::handle_string_(&num_str));

    num_.opr_assign_(handle_fraction_string_(&num_str));
    denom_.opr_assign_(num_str);

    canonicalise_();

    if (is_zero()) { set_sign_(positive); }

    return *this;
}

Q & Q::operator=(const N & n) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(positive);
    num_ = n;
    denom_ = N::one_;
    return *this;
}

Q & Q::operator=(N && n) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(positive);
    num_ = std::move(n);
    denom_ = N::one_;
    return *this;
}

Q & Q::operator=(const Z & z) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(z.sign_);
    num_ = z.abs();
    denom_ = N::one_;
    return *this;
}

Q & Q::operator=(Z && z) {
    JMATHS_FUNCTION_TO_LOG;

    set_sign_(z.sign_);
    num_ = std::move(std::move(z).abs());
    denom_ = N::one_;
    return *this;
}

}  // namespace jmaths
