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

#include <bit>
#include <cassert>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstddef>
#include <def.hh>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>

#include "N.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "detail.hpp"

// comparison functions for N with integral types
namespace jmaths {

bool detail::opr_eq(const N & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit == rhs;
    }

    return false;
}

std::strong_ordering detail::opr_comp(const N & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    if (const auto try_and_fit = lhs.fits_into<decltype(rhs)>(); try_and_fit.has_value()) {
        return *try_and_fit <=> rhs;
    }

    return std::strong_ordering::greater;
}

bool operator==(const N & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_eq(lhs, rhs);
}

bool operator==(std::integral auto lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_eq(rhs, lhs);
}

std::strong_ordering operator<=>(const N & lhs, std::integral auto rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return detail::opr_comp(lhs, rhs);
}

std::strong_ordering operator<=>(std::integral auto lhs, const N & rhs) {
    JMATHS_FUNCTION_TO_LOG;

    return 0 <=> detail::opr_comp(rhs, lhs);
}

}  // namespace jmaths

// member function templates of N
namespace jmaths {

void N::handle_int_(std::integral auto num) {
    JMATHS_FUNCTION_TO_LOG;

    if constexpr (base_int_size < sizeof(num)) {
        static constexpr std::size_t digits_needed = std::ceil((long double)sizeof(num) / base_int_size);

        digits_.reserve(digits_needed);

        for (std::size_t curr_byte = 0; curr_byte < sizeof(num); ++curr_byte) {
            const std::size_t curr_index = curr_byte / base_int_size;
            const std::size_t curr_offset = curr_byte % base_int_size;
            if (curr_offset == 0) { digits_.emplace_back(); }

            static constexpr unsigned bitmask = ~(unsigned char)0;

            digits_[curr_index] |= (num & bitmask) << (curr_offset * bits_in_byte);

            num >>= bits_in_byte;
        }

        assert(num == 0);
    } else {
        digits_.emplace_back((base_int)num);
    }

    remove_leading_zeroes_();
}

template <std::unsigned_integral T> T N::fit_into_(std::size_t max_byte) const {
    JMATHS_FUNCTION_TO_LOG;

    T converted = 0;

    for (std::size_t curr_byte = 0; curr_byte < max_byte; ++curr_byte) {
        const std::size_t curr_index = curr_byte / base_int_size;
        const std::size_t curr_offset = curr_byte % base_int_size;

        static constexpr unsigned bitmask = ~(unsigned char)0;

        const auto relevant_byte = (digits_[curr_index] >> (curr_offset * bits_in_byte)) & bitmask;

        converted |= ((T)relevant_byte << (curr_byte * bits_in_byte));
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

    if (is_zero()) return 0;

    if constexpr (base_int_size < sizeof(T)) {
        if (digits_.size() * base_int_size < sizeof(T)) { return fit_into_<T>(digits_.size() * base_int_size); }

        if (digits_.size() * base_int_size > sizeof(T)) {
            if ((unsigned)std::countr_zero(digits_.back()) <
                (digits_.size() * base_int_size - sizeof(T)) * bits_in_byte) {
                return std::nullopt;
            }
        }

        return fit_into_<T>(sizeof(T));
    } else if constexpr (base_int_size > sizeof(T)) {
        if (digits_.size() > 1) return std::nullopt;

        static constexpr auto bitmask = ~(T)0;

        if (const auto masked_digit = digits_.front() & bitmask; masked_digit == digits_.front()) {
            return masked_digit;
        }

        return std::nullopt;
    } else {
        if (digits_.size() > 1) { return std::nullopt; }

        return digits_.front();
    }
}

template <std::signed_integral T> std::optional<T> N::fits_into() const {
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

template <typename T>
    requires std::same_as<N, std::decay_t<T>>
N::bit_reference_base_<T>::bit_reference_base_(T & num, bitpos_t pos) : num_(num), pos_{pos} {
    JMATHS_FUNCTION_TO_LOG;
}

template <typename T>
    requires std::same_as<N, std::decay_t<T>>
N::bit_reference_base_<T>::operator bool() const {
    JMATHS_FUNCTION_TO_LOG;

    return num_.bit_(pos_);
}

template <typename T>
    requires std::same_as<N, std::decay_t<T>>
N::bit_reference_base_<T>::operator int() const {
    JMATHS_FUNCTION_TO_LOG;

    return operator bool();
}

}  // namespace jmaths
