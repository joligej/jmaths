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

#include <algorithm>
#include <bit>
#include <bitset>
#include <cassert>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstddef>
#include <def.hh>
#include <ios>
#include <istream>
#include <iterator>
#include <limits>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "N.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "error.hpp"

// comparison functions for N with integral types
namespace jmaths {

bool N::detail::opr_eq(const N & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit == rhs;
    }

    return false;
}

std::strong_ordering N::detail::opr_comp(const N & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit <=> rhs;
    }

    return std::strong_ordering::greater;
}

bool operator==(const N & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_eq(lhs, rhs);
}

bool operator==(std::integral auto lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_eq(rhs, lhs);
}

std::strong_ordering operator<=>(const N & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_comp(lhs, rhs);
}

std::strong_ordering operator<=>(std::integral auto lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return 0 <=> N::detail::opr_comp(rhs, lhs);
}

}  // namespace jmaths

// member function templates of N
namespace jmaths {

void N::handle_int_(std::integral auto num) {
    JMATHS_FUNCTION_TO_LOG;

    if constexpr (base_int_size < sizeof(num)) {
        static constexpr std::size_t digits_needed = static_cast<std::size_t>(
            std::ceil(static_cast<long double>(sizeof(num)) / base_int_size));

        digits_.reserve(digits_needed);
#if 0
        for (std::size_t curr_byte = 0; curr_byte < sizeof(num); ++curr_byte) {
            const std::size_t curr_index = curr_byte / base_int_size;
            const std::size_t curr_offset = curr_byte % base_int_size;
            if (curr_offset == 0) { digits_.emplace_back(); }

            static constexpr auto bitmask = static_cast<unsigned>(~std::byte{0});

            digits_[curr_index] |= (num & bitmask) << (curr_offset * bits_in_byte);

            num >>= bits_in_byte;
        }
#else
        for (std::size_t curr_base_int_part = 0U; curr_base_int_part < sizeof(num) / base_int_size;
             ++curr_base_int_part) {
            digits_.emplace_back(static_cast<base_int>(num));
            num >>= base_int_bits;
        }

        if constexpr (sizeof(num) % base_int_size > 0U) {
    #if 0
            digits_.emplace_back();

            for (std::size_t curr_byte = 0; curr_byte < sizeof(num) % base_int_size; ++curr_byte) {
                static constexpr auto bitmask = static_cast<unsigned>(~std::byte{0});
                digits_[sizeof(num) / base_int_size] |= (num & bitmask) << (curr_byte * bits_in_byte);
                num >>= bits_in_byte;
            }
    #else
            digits_.emplace_back(static_cast<base_int>(num));
    #endif
        }
#endif

        assert(num == 0);
    } else {
        digits_.emplace_back(static_cast<base_int>(num));
    }

    remove_leading_zeroes_();
}

template <std::unsigned_integral T> T N::fit_into_(std::size_t max_byte) const {
    JMATHS_FUNCTION_TO_LOG;

    T converted = 0;

    for (std::size_t curr_byte = 0U; curr_byte < max_byte; ++curr_byte) {
        const std::size_t curr_index = curr_byte / base_int_size;
        const std::size_t curr_offset = curr_byte % base_int_size;

        static constexpr auto bitmask = static_cast<unsigned>(~std::byte{0U});

        const auto relevant_byte = (digits_[curr_index] >> (curr_offset * bits_in_byte)) & bitmask;

        converted |= (static_cast<T>(relevant_byte) << (curr_byte * bits_in_byte));
    }

    return converted;
}

void N::opr_assign_(std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    set_zero();
    handle_int_(rhs);
}

N::N(std::integral auto num) {
    JMATHS_FUNCTION_TO_LOG;

    handle_int_(num);
}

template <std::unsigned_integral T> std::optional<T> N::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return 0; }

    if constexpr (base_int_size < sizeof(T)) {
        if (digits_.size() * base_int_size < sizeof(T)) {
            return fit_into_<T>(digits_.size() * base_int_size);
        }

        if (digits_.size() * base_int_size > sizeof(T)) {
            if (static_cast<unsigned>(std::countr_zero(digits_.back())) <
                (digits_.size() * base_int_size - sizeof(T)) * bits_in_byte) {
                return std::nullopt;
            }
        }

        return fit_into_<T>(sizeof(T));
    } else if constexpr (base_int_size > sizeof(T)) {
        if (digits_.size() > 1U) { return std::nullopt; }

        static constexpr auto bitmask = ~static_cast<T>(0);

        if (const auto masked_digit = digits_.front() & bitmask; masked_digit == digits_.front()) {
            return masked_digit;
        }

        return std::nullopt;
    } else {
        if (digits_.size() > 1U) { return std::nullopt; }

        return digits_.front();
    }
}

template <std::signed_integral T> std::optional<T> N::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    auto fits_into_unsigned = fits_into<std::make_unsigned_t<T>>();

    if (!fits_into_unsigned.has_value()) { return std::nullopt; }

    if (*fits_into_unsigned > std::numeric_limits<T>::max()) { return std::nullopt; }

    return *fits_into_unsigned;
}

N & N::operator=(std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    opr_assign_(rhs);
    return *this;
}

}  // namespace jmaths

// functions for N::bit_reference_base_
namespace jmaths {

template <TMP::decays_to<N> T>
N::bit_reference_base_<T>::bit_reference_base_(T & num, bitpos_t pos) : num_(num), pos_{pos} {
    JMATHS_FUNCTION_TO_LOG;
}

template <TMP::decays_to<N> T> N::bit_reference_base_<T>::operator bool() const {
    JMATHS_FUNCTION_TO_LOG;

    return num_.bit_(pos_);
}

template <TMP::decays_to<N> T> N::bit_reference_base_<T>::operator int() const {
    JMATHS_FUNCTION_TO_LOG;

    return operator bool();
}

}  // namespace jmaths

// static class member variable definitions
namespace jmaths {

inline const N N::one_(1U);

}  // namespace jmaths

// some implementation functions

namespace jmaths::impl {

base_int base_converter(char c) noexcept;  // convert char to number for base >= 2 and <= 64

inline base_int base_converter(char c) noexcept {
    JMATHS_FUNCTION_TO_LOG;

    if (c >= '0' && c <= '9') { return static_cast<unsigned char>(c - '0'); }
    if (c >= 'A' && c <= 'Z') { return static_cast<unsigned char>(c - 'A' + 10); }
    if (c >= 'a' && c <= 'z') { return static_cast<unsigned char>(c - 'a' + 10 + 26); }
    if (c == '+') { return 62U; }

    assert(c == '~');  // it is assumed that c == '~' because no other character
                       // would be valid or logical

    return 63U;
}

}  // namespace jmaths::impl

// binary operators for N
namespace jmaths {

/**********************************************************/
// forwarding functions

inline std::ostream & operator<<(std::ostream & os, const N & n) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_ins(os, n);
}

inline std::istream & operator>>(std::istream & is, N & n) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_extr(is, n);
}

inline N operator+(const N & lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_add(lhs, rhs);
}

inline N operator-(const N & lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    const auto difference = N::detail::opr_comp(lhs, rhs);

    if (difference == 0) { return N{}; }
    return difference > 0 ? N::detail::opr_subtr(lhs, rhs) : N::detail::opr_subtr(rhs, lhs);
}

inline N operator*(const N & lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_mult(lhs, rhs);
}

inline std::pair<N, N> operator/(const N & lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(rhs);
    return N::detail::opr_div(lhs, rhs);
}

inline N operator&(const N & lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_and(lhs, rhs);
}

inline N operator|(const N & lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_or(lhs, rhs);
}

inline N operator^(const N & lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_xor(lhs, rhs);
}

inline bool operator==(const N & lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_eq(lhs, rhs);
}

inline std::strong_ordering operator<=>(const N & lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return N::detail::opr_comp(lhs, rhs);
}

}  // namespace jmaths

// member functions of N
namespace jmaths {

inline std::string N::conv_to_base_(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    static constexpr char base_converter[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+~";

    if (is_zero()) {
        static constexpr std::string zero = "0";
        return zero;
    }

    std::string num_str;

    const std::size_t digits_needed = (digits_.size() * base_int_bits) / base + 1U;

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

inline void N::remove_leading_zeroes_() {
    JMATHS_FUNCTION_TO_LOG;

    while (!digits_.empty()) {
        if (digits_.back() != 0U) { break; }
        digits_.pop_back();
    }

    assert(digits_.empty() || digits_.back() != 0U);
}

inline base_int N::front_() const {
    JMATHS_FUNCTION_TO_LOG;

    if (digits_.empty()) { return 0U; }

    return digits_.front();
}

inline void N::handle_str_(std::string_view num_str, unsigned base) {
    JMATHS_FUNCTION_TO_LOG;

    // the naive yet faster check:
    // if (num_str.empty()) { return; }
    // if (num_str.size() == 1U && num_str.front() == '0') { return; }

    // the more thorough but slower check:
    if (std::ranges::all_of(num_str, [](char c) static noexcept { return c == '0'; })) { return; }

    assert(!num_str.empty());

    for (auto cit = num_str.cbegin(); &*cit != &num_str.back(); ++cit) {
        opr_add_assign_(impl::base_converter(*cit));
        opr_mult_assign_(base);
    }

    opr_add_assign_(impl::base_converter(num_str.back()));
}

inline bool N::bit_(bitpos_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    const std::size_t pos_whole = pos / base_int_bits;
    const bitpos_t pos_mod = pos % base_int_bits;

    if (pos_whole < digits_.size()) {
        return (digits_[pos_whole] >> pos_mod) & 1U;
    } else {
        return 0;
    }
}

inline void N::bit_(bitpos_t pos, bool val) {
    JMATHS_FUNCTION_TO_LOG;

    const std::size_t pos_whole = pos / base_int_bits;
    const bitpos_t pos_mod = pos % base_int_bits;

    if (is_zero()) {
        if (val == 0) { return; }
        digits_.reserve(pos_whole + 1U);
        digits_.insert(digits_.begin(), pos_whole, 0U);
        digits_.emplace_back(static_cast<base_int>(1) << pos_mod);
    } else {
        if (pos_whole < digits_.size()) {
            const base_int bitmask = static_cast<base_int>(1) << pos_mod;
            if (val) {
                digits_[pos_whole] |= bitmask;
            } else {
                digits_[pos_whole] &= ~bitmask;
            }

            remove_leading_zeroes_();
        } else {
            if (val == 0) { return; }
            digits_.reserve(digits_.size() + pos_whole + 1U);
            digits_.insert(digits_.end(), pos_whole - digits_.size(), 0U);
            digits_.emplace_back(static_cast<base_int>(1) << pos_mod);
        }
    }
}

inline std::size_t N::dynamic_size_() const {
    JMATHS_FUNCTION_TO_LOG;

    return digits_.size() * base_int_size;
}

inline void N::opr_incr_() {
    JMATHS_FUNCTION_TO_LOG;

    for (auto & digit : digits_) {
        if (digit++ < max_digit) { return; }
    }

    digits_.emplace_back(1);
}

inline void N::opr_decr_() {
    JMATHS_FUNCTION_TO_LOG;

    for (auto & digit : digits_) {
        if (digit-- > 0U) {
            remove_leading_zeroes_();
            return;
        }
    }

    remove_leading_zeroes_();
}

inline void N::opr_add_assign_(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    // check for additive identity
    if (rhs.is_zero()) { return; }

    if (this->is_zero()) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overread"
        operator=(rhs);
#pragma GCC diagnostic pop
        return;
    }

    // minimum number of digits in sum; this expression is >= number of digits
    // in rhs
    digits_.reserve(std::max(this->digits_.size(), rhs.digits_.size()) + 1U);
    digits_.resize(std::max(this->digits_.size(), rhs.digits_.size()));

    bool carry = false;

    std::size_t i = 0U;

    for (; i < rhs.digits_.size(); ++i) {
        const bool next_carry = !(rhs.digits_[i] < (carry ? max_digit : radix) - digits_[i]);

        digits_[i] += carry + rhs.digits_[i];
        carry = next_carry;
    }

    if (carry) {
        for (; i < digits_.size(); ++i) {
            if (digits_[i]++ < max_digit) { return; }
        }

        digits_.emplace_back(1);
    }
}

inline void N::opr_subtr_assign_(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    // this functions assumes that *this >= rhs and for effiency reasons should
    // only be called when *this > rhs

    // check for additive identity
    if (rhs.is_zero()) { return; }

    for (std::size_t i = 0U; i < rhs.digits_.size(); ++i) {
        if (this->digits_[i] < rhs.digits_[i]) {
            for (std::size_t j = i + 1U; j < digits_.size(); ++j) {
                if (digits_[j]-- > 0U) { break; }
            }
        }

        this->digits_[i] -= rhs.digits_[i];
    }

    remove_leading_zeroes_();
}

inline void N::opr_mult_assign_(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    // !!! remove temporary (at least temp1) !!!

    // check for multiplicative identity
    if (this->is_one()) {
        operator=(rhs);
        return;
    }

    if (rhs.is_one()) { return; }

    // check for multiplicative zero
    if (this->is_zero()) { return; }

    if (rhs.is_zero()) {
        set_zero();
        return;
    }

    N product;

    // max storage surplus would be base_int_size
    product.digits_.reserve(this->digits_.size() + rhs.digits_.size());

    for (std::size_t i = 0U; i < rhs.digits_.size(); ++i) {
        N temp1;
        temp1.digits_.reserve(i + this->digits_.size() + 1U);
        temp1.digits_.insert(temp1.digits_.begin(), i, 0U);
        base_int carry = 0U;
        for (std::size_t j = 0U; j < this->digits_.size(); ++j) {
            const base_int_big temp2 = static_cast<base_int_big>(rhs.digits_[i]) *
                                       static_cast<base_int_big>(this->digits_[j]);
            const base_int temp3 = static_cast<base_int>(temp2);
            temp1.digits_.emplace_back(carry + temp3);
            const base_int temp_carry = static_cast<base_int>(
                (static_cast<base_int_big>(carry) + static_cast<base_int_big>(temp3)) >>
                base_int_bits);
            carry = static_cast<base_int>((temp2 >> base_int_bits) + temp_carry);
        }

        if (carry != 0U) { temp1.digits_.emplace_back(carry); }
        product.opr_add_assign_(temp1);
    }

    operator=(std::move(product));
}

inline void N::opr_and_assign_(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_zero()) { return; }

    if (rhs.is_zero()) {
        set_zero();
        return;
    }

    digits_.resize(std::min(this->digits_.size(), rhs.digits_.size()));

    for (std::size_t i = 0U; i < digits_.size(); ++i) {
        this->digits_[i] &= rhs.digits_[i];
    }

    remove_leading_zeroes_();
}

inline void N::opr_or_assign_(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_zero()) {
        operator=(rhs);
        return;
    }

    if (rhs.is_zero()) { return; }

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

    std::size_t i = 0U;

    for (; i < shortest->digits_.size(); ++i) {
        this->digits_[i] |= rhs.digits_[i];
    }

    if (longest == this) { return; }

    for (; i < rhs.digits_.size(); ++i) {
        this->digits_.emplace_back(rhs.digits_[i]);
    }
}

inline void N::opr_xor_assign_(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_zero()) {
        operator=(rhs);
        return;
    }
    if (rhs.is_zero()) { return; }

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

    std::size_t i = 0U;

    for (; i < shortest->digits_.size(); ++i) {
        this->digits_[i] ^= rhs.digits_[i];
    }

    if (longest == this) { return; }

    for (; i < rhs.digits_.size(); ++i) {
        this->digits_.emplace_back(rhs.digits_[i]);
    }
}

inline N N::opr_compl_() const {
    JMATHS_FUNCTION_TO_LOG;

    // this function returns 0 if *this == 0

    N inverted;

    inverted.digits_.reserve(digits_.size());

    for (const auto & digit : digits_) {
        inverted.digits_.emplace_back(~digit);
    }

    inverted.remove_leading_zeroes_();

    return inverted;
}

inline N N::opr_bitshift_l_(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return N{}; }
    if (pos == 0U) { return *this; }

    const std::size_t pos_whole = pos / base_int_bits;
    const bitpos_t pos_mod = pos % base_int_bits;

    N shifted;

    shifted.digits_.reserve(digits_.size() + pos_whole + 1U);

    shifted.digits_.insert(shifted.digits_.begin(), pos_whole, 0U);

    if (pos_mod == 0U) {
        shifted.digits_.insert(shifted.digits_.end(), digits_.cbegin(), digits_.cend());
    } else {
        shifted.digits_.emplace_back(digits_.front() << pos_mod);

        for (std::size_t i = 1U; i < digits_.size(); ++i) {
            shifted.digits_.emplace_back((digits_[i - 1U] >> (base_int_bits - pos_mod)) +
                                         (digits_[i] << pos_mod));
        }

        shifted.digits_.emplace_back(digits_.back() >> (base_int_bits - pos_mod));

        shifted.remove_leading_zeroes_();
    }

    return shifted;
}

inline N N::opr_bitshift_r_(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return N{}; }
    if (pos == 0U) { return *this; }

    const std::size_t pos_whole = pos / base_int_bits;

    if (pos_whole >= digits_.size()) { return N{}; }

    const bitpos_t pos_mod = pos % base_int_bits;

    N shifted;

    shifted.digits_.reserve(digits_.size() - pos_whole);

    if (pos_mod == 0U) {
        shifted.digits_.insert(
            shifted.digits_.end(),
            digits_.cbegin() + static_cast<decltype(digits_)::difference_type>(pos_whole),
            digits_.cend());
    } else {
        for (std::size_t i = pos_whole; i < digits_.size() - 1U; ++i) {
            shifted.digits_.emplace_back((digits_[i] >> pos_mod) +
                                         (digits_[i + 1U] << (base_int_bits - pos_mod)));
        }

        shifted.digits_.emplace_back(digits_.back() >> pos_mod);

        shifted.remove_leading_zeroes_();
    }

    return shifted;
}

inline void N::opr_bitshift_l_assign_(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return; }
    if (pos == 0U) { return; }

    const std::size_t pos_whole = pos / base_int_bits;
    const bitpos_t pos_mod = pos % base_int_bits;

    digits_.reserve(digits_.size() + pos_whole + 1U);

    if (pos_mod != 0U) {
        base_int previous = digits_.front();

        digits_.front() <<= pos_mod;

        for (std::size_t i = 1U; i < digits_.size(); ++i) {
            const base_int current = digits_[i];
            digits_[i] = (previous >> (base_int_bits - pos_mod)) | (digits_[i] << pos_mod);
            previous = current;
        }

        digits_.emplace_back(previous >> (base_int_bits - pos_mod));

        remove_leading_zeroes_();
    }

    digits_.insert(digits_.begin(), pos_whole, 0U);
}

inline void N::opr_bitshift_r_assign_(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return; }
    if (pos == 0U) { return; }

    const std::size_t pos_whole = pos / base_int_bits;

    if (pos_whole >= digits_.size()) {
        set_zero();
        return;
    }

    digits_.erase(digits_.begin(),
                  digits_.begin() + static_cast<decltype(digits_)::difference_type>(pos_whole));

    if (const bitpos_t pos_mod = pos % base_int_bits; pos_mod != 0U) {
        for (std::size_t i = 0U; i < digits_.size() - 1U; ++i) {
            digits_[i] = (digits_[i] >> pos_mod) + (digits_[i + 1U] << (base_int_bits - pos_mod));
        }

        digits_.back() >>= pos_mod;

        remove_leading_zeroes_();
    }
}

inline void N::opr_assign_(std::string_view num_str) {
    JMATHS_FUNCTION_TO_LOG;

    set_zero();

    handle_str_(num_str, default_base);
}

inline N::N() = default;

inline N::N(std::string_view num_str, unsigned base) {
    JMATHS_FUNCTION_TO_LOG;

    error::invalid_base::check(base);

    handle_str_(num_str, base);
}

inline bool N::is_zero() const {
    JMATHS_FUNCTION_TO_LOG;

    return digits_.empty();
}

inline bool N::is_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return digits_.size() == 1U && digits_.front() == 1U;
}

inline bool N::is_even() const {
    JMATHS_FUNCTION_TO_LOG;

    return !is_odd();
}

inline bool N::is_odd() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return false; }

    return digits_.front() & 1U;
}

inline bitcount_t N::ctz() const {
    JMATHS_FUNCTION_TO_LOG;

    bitcount_t tz = 0U;
    for (const auto & digit : digits_) {
        if (digit != 0U) {
            tz += static_cast<bitcount_t>(std::countr_zero(digit));
            break;
        }

        tz += base_int_bits;
    }

    return tz;
}

inline bitcount_t N::bits() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return 1U; }
    return digits_.size() * base_int_bits -
           static_cast<bitcount_t>(std::countl_zero(digits_.back()));
}

inline std::size_t N::size() const {
    JMATHS_FUNCTION_TO_LOG;

    return sizeof(*this) + dynamic_size_();
}

inline std::string N::to_str(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    error::invalid_base::check(base);

    return conv_to_base_(base);
}

inline std::string N::to_hex() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) {
        static constexpr std::string zero = "0";
        return zero;
    }

    std::ostringstream oss;

    oss.setf(std::ios_base::hex, std::ios_base::basefield);
    oss.setf(std::ios_base::uppercase);

    auto crit = digits_.crbegin();

    oss << *crit;

    oss.setf(std::ios_base::right, std::ios_base::adjustfield);
    static constexpr auto field_width = base_int_bits / 4U;
    oss.width(field_width);
    oss.fill('0');

    while (++crit != digits_.crend()) {
        oss << *crit;
    }

    return std::move(oss).str();
}

inline std::string N::to_bin() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) {
        static constexpr std::string zero = "0";
        return zero;
    }

    using bitset_type = std::bitset<sizeof(base_int) * bits_in_byte>;

    std::ostringstream oss;

    auto crit = digits_.crbegin();

    std::string first_digit = bitset_type(*crit).to_string();
    const auto first_non_zero = first_digit.find_first_not_of('0');
    first_digit.erase(0U, first_non_zero);

    oss << first_digit;

    while (++crit != digits_.crend()) {
        oss << bitset_type(*crit);
    }

    return std::move(oss).str();
}

inline N::operator bool() const {
    JMATHS_FUNCTION_TO_LOG;

    return !is_zero();
}

inline N::bit_reference N::operator[](bitpos_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    return {*this, pos};
}

inline N::const_bit_reference N::operator[](bitpos_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    return {*this, pos};
}

inline void N::set_zero() {
    JMATHS_FUNCTION_TO_LOG;

    digits_.clear();
}

inline N & N::operator++() {
    JMATHS_FUNCTION_TO_LOG;

    opr_incr_();
    return *this;
}

inline N & N::operator--() {
    JMATHS_FUNCTION_TO_LOG;

    opr_decr_();
    return *this;
}

inline N & N::operator+=(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    opr_add_assign_(rhs);
    return *this;
}

inline N & N::operator-=(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto difference = detail::opr_comp(*this, rhs); difference == 0) {
        set_zero();
    } else if (difference > 0) {
        opr_subtr_assign_(rhs);
    } else {
        operator=(detail::opr_subtr(rhs, *this));
    }

    return *this;
}

inline N & N::operator*=(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    opr_mult_assign_(rhs);
    return *this;
}

inline N & N::operator&=(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    opr_and_assign_(rhs);
    return *this;
}

inline N & N::operator|=(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    opr_or_assign_(rhs);
    return *this;
}

inline N & N::operator^=(const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    opr_xor_assign_(rhs);
    return *this;
}

inline N N::operator~() const {
    JMATHS_FUNCTION_TO_LOG;

    return opr_compl_();
}

inline N N::operator<<(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    return opr_bitshift_l_(pos);
}

inline N N::operator>>(bitcount_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    return opr_bitshift_r_(pos);
}

inline N & N::operator<<=(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    opr_bitshift_l_assign_(pos);
    return *this;
}

inline N & N::operator>>=(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    opr_bitshift_r_assign_(pos);
    return *this;
}

inline N & N::operator=(std::string_view num_str) {
    JMATHS_FUNCTION_TO_LOG;

    opr_assign_(num_str);
    return *this;
}

}  // namespace jmaths

// member functions of N::bit_reference_base_ and derived classes
namespace jmaths {

inline N::bit_reference::bit_reference(const bit_reference & ref) = default;
inline N::const_bit_reference::const_bit_reference(const const_bit_reference & ref) = default;
inline N::const_bit_reference::const_bit_reference(const bit_reference & ref) :
    bit_reference_base_(ref.num_, ref.pos_) {
    JMATHS_FUNCTION_TO_LOG;
}

inline N::bit_reference & N::bit_reference::operator=(bool val) {
    JMATHS_FUNCTION_TO_LOG;

    num_.bit_(pos_, val);
    num_.remove_leading_zeroes_();
    return *this;
}

inline N::bit_reference & N::bit_reference::operator=(const bit_reference & ref) {
    JMATHS_FUNCTION_TO_LOG;

    num_.bit_(pos_, ref.num_.bit_(ref.pos_));
    num_.remove_leading_zeroes_();
    return *this;
}

inline N::bit_reference & N::bit_reference::operator=(const const_bit_reference & ref) {
    JMATHS_FUNCTION_TO_LOG;

    num_.bit_(pos_, ref.num_.bit_(ref.pos_));
    num_.remove_leading_zeroes_();
    return *this;
}

}  // namespace jmaths
