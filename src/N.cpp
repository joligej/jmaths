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

#include "N.hpp"

#include <algorithm>
#include <bit>
#include <cassert>
#include <compare>
#include <cstddef>
#include <ios>
#include <istream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

#include "constants_and_types.hpp"
#include "def.hh"
#include "detail.hpp"
#include "error.hpp"

namespace jmaths {

namespace {

base_int base_converter(char c) noexcept;  // convert char to number for base >= 2 and <= 64

base_int base_converter(char c) noexcept {
    FUNCTION_TO_LOG;

    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'Z') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'z') return (c - 'a' + 10 + 26);
    if (c == '+') return 62;

    assert(c == '/');  // it is assumed that c == '/' because no other character
                       // would be valid or logical

    return 63;
}

}  // namespace

}  // namespace jmaths

namespace jmaths {

/**********************************************************/
// forwarding functions

std::ostream & operator<<(std::ostream & os, const N & n) {
    FUNCTION_TO_LOG;

    return detail::opr_ins(os, n);
}

std::istream & operator>>(std::istream & is, N & n) {
    FUNCTION_TO_LOG;

    return detail::opr_extr(is, n);
}

N operator+(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_add(lhs, rhs);
}

N operator-(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    const auto difference = detail::opr_comp(lhs, rhs);

    if (difference == 0) return N{};
    return (difference > 0 ? detail::opr_subtr(lhs, rhs) : detail::opr_subtr(rhs, lhs));
}

N operator*(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_mult(lhs, rhs);
}

std::pair<N, N> operator/(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    if (rhs.is_zero()) throw error::division_by_zero{};
    return detail::opr_div(lhs, rhs);
}

N operator&(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_and(lhs, rhs);
}

N operator|(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_or(lhs, rhs);
}

N operator^(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_xor(lhs, rhs);
}

bool operator==(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_eq(lhs, rhs);
}

std::strong_ordering operator<=>(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_comp(lhs, rhs);
}

}  // namespace jmaths

namespace jmaths {

void N::remove_leading_zeroes_() {
    FUNCTION_TO_LOG;

    while (!digits_.empty()) {
        if (digits_.back() != 0) break;
        digits_.pop_back();
    }

    assert(digits_.empty() || digits_.back() != 0);
}

base_int N::front_() const {
    FUNCTION_TO_LOG;

    if (digits_.empty()) return 0;

    return digits_.front();
}

std::string N::conv_to_base_(unsigned base) const {
    FUNCTION_TO_LOG;

    static constexpr char base_converter[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

    if (is_zero()) return "0";

    std::string num_str;

    const std::size_t digits_needed = ((double)base_int_bits / base) * digits_.size() + 1;

    num_str.reserve(digits_needed);

    N helper(*this);

    while (!helper.is_zero()) {
        auto [quotient, remainder] = detail::opr_div(helper, base);
        num_str.push_back(base_converter[remainder.front_()]);
        helper = std::move(quotient);
    }

    num_str.shrink_to_fit();

    std::ranges::reverse(num_str);

    return num_str;
}

void N::handle_str_(std::string_view num_str, unsigned base) {
    FUNCTION_TO_LOG;

    if (num_str.empty() || (num_str.size() == 1 && num_str.front() == '0')) return;

    for (auto cit = num_str.cbegin(); &*cit != &num_str.back(); ++cit) {
        opr_add_assign_(base_converter(*cit));
        opr_mult_assign_(base);
    }

    opr_add_assign_(base_converter(num_str.back()));
}

bool N::bit_(bit_type pos) const {
    FUNCTION_TO_LOG;

    const std::size_t pos_whole = pos / base_int_bits;
    const bit_type pos_mod = pos % base_int_bits;

    if (pos_whole < digits_.size()) {
        return (digits_[pos_whole] >> pos_mod) & 1;
    } else {
        return 0;
    }
}

void N::bit_(bit_type pos, bool val) {
    FUNCTION_TO_LOG;

    const std::size_t pos_whole = pos / base_int_bits;
    const bit_type pos_mod = pos % base_int_bits;

    if (is_zero()) {
        if (val == 0) return;
        digits_.reserve(pos_whole + 1);
        digits_.insert(digits_.begin(), pos_whole, 0);
        digits_.emplace_back((base_int)1 << pos_mod);
    } else {
        if (pos_whole < digits_.size()) {
            const base_int bitmask = (base_int)1 << pos_mod;
            if (val) {
                digits_[pos_whole] |= bitmask;
            } else {
                digits_[pos_whole] &= ~bitmask;
            }

            remove_leading_zeroes_();
        } else {
            if (val == 0) return;
            digits_.reserve(digits_.size() + pos_whole + 1);
            digits_.insert(digits_.end(), pos_whole - digits_.size(), 0);
            digits_.emplace_back((base_int)1 << pos_mod);
        }
    }
}

std::size_t N::dynamic_size_() const {
    FUNCTION_TO_LOG;

    return (digits_.size() * base_int_size);
}

void N::opr_incr_() {
    FUNCTION_TO_LOG;

    for (auto & digit : digits_)
        if (digit++ < max_digit) return;

    digits_.emplace_back(1);
}

void N::opr_decr_() {
    FUNCTION_TO_LOG;

    for (auto & digit : digits_) {
        if (digit-- > 0) {
            remove_leading_zeroes_();
            return;
        }
    }

    remove_leading_zeroes_();
}

void N::opr_add_assign_(const N & rhs) {
    FUNCTION_TO_LOG;

    // check for additive identity
    if (rhs.is_zero()) return;
    if (this->is_zero()) return (void)operator=(rhs);

    // minimum number of digits in sum; this expression is >= number of digits
    // in rhs
    digits_.reserve(std::max(this->digits_.size(), rhs.digits_.size()) + 1);
    digits_.resize(std::max(this->digits_.size(), rhs.digits_.size()));

    bool carry = false;

    std::size_t i = 0;

    for (; i < rhs.digits_.size(); ++i) {
        const bool next_carry = !(rhs.digits_[i] < (carry ? max_digit : radix) - digits_[i]);

        digits_[i] += carry + rhs.digits_[i];
        carry = next_carry;
    }

    if (carry) {
        for (; i < digits_.size(); ++i) {
            if (digits_[i]++ < max_digit) {
                return;
            }
        }

        digits_.emplace_back(1);
    }
}

void N::opr_subtr_assign_(const N & rhs) {
    FUNCTION_TO_LOG;

    // this functions assumes that *this >= rhs and for effiency reasons should
    // only be called when *this > rhs

    // check for additive identity
    if (rhs.is_zero()) return;

    for (std::size_t i = 0; i < rhs.digits_.size(); ++i) {
        if (this->digits_[i] < rhs.digits_[i]) {
            for (std::size_t j = i + 1; j < digits_.size(); ++j) {
                if ((digits_[j])-- > 0) break;
            }
        }

        this->digits_[i] -= rhs.digits_[i];
    }

    remove_leading_zeroes_();
}

void N::opr_mult_assign_(const N & rhs) {
    FUNCTION_TO_LOG;

    // !!! remove temporary (at least temp1) !!!

    // check for multiplicative identity
    if (this->is_one()) return (void)operator=(rhs);
    if (rhs.is_one()) return;

    // check for multiplicative zero
    if (this->is_zero()) return;
    if (rhs.is_zero()) return (void)set_zero();

    N product;

    // max storage surplus would be base_int_size
    product.digits_.reserve(this->digits_.size() + rhs.digits_.size());

    for (std::size_t i = 0; i < rhs.digits_.size(); ++i) {
        N temp1;
        temp1.digits_.reserve(i + this->digits_.size() + 1);
        temp1.digits_.insert(temp1.digits_.begin(), i, 0);
        base_int carry = 0;
        for (std::size_t j = 0; j < this->digits_.size(); ++j) {
            const base_int_big temp2 = (base_int_big)rhs.digits_[i] * (base_int_big)this->digits_[j];
            const base_int temp3 = (base_int)temp2;
            temp1.digits_.emplace_back(carry + temp3);
            const base_int temp_carry = ((base_int_big)carry + (base_int_big)temp3) >> base_int_bits;
            carry = (temp2 >> base_int_bits) + temp_carry;
        }

        if (carry) temp1.digits_.emplace_back(carry);
        product.opr_add_assign_(temp1);
    }

    operator=(std::move(product));
}

void N::opr_and_assign_(const N & rhs) {
    FUNCTION_TO_LOG;

    if (this->is_zero()) return;
    if (rhs.is_zero()) return (void)set_zero();

    digits_.resize(std::min(this->digits_.size(), rhs.digits_.size()));

    for (std::size_t i = 0; i < digits_.size(); ++i) {
        this->digits_[i] &= rhs.digits_[i];
    }

    remove_leading_zeroes_();
}

void N::opr_or_assign_(const N & rhs) {
    FUNCTION_TO_LOG;

    if (this->is_zero()) return (void)operator=(rhs);
    if (rhs.is_zero()) return;

    const N * longest;
    const N * shortest;

    if (this->digits_.size() < rhs.digits_.size()) {
        longest = &rhs;
        shortest = this;
    } else {
        longest = this;
        shortest = &rhs;
    }

    digits_.reserve(longest->digits_.size());

    std::size_t i = 0;

    for (; i < shortest->digits_.size(); ++i) {
        this->digits_[i] |= rhs.digits_[i];
    }

    if (longest == this) return;

    for (; i < rhs.digits_.size(); ++i) {
        this->digits_.emplace_back(rhs.digits_[i]);
    }
}

void N::opr_xor_assign_(const N & rhs) {
    FUNCTION_TO_LOG;

    if (this->is_zero()) return (void)operator=(rhs);
    if (rhs.is_zero()) return;

    const N * longest;
    const N * shortest;

    if (this->digits_.size() < rhs.digits_.size()) {
        longest = &rhs;
        shortest = this;
    } else {
        longest = this;
        shortest = &rhs;
    }

    digits_.reserve(longest->digits_.size());

    std::size_t i = 0;

    for (; i < shortest->digits_.size(); ++i) {
        this->digits_[i] ^= rhs.digits_[i];
    }

    if (longest == this) return;

    for (; i < rhs.digits_.size(); ++i) {
        this->digits_.emplace_back(rhs.digits_[i]);
    }
}

N N::opr_compl_() const {
    FUNCTION_TO_LOG;

    // this function returns 0 if *this == 0

    N inverted;

    inverted.digits_.reserve(digits_.size());

    for (const auto & digit : digits_)
        inverted.digits_.emplace_back(~digit);

    inverted.remove_leading_zeroes_();

    return inverted;
}

N N::opr_bitshift_l_(bit_type pos) const {
    FUNCTION_TO_LOG;

    if (is_zero()) return N{};
    if (pos == 0) return *this;

    const std::size_t pos_whole = pos / base_int_bits;
    const bit_type pos_mod = pos % base_int_bits;

    N shifted;

    shifted.digits_.reserve(digits_.size() + pos_whole + 1);

    shifted.digits_.insert(shifted.digits_.begin(), pos_whole, 0);

    if (pos_mod == 0) {
        std::ranges::copy(digits_, std::back_inserter(shifted.digits_));
    } else {
        shifted.digits_.emplace_back(digits_.front() << pos_mod);

        for (std::size_t i = 1; i < digits_.size(); ++i) {
            shifted.digits_.emplace_back((digits_[i - 1] >> (base_int_bits - pos_mod)) + (digits_[i] << pos_mod));
        }

        shifted.digits_.emplace_back(digits_.back() >> (base_int_bits - pos_mod));

        shifted.remove_leading_zeroes_();
    }

    return shifted;
}

N N::opr_bitshift_r_(bit_type pos) const {
    FUNCTION_TO_LOG;

    if (is_zero()) return N{};
    if (pos == 0) return *this;

    const std::size_t pos_whole = pos / base_int_bits;

    if (pos_whole >= digits_.size()) return N{};

    const bit_type pos_mod = pos % base_int_bits;

    N shifted;

    shifted.digits_.reserve(digits_.size() - pos_whole);

    if (pos_mod == 0) {
        std::copy(digits_.data() + pos_whole, digits_.data() + digits_.size(), std::back_inserter(shifted.digits_));
    } else {
        for (std::size_t i = pos_whole; i < digits_.size() - 1; ++i) {
            shifted.digits_.emplace_back((digits_[i] >> pos_mod) + (digits_[i + 1] << (base_int_bits - pos_mod)));
        }

        shifted.digits_.emplace_back(digits_.back() >> pos_mod);

        shifted.remove_leading_zeroes_();
    }

    return shifted;
}

void N::opr_bitshift_l_assign_(bit_type pos) {
    FUNCTION_TO_LOG;

    if (is_zero()) return;
    if (pos == 0) return;

    const std::size_t pos_whole = pos / base_int_bits;
    const bit_type pos_mod = pos % base_int_bits;

    digits_.reserve(digits_.size() + pos_whole + 1);

    if (pos_mod != 0) {
        base_int previous = digits_.front();

        digits_.front() <<= pos_mod;

        for (std::size_t i = 1; i < digits_.size(); ++i) {
            const base_int current = digits_[i];
            digits_[i] = (previous >> (base_int_bits - pos_mod)) | (digits_[i] << pos_mod);
            previous = current;
        }

        digits_.emplace_back(previous >> (base_int_bits - pos_mod));

        remove_leading_zeroes_();
    }

    digits_.insert(digits_.begin(), pos_whole, 0);
}

void N::opr_bitshift_r_assign_(bit_type pos) {
    FUNCTION_TO_LOG;

    if (is_zero()) return;
    if (pos == 0) return;

    const std::size_t pos_whole = pos / base_int_bits;

    if (pos_whole >= digits_.size()) return (void)set_zero();

    digits_.erase(digits_.begin(), digits_.begin() + pos_whole);

    if (const bit_type pos_mod = pos % base_int_bits; pos_mod != 0) {
        for (std::size_t i = 0; i < digits_.size() - 1; ++i) {
            digits_[i] = ((digits_[i] >> pos_mod) + (digits_[i + 1] << (base_int_bits - pos_mod)));
        }

        digits_.back() >>= pos_mod;

        remove_leading_zeroes_();
    }
}

void N::opr_assign_(std::string_view num_str) {
    FUNCTION_TO_LOG;

    set_zero();

    handle_str_(num_str, default_base);
}

N::N() = default;

N::N(std::string_view num_str, unsigned base) {
    FUNCTION_TO_LOG;

    if (base < 2 || base > 64) throw error::invalid_base("You need to provide a string in a base between 2 and 64!");

    handle_str_(num_str, base);
}

bool N::is_zero() const {
    FUNCTION_TO_LOG;

    return digits_.empty();
}

bool N::is_one() const {
    FUNCTION_TO_LOG;

    return (digits_.size() == 1 && digits_.front() == 1);
}

bool N::is_even() const {
    FUNCTION_TO_LOG;

    return !is_odd();
}

bool N::is_odd() const {
    FUNCTION_TO_LOG;

    if (is_zero()) return false;

    return (digits_.front() & 1);
}

bit_type N::ctz() const {
    FUNCTION_TO_LOG;

    bit_type tz = 0;
    for (const auto & digit : digits_) {
        if (digit != 0) {
            tz += std::countr_zero(digit);
            break;
        }

        tz += base_int_bits;
    }

    return tz;
}

bit_type N::bits() const {
    FUNCTION_TO_LOG;

    if (is_zero()) return 1;
    return (digits_.size() * base_int_bits - std::countl_zero(digits_.back()));
}

std::size_t N::size() const {
    FUNCTION_TO_LOG;

    return (sizeof(*this) + dynamic_size_());
}

std::string N::to_str(unsigned base) const {
    FUNCTION_TO_LOG;

    if (base < 2 || base > 64) throw error::invalid_base("You need to enter a base between 2 and 64!");

    return conv_to_base_(base);
}

std::string N::to_hex() const {
    FUNCTION_TO_LOG;

    if (is_zero()) return "0";

    std::ostringstream oss;

    oss.setf(std::ios_base::hex, std::ios_base::basefield);
    oss.setf(std::ios_base::uppercase);

    auto crit = digits_.crbegin();

    oss << *crit;

    oss.setf(std::ios_base::right, std::ios_base::adjustfield);
    static constexpr auto field_width = base_int_bits / 4;
    oss.width(field_width);
    oss.fill('0');

    while (++crit != digits_.crend()) {
        oss << *crit;
    }

    return std::move(oss).str();
}

N::operator bool() const {
    FUNCTION_TO_LOG;

    return !is_zero();
}

N::bit_reference N::operator[](bit_type pos) {
    FUNCTION_TO_LOG;

    return bit_reference(*this, pos);
}

N::const_bit_reference N::operator[](bit_type pos) const {
    FUNCTION_TO_LOG;

    return const_bit_reference(*this, pos);
}

void N::set_zero() {
    digits_.clear();
}

N & N::operator++() {
    FUNCTION_TO_LOG;

    opr_incr_();
    return *this;
}

N & N::operator--() {
    FUNCTION_TO_LOG;

    opr_decr_();
    return *this;
}

N & N::operator+=(const N & rhs) {
    FUNCTION_TO_LOG;

    opr_add_assign_(rhs);
    return *this;
}

N & N::operator-=(const N & rhs) {
    FUNCTION_TO_LOG;

    if (const auto difference = detail::opr_comp(*this, rhs); difference == 0)
        set_zero();
    else if (difference > 0)
        opr_subtr_assign_(rhs);
    else
        operator=(detail::opr_subtr(rhs, *this));

    return *this;
}

N & N::operator*=(const N & rhs) {
    FUNCTION_TO_LOG;

    opr_mult_assign_(rhs);
    return *this;
}

N & N::operator&=(const N & rhs) {
    FUNCTION_TO_LOG;

    opr_and_assign_(rhs);
    return *this;
}

N & N::operator|=(const N & rhs) {
    FUNCTION_TO_LOG;

    opr_or_assign_(rhs);
    return *this;
}

N & N::operator^=(const N & rhs) {
    FUNCTION_TO_LOG;

    opr_xor_assign_(rhs);
    return *this;
}

N N::operator~() const {
    FUNCTION_TO_LOG;

    return opr_compl_();
}

N N::operator<<(bit_type pos) const {
    FUNCTION_TO_LOG;

    return opr_bitshift_l_(pos);
}

N N::operator>>(bit_type pos) const {
    FUNCTION_TO_LOG;

    return opr_bitshift_r_(pos);
}

N & N::operator<<=(bit_type pos) {
    FUNCTION_TO_LOG;

    opr_bitshift_l_assign_(pos);
    return *this;
}

N & N::operator>>=(bit_type pos) {
    FUNCTION_TO_LOG;

    opr_bitshift_r_assign_(pos);
    return *this;
}

N & N::operator=(std::string_view num_str) {
    FUNCTION_TO_LOG;

    opr_assign_(num_str);
    return *this;
}

}  // namespace jmaths

namespace jmaths {

N::bit_reference::bit_reference(const bit_reference & ref) = default;
N::const_bit_reference::const_bit_reference(const const_bit_reference & ref) = default;
N::const_bit_reference::const_bit_reference(const bit_reference & ref) : bit_reference_base_(ref.num_, ref.pos_) {
    FUNCTION_TO_LOG;
}

N::bit_reference & N::bit_reference::operator=(bool val) {
    FUNCTION_TO_LOG;

    num_.bit_(pos_, val);
    num_.remove_leading_zeroes_();
    return *this;
}

N::bit_reference & N::bit_reference::operator=(const bit_reference & ref) {
    FUNCTION_TO_LOG;

    num_.bit_(pos_, ref.num_.bit_(ref.pos_));
    num_.remove_leading_zeroes_();
    return *this;
}

N::bit_reference & N::bit_reference::operator=(const const_bit_reference & ref) {
    FUNCTION_TO_LOG;

    num_.bit_(pos_, ref.num_.bit_(ref.pos_));
    num_.remove_leading_zeroes_();
    return *this;
}

}  // namespace jmaths

#include "undef.hh"
