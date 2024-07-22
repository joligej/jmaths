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
#include <cstddef>
#include <def.hh>
#include <optional>
#include <type_traits>
#include <utility>

#include "N.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "detail.hpp"

// comparison functions for N with integral types
namespace jmaths {

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
bool detail::opr_eq(const N & lhs, INT rhs) {
    FUNCTION_TO_LOG;

#if 0

    if (rhs == 0) return lhs.is_zero();
    if (lhs.is_zero()) return false;

    if ((lhs.digits_.size() - 1) * base_int_size + 1 > sizeof(rhs))
        return false;

    if (lhs.digits_.size() * base_int_size < sizeof(rhs) &&
        (rhs >> (base_int_bits * lhs.digits_.size())) != 0)
        return false;

    for (const auto & digit : lhs.digits_) {
        if (digit != (base_int)rhs) return false;

        if constexpr (sizeof(rhs) > base_int_size) {
            rhs >>= base_int_bits;
        } else {
        }
    }

    return true;

#endif

    if (const auto try_and_fit = lhs.fits_into<INT>();
        try_and_fit.has_value()) {
        return (*try_and_fit == rhs);
    } else {
        return false;
    }

#if 0

    if constexpr (base_int_size < sizeof(rhs)) {
        if (lhs.digits_.size() * base_int_size < sizeof(rhs)) {

        } else if (lhs.digits_.size() * base_int_size > sizeof(rhs)) {

        } else {

        }
    } else if constexpr (base_int_size > sizeof(rhs)) {
        if (lhs.digits_.size() * base_int_size < sizeof(rhs)) {

        } else if (lhs.digits_.size() * base_int_size > sizeof(rhs)) {

        } else {

        }
    } else {
        if (lhs.digits_.size() * base_int_size < sizeof(rhs)) {

        } else if (lhs.digits_.size() * base_int_size > sizeof(rhs)) {

        } else {
            return (lhs.digits_.front() == rhs);
        }
    }

#endif
}

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
std::strong_ordering detail::opr_comp(const N & lhs, INT rhs) {
    FUNCTION_TO_LOG;

#if 0

    if (rhs == 0)
        return lhs.is_zero() ? std::strong_ordering::equal :
                               std::strong_ordering::greater;
    if (lhs.is_zero()) return std::strong_ordering::less;

    if ((lhs.digits_.size() - 1) * base_int_size + 1 > sizeof(rhs))
        return std::strong_ordering::greater;

    if (lhs.digits_.size() * base_int_size < sizeof(rhs) &&
        (rhs >> (base_int_bits * lhs.digits_.size())) != 0)
        return std::strong_ordering::less;

    std::size_t pos = 1;
    for (auto crit = lhs.digits_.crbegin(); crit != lhs.digits_.crend();
         ++crit) {
        INT curr_byte = rhs;
        REPEAT(base_int_size * (lhs.digits_.size() - pos)) {
            curr_byte >>= bits_in_byte;
        }

        if (*crit < (base_int)curr_byte) return std::strong_ordering::less;
        if (*crit > (base_int)curr_byte) return std::strong_ordering::greater;

        ++pos;
    }

    return std::strong_ordering::equal;

#endif

    if (const auto try_and_fit = lhs.fits_into<INT>();
        try_and_fit.has_value()) {
        return (*try_and_fit <=> rhs);
    } else {
        return std::strong_ordering::greater;
    }
}

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
bool operator==(const N & lhs, INT rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_eq(lhs, rhs);
}

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
bool operator==(INT lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_eq(rhs, lhs);
}

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
std::strong_ordering operator<=>(const N & lhs, INT rhs) {
    FUNCTION_TO_LOG;

    return detail::opr_comp(lhs, rhs);
}

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
std::strong_ordering operator<=>(INT lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    return (0 <=> detail::opr_comp(rhs, lhs));
}

}  // namespace jmaths

// member function templates of N
namespace jmaths {

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
void N::handle_int_(INT num) {
    FUNCTION_TO_LOG;

    if constexpr (base_int_size < sizeof(num)) {
        static constexpr std::size_t digits_needed =
            std::ceil((long double)sizeof(num) / base_int_size);

        digits_.reserve(digits_needed);

        for (std::size_t curr_byte = 0; curr_byte < sizeof(num); ++curr_byte) {
            const std::size_t curr_index = curr_byte / base_int_size;
            const std::size_t curr_offset = curr_byte % base_int_size;
            if (curr_offset == 0) { digits_.emplace_back(); }

            static constexpr unsigned bitmask = ~(unsigned char)0;

            digits_[curr_index] |= (num & bitmask)
                                   << (curr_offset * bits_in_byte);

            num >>= bits_in_byte;
        }

        assert(num == 0);
    } else {
        digits_.emplace_back((base_int)num);
    }

    remove_leading_zeroes_();
}

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
INT N::fit_into_(std::size_t max_byte) const {
    INT converted = 0;

    for (std::size_t curr_byte = 0; curr_byte < max_byte; ++curr_byte) {
        const std::size_t curr_index = curr_byte / base_int_size;
        const std::size_t curr_offset = curr_byte % base_int_size;

        static constexpr unsigned bitmask = ~(unsigned char)0;

        const auto relevant_byte =
            (digits_[curr_index] >> (curr_offset * bits_in_byte)) & bitmask;

        converted |= ((INT)relevant_byte << (curr_byte * bits_in_byte));
    }

    return converted;
}

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
void N::opr_assign_(INT rhs) {
    FUNCTION_TO_LOG;

    digits_.clear();
    handle_int_(rhs);
}

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
N::N(INT num) {
    FUNCTION_TO_LOG;

    handle_int_(num);
}

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
std::optional<INT> N::fits_into() const {
    FUNCTION_TO_LOG;

    if (is_zero()) return 0;

    if constexpr (base_int_size < sizeof(INT)) {
        if (digits_.size() * base_int_size < sizeof(INT)) {
            return fit_into_<INT>(digits_.size() * base_int_size);
        }

        if (digits_.size() * base_int_size > sizeof(INT)) {
            if ((unsigned)std::countr_zero(digits_.back()) <
                (digits_.size() * base_int_size - sizeof(INT)) * bits_in_byte) {
                return std::nullopt;
            }
        }

        return fit_into_<INT>(sizeof(INT));
    } else if constexpr (base_int_size > sizeof(INT)) {
        if (digits_.size() > 1) return std::nullopt;

        static constexpr auto bitmask = ~(INT)0;

        if (const auto masked_digit = digits_.front() & bitmask;
            masked_digit == digits_.front()) {
            return masked_digit;
        }

        return std::nullopt;
    } else {
        if (digits_.size() > 1) { return std::nullopt; }

        return digits_.front();
    }
}

template <typename INT>
    requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
N & N::operator=(INT rhs) {
    FUNCTION_TO_LOG;

    opr_assign_(rhs);
    return *this;
}

}  // namespace jmaths

// functions for N::bit_reference_base_
namespace jmaths {

template <typename T>
    requires std::is_same_v<N, std::decay_t<T>>
N::bit_reference_base_<T>::bit_reference_base_(T & num, bit_type pos) :
    num_(num), pos_(pos) {
    FUNCTION_TO_LOG;
}

template <typename T>
    requires std::is_same_v<N, std::decay_t<T>>
N::bit_reference_base_<T>::operator bool() const {
    FUNCTION_TO_LOG;

    return num_.bit_(pos_);
}

template <typename T>
    requires std::is_same_v<N, std::decay_t<T>>
N::bit_reference_base_<T>::operator int() const {
    FUNCTION_TO_LOG;

    return operator bool();
}

}  // namespace jmaths

#include "undef.hh"
