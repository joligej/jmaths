// The jmaths library for C++
// Copyright (C) 2025  Jasper de Smaele

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

#include "basic_N.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "error.hpp"

// comparison functions for N with integral types
namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_eq(const basic_N & lhs,
                                                                       std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.template fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit == rhs;
    }

    return false;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::strong_ordering basic_N<BaseInt, BaseIntBig, Allocator>::detail::opr_comp(
    const basic_N & lhs,
    std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.template fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit <=> rhs;
    }

    return std::strong_ordering::greater;
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr bool operator==(const basic_N_type & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_eq(lhs, rhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr bool operator==(std::integral auto lhs, const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_eq(rhs, lhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::strong_ordering operator<=>(const basic_N_type & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_comp(lhs, rhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::strong_ordering operator<=>(std::integral auto lhs, const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return 0 <=> basic_N_type::detail::opr_comp(rhs, lhs);
}

}  // namespace jmaths

// member function templates of N
namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::handle_int_(std::integral auto num) {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Convert native integer to arbitrary-precision format
    // This function converts a standard C++ integer type (like int, long, uint64_t, etc.)
    // into the internal digit representation used by basic_N.
    //
    // Two strategies depending on integer size vs base_int_type size:
    //
    // Strategy 1: If the input integer is larger than base_int_type
    // - Split the integer into multiple base_int_type digits
    // - Extract each digit by masking and shifting
    // - Store in little-endian order (least significant digit first)
    //
    // Strategy 2: If the input integer fits in one base_int_type
    // - Simply store it as a single digit
    //
    // This handles all integer types uniformly, including types larger than 64 bits
    // if the compiler supports them (e.g., __int128).

    if constexpr (base_int_type_size < sizeof(num)) {
        // Multi-digit case: input is larger than our base digit type
        static constexpr std::size_t digits_needed = static_cast<std::size_t>(std::ceil(
            static_cast<long double>(sizeof(num)) / static_cast<long double>(base_int_type_size)));

        digits_.reserve(digits_needed);
#if 0
        // Alternative byte-by-byte approach (commented out)
        for (std::size_t curr_byte = 0; curr_byte < sizeof(num); ++curr_byte) {
            const std::size_t curr_index = curr_byte / base_int_type_size;
            const std::size_t curr_offset = curr_byte % base_int_type_size;
            if (curr_offset == 0) { digits_.emplace_back(); }

            static constexpr auto bitmask = static_cast<unsigned>(~std::byte{0});

            digits_[curr_index] |= (num & bitmask) << (curr_offset * bits_in_byte);

            num >>= bits_in_byte;
        }
#else
        // Extract full base_int_type sized chunks
        for (std::size_t curr_base_int_part = 0U;
             curr_base_int_part < sizeof(num) / base_int_type_size;
             ++curr_base_int_part) {
            digits_.emplace_back(static_cast<base_int_type>(num));
            num >>= base_int_type_bits;
        }

        // Handle remaining bytes if input size is not a multiple of base_int_type_size
        if constexpr (sizeof(num) % base_int_type_size > 0U) {
    #if 0
            digits_.emplace_back();

            for (std::size_t curr_byte = 0; curr_byte < sizeof(num) % base_int_type_size; ++curr_byte) {
                static constexpr auto bitmask = static_cast<unsigned>(~std::byte{0});
                digits_[sizeof(num) / base_int_type_size] |= (num & bitmask) << (curr_byte * bits_in_byte);
                num >>= bits_in_byte;
            }
    #else
            digits_.emplace_back(static_cast<base_int_type>(num));
    #endif
        }
#endif

        assert(num == 0);
    } else {
        // Single-digit case: input fits in one base_int_type
        digits_.emplace_back(static_cast<base_int_type>(num));
    }

    remove_leading_zeroes_();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <std::unsigned_integral T>
constexpr T basic_N<BaseInt, BaseIntBig, Allocator>::fit_into_(std::size_t max_byte) const {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Convert arbitrary-precision to native unsigned integer
    // This is the inverse of handle_int_, extracting bytes from the internal
    // representation and assembling them into a standard C++ integer type.
    //
    // Process:
    // 1. Iterate through bytes up to max_byte (size of target type)
    // 2. For each byte:
    //    - Calculate which digit it belongs to (curr_index)
    //    - Calculate offset within that digit (curr_offset)
    //    - Extract the byte using bit masking and shifting
    //    - Place it in the correct position in the result
    // 3. Return the assembled integer
    //
    // This reconstructs the original integer from the digit representation.

    T converted = 0;

    for (std::size_t curr_byte = 0U; curr_byte < max_byte; ++curr_byte) {
        const std::size_t curr_index = curr_byte / base_int_type_size;
        const std::size_t curr_offset = curr_byte % base_int_type_size;

        static constexpr auto bitmask = static_cast<unsigned>(~std::byte{0U});

        // Extract the relevant byte from the appropriate digit
        const auto relevant_byte = (digits_[curr_index] >> (curr_offset * bits_in_byte)) & bitmask;

        // Place it in the correct position in the result
        converted |= (static_cast<T>(relevant_byte) << (curr_byte * bits_in_byte));
    }

    return converted;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_assign_(std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    set_zero();
    handle_int_(rhs);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_N<BaseInt, BaseIntBig, Allocator>::basic_N(std::integral auto num) {
    JMATHS_FUNCTION_TO_LOG;

    handle_int_(num);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <std::unsigned_integral T>
constexpr std::optional<T> basic_N<BaseInt, BaseIntBig, Allocator>::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return 0; }

    if constexpr (base_int_type_size < sizeof(T)) {
        if (digits_.size() * base_int_type_size < sizeof(T)) {
            return fit_into_<T>(digits_.size() * base_int_type_size);
        }

        if (digits_.size() * base_int_type_size > sizeof(T)) {
            if (static_cast<unsigned>(std::countr_zero(digits_.back())) <
                (digits_.size() * base_int_type_size - sizeof(T)) * bits_in_byte) {
                return std::nullopt;
            }
        }

        return fit_into_<T>(sizeof(T));
    } else if constexpr (base_int_type_size > sizeof(T)) {
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

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <std::signed_integral T>
constexpr std::optional<T> basic_N<BaseInt, BaseIntBig, Allocator>::fits_into() const {
    JMATHS_FUNCTION_TO_LOG;

    auto fits_into_unsigned = fits_into<std::make_unsigned_t<T>>();

    if (!fits_into_unsigned.has_value()) { return std::nullopt; }

    if (*fits_into_unsigned > std::numeric_limits<T>::max()) { return std::nullopt; }

    return *fits_into_unsigned;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator=(std::integral auto rhs)
    -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_assign_(rhs);
    return *this;
}

}  // namespace jmaths

// functions for N::bit_reference_base_
namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <TMP::decays_to<basic_N<BaseInt, BaseIntBig, Allocator>> T>
constexpr basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference_base_<T>::bit_reference_base_(
    T & num,
    bitpos_t pos) :
    num_(num), pos_{pos} {
    JMATHS_FUNCTION_TO_LOG;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <TMP::decays_to<basic_N<BaseInt, BaseIntBig, Allocator>> T>
constexpr basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference_base_<T>::operator bool() const {
    JMATHS_FUNCTION_TO_LOG;

    return num_.bit_(pos_);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <TMP::decays_to<basic_N<BaseInt, BaseIntBig, Allocator>> T>
constexpr basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference_base_<T>::operator int() const {
    JMATHS_FUNCTION_TO_LOG;

    return operator bool();
}

}  // namespace jmaths

// static class member variable definitions
namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
inline const basic_N<BaseInt, BaseIntBig, Allocator> basic_N<BaseInt, BaseIntBig, Allocator>::one_(
    1U);

}  // namespace jmaths

// some implementation functions

namespace jmaths::impl {

template <std::unsigned_integral BaseInt>
[[nodiscard]] constexpr BaseInt base_converter(
    char c) noexcept;  // convert char to number for base >= 2 and <= 64

template <std::unsigned_integral BaseInt> constexpr BaseInt base_converter(char c) noexcept {
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

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::ostream & operator<<(std::ostream & os, const basic_N_type & n) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_ins(os, n);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::istream & operator>>(std::istream & is, basic_N_type & n) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_extr(is, n);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator+(const basic_N_type & lhs, const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_add(lhs, rhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator-(const basic_N_type & lhs, const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    const auto difference = basic_N_type::detail::opr_comp(lhs, rhs);

    if (difference == 0) { return basic_N_type{}; }
    return difference > 0 ? basic_N_type::detail::opr_subtr(lhs, rhs) :
                            basic_N_type::detail::opr_subtr(rhs, lhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator*(const basic_N_type & lhs, const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_mult(lhs, rhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::pair<basic_N_type, basic_N_type> operator/(const basic_N_type & lhs,
                                                          const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    error::division_by_zero::check(rhs);
    return basic_N_type::detail::opr_div(lhs, rhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator&(const basic_N_type & lhs, const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_and(lhs, rhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator|(const basic_N_type & lhs, const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_or(lhs, rhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator^(const basic_N_type & lhs, const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_xor(lhs, rhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr bool operator==(const basic_N_type & lhs, const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_eq(lhs, rhs);
}

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::strong_ordering operator<=>(const basic_N_type & lhs, const basic_N_type & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return basic_N_type::detail::opr_comp(lhs, rhs);
}

}  // namespace jmaths

// member functions of N
namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_N<BaseInt, BaseIntBig, Allocator>::conv_to_base_(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    static constexpr char base_converter[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+~";

    if (is_zero()) {
        static const /*expr*/ std::string zero = "0";
        return zero;
    }

    std::string num_str;

    const std::size_t digits_needed = (digits_.size() * base_int_type_bits) / base + 1U;

    num_str.reserve(digits_needed);

    basic_N helper(*this);

    while (!helper.is_zero()) {
        auto [quotient, remainder] = detail::opr_div(helper, base);
        num_str.push_back(base_converter[remainder.front_()]);
        helper = std::move(quotient);
    }

    num_str.shrink_to_fit();

    std::ranges::reverse(num_str);

    return num_str;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::remove_leading_zeroes_() {
    JMATHS_FUNCTION_TO_LOG;

    while (!digits_.empty()) {
        if (digits_.back() != 0U) { break; }
        digits_.pop_back();
    }

    assert(digits_.empty() || digits_.back() != 0U);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::front_() const -> base_int_type {
    JMATHS_FUNCTION_TO_LOG;

    if (digits_.empty()) { return 0U; }

    return digits_.front();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::handle_str_(std::string_view num_str,
                                                                    unsigned base) {
    JMATHS_FUNCTION_TO_LOG;

    // the naive yet faster check:
    // if (num_str.empty()) { return; }
    // if (num_str.size() == 1U && num_str.front() == '0') { return; }

    // the more thorough but slower check:
    if (std::ranges::all_of(num_str, [](char c) static noexcept { return c == '0'; })) { return; }

    assert(!num_str.empty());

    for (auto cit = num_str.cbegin(); &*cit != &num_str.back(); ++cit) {
        opr_add_assign_(impl::base_converter<base_int_type>(*cit));
        opr_mult_assign_(base);
    }

    opr_add_assign_(impl::base_converter<base_int_type>(num_str.back()));
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_N<BaseInt, BaseIntBig, Allocator>::bit_(bitpos_t pos) const {
    JMATHS_FUNCTION_TO_LOG;

    const std::size_t pos_whole = pos / base_int_type_bits;
    const bitpos_t pos_mod = pos % base_int_type_bits;

    if (pos_whole < digits_.size()) {
        return (digits_[pos_whole] >> pos_mod) & 1U;
    } else {
        return 0;
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::bit_(bitpos_t pos, bool val) {
    JMATHS_FUNCTION_TO_LOG;

    const std::size_t pos_whole = pos / base_int_type_bits;
    const bitpos_t pos_mod = pos % base_int_type_bits;

    if (is_zero()) {
        if (val == 0) { return; }
        digits_.reserve(pos_whole + 1U);
        digits_.insert(digits_.begin(), pos_whole, 0U);
        digits_.emplace_back(static_cast<base_int_type>(1) << pos_mod);
    } else {
        if (pos_whole < digits_.size()) {
            const base_int_type bitmask = static_cast<base_int_type>(1) << pos_mod;
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
            digits_.emplace_back(static_cast<base_int_type>(1) << pos_mod);
        }
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::size_t basic_N<BaseInt, BaseIntBig, Allocator>::dynamic_size_() const {
    JMATHS_FUNCTION_TO_LOG;

    return digits_.size() * base_int_type_size;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_incr_() {
    JMATHS_FUNCTION_TO_LOG;

    for (auto & digit : digits_) {
        if (digit++ < max_digit) { return; }
    }

    digits_.emplace_back(1);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_decr_() {
    JMATHS_FUNCTION_TO_LOG;

    for (auto & digit : digits_) {
        if (digit-- > 0U) {
            remove_leading_zeroes_();
            return;
        }
    }

    remove_leading_zeroes_();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_add_assign_(const basic_N & rhs) {
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

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_subtr_assign_(const basic_N & rhs) {
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

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_mult_assign_(const basic_N & rhs) {
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

    basic_N product;

    // max storage surplus would be base_int_size
    product.digits_.reserve(this->digits_.size() + rhs.digits_.size());

    for (std::size_t i = 0U; i < rhs.digits_.size(); ++i) {
        basic_N temp1;
        temp1.digits_.reserve(i + this->digits_.size() + 1U);
        temp1.digits_.insert(temp1.digits_.begin(), i, 0U);
        base_int_type carry = 0U;
        for (std::size_t j = 0U; j < this->digits_.size(); ++j) {
            const base_int_big_type temp2 = static_cast<base_int_big_type>(rhs.digits_[i]) *
                                            static_cast<base_int_big_type>(this->digits_[j]);
            const base_int_type temp3 = static_cast<base_int_type>(temp2);
            temp1.digits_.emplace_back(carry + temp3);
            const base_int_type temp_carry = static_cast<base_int_type>(
                (static_cast<base_int_big_type>(carry) + static_cast<base_int_big_type>(temp3)) >>
                base_int_type_bits);
            carry = static_cast<base_int_type>((temp2 >> base_int_type_bits) + temp_carry);
        }

        if (carry != 0U) { temp1.digits_.emplace_back(carry); }
        product.opr_add_assign_(temp1);
    }

    operator=(std::move(product));
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_and_assign_(const basic_N & rhs) {
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

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_or_assign_(const basic_N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_zero()) {
        operator=(rhs);
        return;
    }

    if (rhs.is_zero()) { return; }

    const basic_N * longest;
    const basic_N * shortest;

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

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_xor_assign_(const basic_N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (this->is_zero()) {
        operator=(rhs);
        return;
    }
    if (rhs.is_zero()) { return; }

    const basic_N * longest;
    const basic_N * shortest;

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

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::opr_compl_() const -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    // this function returns 0 if *this == 0

    basic_N inverted;

    inverted.digits_.reserve(digits_.size());

    for (const auto & digit : digits_) {
        inverted.digits_.emplace_back(~digit);
    }

    inverted.remove_leading_zeroes_();

    return inverted;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::opr_bitshift_l_(bitcount_t pos) const
    -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return basic_N{}; }
    if (pos == 0U) { return *this; }

    const std::size_t pos_whole = pos / base_int_type_bits;
    const bitpos_t pos_mod = pos % base_int_type_bits;

    basic_N shifted;

    shifted.digits_.reserve(digits_.size() + pos_whole + 1U);

    shifted.digits_.insert(shifted.digits_.begin(), pos_whole, 0U);

    if (pos_mod == 0U) {
        shifted.digits_.insert(shifted.digits_.end(), digits_.cbegin(), digits_.cend());
    } else {
        shifted.digits_.emplace_back(digits_.front() << pos_mod);

        for (std::size_t i = 1U; i < digits_.size(); ++i) {
            shifted.digits_.emplace_back((digits_[i - 1U] >> (base_int_type_bits - pos_mod)) +
                                         (digits_[i] << pos_mod));
        }

        shifted.digits_.emplace_back(digits_.back() >> (base_int_type_bits - pos_mod));

        shifted.remove_leading_zeroes_();
    }

    return shifted;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::opr_bitshift_r_(bitcount_t pos) const
    -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return basic_N{}; }
    if (pos == 0U) { return *this; }

    const std::size_t pos_whole = pos / base_int_type_bits;

    if (pos_whole >= digits_.size()) { return basic_N{}; }

    const bitpos_t pos_mod = pos % base_int_type_bits;

    basic_N shifted;

    shifted.digits_.reserve(digits_.size() - pos_whole);

    if (pos_mod == 0U) {
        shifted.digits_.insert(
            shifted.digits_.end(),
            digits_.cbegin() + static_cast<typename decltype(digits_)::difference_type>(pos_whole),
            digits_.cend());
    } else {
        for (std::size_t i = pos_whole; i < digits_.size() - 1U; ++i) {
            shifted.digits_.emplace_back((digits_[i] >> pos_mod) +
                                         (digits_[i + 1U] << (base_int_type_bits - pos_mod)));
        }

        shifted.digits_.emplace_back(digits_.back() >> pos_mod);

        shifted.remove_leading_zeroes_();
    }

    return shifted;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_bitshift_l_assign_(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return; }
    if (pos == 0U) { return; }

    const std::size_t pos_whole = pos / base_int_type_bits;
    const bitpos_t pos_mod = pos % base_int_type_bits;

    digits_.reserve(digits_.size() + pos_whole + 1U);

    if (pos_mod != 0U) {
        base_int_type previous = digits_.front();

        digits_.front() <<= pos_mod;

        for (std::size_t i = 1U; i < digits_.size(); ++i) {
            const base_int_type current = digits_[i];
            digits_[i] = (previous >> (base_int_type_bits - pos_mod)) | (digits_[i] << pos_mod);
            previous = current;
        }

        digits_.emplace_back(previous >> (base_int_type_bits - pos_mod));

        remove_leading_zeroes_();
    }

    digits_.insert(digits_.begin(), pos_whole, 0U);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_bitshift_r_assign_(bitcount_t pos) {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return; }
    if (pos == 0U) { return; }

    const std::size_t pos_whole = pos / base_int_type_bits;

    if (pos_whole >= digits_.size()) {
        set_zero();
        return;
    }

    digits_.erase(
        digits_.begin(),
        digits_.begin() + static_cast<typename decltype(digits_)::difference_type>(pos_whole));

    if (const bitpos_t pos_mod = pos % base_int_type_bits; pos_mod != 0U) {
        for (std::size_t i = 0U; i < digits_.size() - 1U; ++i) {
            digits_[i] =
                (digits_[i] >> pos_mod) + (digits_[i + 1U] << (base_int_type_bits - pos_mod));
        }

        digits_.back() >>= pos_mod;

        remove_leading_zeroes_();
    }
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::opr_assign_(std::string_view num_str) {
    JMATHS_FUNCTION_TO_LOG;

    set_zero();

    handle_str_(num_str, default_base);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_N<BaseInt, BaseIntBig, Allocator>::basic_N() = default;

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_N<BaseInt, BaseIntBig, Allocator>::basic_N(std::string_view num_str, unsigned base) {
    JMATHS_FUNCTION_TO_LOG;

    error::invalid_base::check(base);

    handle_str_(num_str, base);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_N<BaseInt, BaseIntBig, Allocator>::is_zero() const {
    JMATHS_FUNCTION_TO_LOG;

    return digits_.empty();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_N<BaseInt, BaseIntBig, Allocator>::is_one() const {
    JMATHS_FUNCTION_TO_LOG;

    return digits_.size() == 1U && digits_.front() == 1U;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_N<BaseInt, BaseIntBig, Allocator>::is_even() const {
    JMATHS_FUNCTION_TO_LOG;

    return !is_odd();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bool basic_N<BaseInt, BaseIntBig, Allocator>::is_odd() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return false; }

    return digits_.front() & 1U;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bitcount_t basic_N<BaseInt, BaseIntBig, Allocator>::ctz() const {
    JMATHS_FUNCTION_TO_LOG;

    bitcount_t tz = 0U;
    for (const auto & digit : digits_) {
        if (digit != 0U) {
            tz += static_cast<bitcount_t>(std::countr_zero(digit));
            break;
        }

        tz += base_int_type_bits;
    }

    return tz;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr bitcount_t basic_N<BaseInt, BaseIntBig, Allocator>::bits() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) { return 1U; }
    return digits_.size() * base_int_type_bits -
           static_cast<bitcount_t>(std::countl_zero(digits_.back()));
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::size_t basic_N<BaseInt, BaseIntBig, Allocator>::size() const {
    JMATHS_FUNCTION_TO_LOG;

    return sizeof(*this) + dynamic_size_();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_N<BaseInt, BaseIntBig, Allocator>::to_str(unsigned base) const {
    JMATHS_FUNCTION_TO_LOG;

    error::invalid_base::check(base);

    return conv_to_base_(base);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_N<BaseInt, BaseIntBig, Allocator>::to_hex() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) {
        static const /*expr*/ std::string zero = "0";
        return zero;
    }

    std::ostringstream oss;

    oss.setf(std::ios_base::hex, std::ios_base::basefield);
    oss.setf(std::ios_base::uppercase);

    auto crit = digits_.crbegin();

    oss << *crit;

    oss.setf(std::ios_base::right, std::ios_base::adjustfield);
    static constexpr auto field_width = base_int_type_bits / 4U;
    // ReSharper disable once CppRedundantCastExpression
    oss.width(static_cast<std::streamsize>(field_width));
    oss.fill('0');

    while (++crit != digits_.crend()) {
        oss << *crit;
    }

    return std::move(oss).str();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr std::string basic_N<BaseInt, BaseIntBig, Allocator>::to_bin() const {
    JMATHS_FUNCTION_TO_LOG;

    if (is_zero()) {
        static const /*expr*/ std::string zero = "0";
        return zero;
    }

    using bitset_type = std::bitset<base_int_type_size * bits_in_byte>;

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

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_N<BaseInt, BaseIntBig, Allocator>::operator bool() const {
    JMATHS_FUNCTION_TO_LOG;

    return !is_zero();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator[](bitpos_t pos) & -> bit_reference {
    JMATHS_FUNCTION_TO_LOG;

    return {*this, pos};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator[](
    bitpos_t pos) const & -> const_bit_reference {
    JMATHS_FUNCTION_TO_LOG;

    return {*this, pos};
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr void basic_N<BaseInt, BaseIntBig, Allocator>::set_zero() {
    JMATHS_FUNCTION_TO_LOG;

    digits_.clear();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator++() -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_incr_();
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator--() -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_decr_();
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator+=(const basic_N & rhs)
    -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_add_assign_(rhs);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator-=(const basic_N & rhs)
    -> basic_N & {
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

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator*=(const basic_N & rhs)
    -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_mult_assign_(rhs);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator&=(const basic_N & rhs)
    -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_and_assign_(rhs);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator|=(const basic_N & rhs)
    -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_or_assign_(rhs);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator^=(const basic_N & rhs)
    -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_xor_assign_(rhs);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator~() const -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    return opr_compl_();
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator<<(bitcount_t pos) const -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    return opr_bitshift_l_(pos);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator>>(bitcount_t pos) const -> basic_N {
    JMATHS_FUNCTION_TO_LOG;

    return opr_bitshift_r_(pos);
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator<<=(bitcount_t pos) -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_bitshift_l_assign_(pos);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator>>=(bitcount_t pos) -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_bitshift_r_assign_(pos);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::operator=(std::string_view num_str)
    -> basic_N & {
    JMATHS_FUNCTION_TO_LOG;

    opr_assign_(num_str);
    return *this;
}

}  // namespace jmaths

// member functions of N::bit_reference_base_ and derived classes
namespace jmaths {

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference::bit_reference(
    const bit_reference & ref) = default;

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_N<BaseInt, BaseIntBig, Allocator>::const_bit_reference::const_bit_reference(
    const const_bit_reference & ref) = default;

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr basic_N<BaseInt, BaseIntBig, Allocator>::const_bit_reference::const_bit_reference(
    const bit_reference & ref) :
    bit_reference_base_<const basic_N>(ref.num_, ref.pos_) {
    JMATHS_FUNCTION_TO_LOG;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference::operator=(bool val)
    -> bit_reference & {
    JMATHS_FUNCTION_TO_LOG;

    this->num_.bit_(this->pos_, val);
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference::operator=(
    const bit_reference & ref) -> bit_reference & {
    JMATHS_FUNCTION_TO_LOG;

    this->num_.bit_(this->pos_, ref.num_.bit_(ref.pos_));
    return *this;
}

template <typename BaseInt, typename BaseIntBig, typename Allocator>
constexpr auto basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference::operator=(
    const const_bit_reference & ref) -> bit_reference & {
    JMATHS_FUNCTION_TO_LOG;

    this->num_.bit_(this->pos_, ref.num_.bit_(ref.pos_));
    return *this;
}

}  // namespace jmaths
