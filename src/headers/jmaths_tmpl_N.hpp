#pragma once

#include <type_traits>
#include <compare>
#include <cstddef>
#include <optional>

#include "jmaths_constants_and_types.hpp"
#include "jmaths_N.hpp"
#include "jmaths_detail.hpp"

#include "jmaths_def.hh"

// comparison functions for N with integral types
namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
bool detail::opr_eq (const N & lhs, INT rhs) {
	FUNCTION_TO_LOG;

	if (rhs == 0) return lhs.is_zero();
	if (lhs.is_zero()) return false;

	if (lhs.digits_.size() * base_int_size - 1 > sizeof(rhs)) return false;

	if (lhs.digits_.size() * base_int_size < sizeof(rhs) && (rhs >> (base_int_bits * lhs.digits_.size())) != 0) return false;

	for (const auto & digit : lhs.digits_) {
		if (digit != (base_int)rhs) return false;
		REPEAT(base_int_size) rhs >>= bits_in_byte;
	}

	return true;
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering detail::opr_comp (const N & lhs, INT rhs) {
	FUNCTION_TO_LOG;

	if (rhs == 0) return lhs.is_zero() ? std::strong_ordering::equal : std::strong_ordering::greater;
	if (lhs.is_zero()) return std::strong_ordering::less;

	if (lhs.digits_.size() * base_int_size - 1 > sizeof(rhs)) return std::strong_ordering::greater;

	if (lhs.digits_.size() * base_int_size < sizeof(rhs) && (rhs >> (base_int_bits * lhs.digits_.size())) != 0) return std::strong_ordering::less;

	std::size_t pos = 1;
	for (auto crit = lhs.digits_.crbegin(); crit != lhs.digits_.crend(); ++crit) {
		INT curr_byte = rhs;
		REPEAT(base_int_size * (lhs.digits_.size() - pos)) curr_byte >>= bits_in_byte;
		if (*crit < (base_int)curr_byte) return std::strong_ordering::less;
		if (*crit > (base_int)curr_byte) return std::strong_ordering::greater;

		++pos;
	}

	return std::strong_ordering::equal;
}

template <typename INT>
requires std::is_integral_v<INT>
bool operator == (const N & lhs, INT rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_eq(lhs, rhs);
}

template <typename INT>
requires std::is_integral_v<INT>
bool operator == (INT lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_eq(rhs, lhs);
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering operator <=> (const N & lhs, INT rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_comp(lhs, rhs);
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering operator <=> (INT lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	return (0 <=> detail::opr_comp(rhs, lhs));
}

} // /namespace jmaths

// member function templates of N
namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
void N::opr_assign_ (INT rhs) {
	FUNCTION_TO_LOG;

	digits_.clear();

	digits_.reserve((std::size_t)((double)sizeof(INT)/base_int_size - max_ratio + 1));

	// this assumes that sizeof(smallest type)/base_int_size >= max_ratio
	REPEAT((std::size_t)((double)sizeof(INT)/base_int_size - max_ratio)) {
		digits_.emplace_back((base_int)rhs);
		REPEAT(base_int_size) rhs >>= bits_in_byte;
	}

	digits_.emplace_back((base_int)rhs);

	remove_leading_zeroes_();
}

template <typename INT>
requires std::is_integral_v<INT>
N::N (INT num) {
	FUNCTION_TO_LOG;

	if constexpr (sizeof(INT) > 1) {
		digits_.reserve((std::size_t)((double)sizeof(INT)/base_int_size - max_ratio + 1));

		// this assumes that sizeof(smallest type)/base_int_size >= max_ratio
		REPEAT((std::size_t)((double)sizeof(INT)/base_int_size - max_ratio)) {
			digits_.emplace_back((base_int)num);
			REPEAT(base_int_size) num >>= bits_in_byte;
		}

	} else {}

	digits_.emplace_back((base_int)num);

	remove_leading_zeroes_();

	assert(digits_.empty() || digits_.back() != 0);
}

template <typename INT>
requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
std::optional<INT> N::fits_into() const {
	FUNCTION_TO_LOG;

	#if 0
	if constexpr (std::is_signed_v<INT>) {
		if (is_positive()) return N::fits_into<INT>();
		else return std::nullopt;
		// !!! implement this perhaps ???
	}
	#endif

	if (is_zero()) return 0;

	if constexpr (sizeof(INT) < base_int_size) {
		if ((INT)digits_.front() != digits_.front() || digits_.size() > 1) return std::nullopt;
		return digits_.front();
	} else {
		if (digits_.size() * base_int_size > sizeof(INT)) return std::nullopt;

		#if 0
		INT converted (digits_.back());
		for (std::size_t i = digits_.size() - 1; i --> 0;) {
			converted <<= base_int_bits;
		  	converted |= digits_[i];
		}
		#endif

		INT converted (digits_.front());

		for (std::size_t i = 1; i < digits_.size(); ++i) {
			converted |= ((INT)digits_[i] << (base_int_bits * i));
		}

		return converted;
	}
}

template <typename INT>
requires std::is_integral_v<INT>
N & N::operator = (INT rhs) {
	FUNCTION_TO_LOG;

	opr_assign_(rhs);
	return *this;
}

} // /namespace jmaths

// functions for N::bit_reference_base_
namespace jmaths {

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
N::bit_reference_base_<T>::bit_reference_base_ (T & num, bit_type pos) : num_(num), pos_(pos) {
	FUNCTION_TO_LOG;
}

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
N::bit_reference_base_<T>::operator bool () const {
	FUNCTION_TO_LOG;

	return num_.bit_(pos_);
}

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
N::bit_reference_base_<T>::operator int () const {
	FUNCTION_TO_LOG;

	return operator bool();
}

} // /namespace jmaths

#include "jmaths_undef.hh"
