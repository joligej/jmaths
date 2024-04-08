#include "jmaths_def.cfg"

#define REPEAT(x) for (decltype(x) for_loop_repition_counter_ = 0; for_loop_repition_counter_ < x; ++for_loop_repition_counter_)

namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
bool detail::opr_eq (const N & lhs, INT rhs) {
	if (rhs == 0) return lhs.is_zero();
	if (lhs.is_zero()) return false;
	
	if (lhs.digits_.size() * BASE_INT_SIZE - 1 > sizeof(rhs)) return false;
	
	if (lhs.digits_.size() * BASE_INT_SIZE < sizeof(rhs) && (rhs >> (BASE_INT_BITS * lhs.digits_.size())) != 0) return false;
	
	for (const auto & digit : lhs.digits_) {
		if (digit != (BASE_INT)rhs) return false;
		REPEAT(BASE_INT_SIZE) rhs >>= BITS_IN_BYTE;
	}
	
	return true;
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering detail::opr_comp (const N & lhs, INT rhs) {
	if (rhs == 0) return lhs.is_zero() ? std::strong_ordering::equal : std::strong_ordering::greater;
	if (lhs.is_zero()) return std::strong_ordering::less;
	
	if (lhs.digits_.size() * BASE_INT_SIZE - 1 > sizeof(rhs)) return std::strong_ordering::greater;
	
	if (lhs.digits_.size() * BASE_INT_SIZE < sizeof(rhs) && (rhs >> (BASE_INT_BITS * lhs.digits_.size())) != 0) return std::strong_ordering::less;
	
	size_t pos = 1;
	for (auto crit = lhs.digits_.crbegin(); crit != lhs.digits_.crend(); ++crit) {
		INT curr_byte = rhs;
		REPEAT(BASE_INT_SIZE * (lhs.digits_.size() - pos)) curr_byte >>= BITS_IN_BYTE;
		if (*crit < (BASE_INT)curr_byte) return std::strong_ordering::less;
		if (*crit > (BASE_INT)curr_byte) return std::strong_ordering::greater;
		
		++pos;
	}
	
	return std::strong_ordering::equal;
}

template <typename INT>
requires std::is_integral_v<INT>
bool operator == (const N & lhs, INT rhs) {
	return detail::opr_eq(lhs, rhs);
}

template <typename INT>
requires std::is_integral_v<INT>
bool operator == (INT lhs, const N & rhs) {
	return detail::opr_eq(rhs, lhs);
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering operator <=> (const N & lhs, INT rhs) {
	return detail::opr_comp(lhs, rhs);
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering operator <=> (INT lhs, const N & rhs) {
	return (0 <=> detail::opr_comp(rhs, lhs));
}

} // /namespace jmaths

namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
bool operator == (const Z & lhs, INT rhs) {
	if (rhs < 0) {
		if (lhs.is_positive()) return false;
		return (lhs.abs() == -rhs);
	}
	
	return (lhs.abs() == rhs);
}
	
template <typename INT>
requires std::is_integral_v<INT>
bool operator == (INT lhs, const Z & rhs) {
	return (rhs == lhs);
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering operator <=> (const Z & lhs, INT rhs) {
	if (lhs.is_positive()) {
		if (rhs >= 0) {
			return (lhs.abs() <=> rhs);
		} else {
			return std::strong_ordering::greater;
		}
	} else {
		if (rhs >= 0) {
			return std::strong_ordering::less;
		} else {
			return (-rhs <=> lhs.abs());
		}
	}
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering operator <=> (INT lhs, const Z & rhs) {
	return (0 <=> (rhs <=> lhs));
}

} // /namespace jmaths

namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
void N::opr_assign_ (INT rhs) {
	digits_.clear();
		
	digits_.reserve((size_t)((double)sizeof(INT)/BASE_INT_SIZE - MAX_RATIO + 1));
	
	// this assumes that sizeof(smallest type)/BASE_INT_SIZE >= MAX_RATIO
	REPEAT((size_t)((double)sizeof(INT)/BASE_INT_SIZE - MAX_RATIO)) {
		digits_.emplace_back((BASE_INT)rhs);
		REPEAT(BASE_INT_SIZE) rhs >>= BITS_IN_BYTE;
	}
	
	digits_.emplace_back((BASE_INT)rhs);
	
	remove_leading_zeroes_();
}

template <typename INT>
requires std::is_integral_v<INT>
N::N (INT num) {
	digits_.reserve((size_t)((double)sizeof(INT)/BASE_INT_SIZE - MAX_RATIO + 1));
	
	// this assumes that sizeof(smallest type)/BASE_INT_SIZE >= MAX_RATIO
	REPEAT((size_t)((double)sizeof(INT)/BASE_INT_SIZE - MAX_RATIO)) {
		digits_.emplace_back((BASE_INT)num);
		REPEAT(BASE_INT_SIZE) num >>= BITS_IN_BYTE;
	}
	
	digits_.emplace_back((BASE_INT)num);
	
	remove_leading_zeroes_();
}

template <typename INT>
requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
std::optional<INT> N::fits_into() const {
	// !!! perhaps use std::numeric_limits<T>::max from <limits> for signed types ???
	
	/*if constexpr (std::is_signed_v<INT>) {
		//if (is_positive()) return N::fits_into<INT>();
		//else return std::nullopt;
		// !!! implement this perhaps ???
	}*/
	
	if (is_zero()) return 0;
	
	if constexpr (sizeof(INT) < BASE_INT_SIZE) {
		if ((INT)digits_.front() != digits_.front() || digits_.size() > 1) return std::nullopt;
		return digits_.front();
	} else {
		if (digits_.size() * BASE_INT_SIZE > sizeof(INT)) return std::nullopt;
		
		/*INT converted (digits_.back());
		for (size_t i = digits_.size() - 1; i --> 0;) {
			converted <<= BASE_INT_BITS;
		  	converted |= digits_[i];
		}*/
	
		INT converted (digits_.front());
	
		for (size_t i = 1; i < digits_.size(); ++i) {
			converted |= ((INT)digits_[i] << (BASE_INT_BITS * i));
		}
	
		return converted;	
	}
}

template <typename INT>
requires std::is_integral_v<INT>
N & N::operator = (INT rhs) {
	opr_assign_(rhs);
	return *this;
}

} // /namespace jmaths

namespace jmaths {

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
N::bit_reference_base_<T>::bit_reference_base_ (T & num, BIT_TYPE pos) : num_(num), pos_(pos) {}

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
N::bit_reference_base_<T>::operator bool () const {
	return num_.bit_(pos_);
}

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
N::bit_reference_base_<T>::operator int () const {
	return operator bool();
}

} // /namespace jmaths

namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
sign_type::sign_type (INT & num) : sign_(handle_int_(num)) {}

template <typename INT>
requires std::is_integral_v<INT>
sign_type::sign_bool sign_type::handle_int_ (INT & num) {
	if (num < 0) {
		num *= -1;
		return negative;
	} else {
		return positive;
	}
}

template <typename BOOL>
requires std::is_convertible_v<BOOL, std::underlying_type_t<sign_type::sign_bool>>
void sign_type::set_sign_ (BOOL val) {
	sign_ = static_cast<sign_bool>(val);
}
	
} // /namespace jmaths

namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
Z::Z (INT num) : sign_type(num), N(num) {}

template <typename INT>
requires std::is_integral_v<INT>
std::optional<INT> Z::fits_into() const {
	if constexpr (std::is_unsigned_v<INT>) {
		if (is_positive()) return N::fits_into<INT>();
		else return std::nullopt;
	}
	
	if (is_zero()) return 0;
	
	if constexpr (sizeof(INT) < BASE_INT_SIZE) {
		if (digits_.front() > (is_negative() ? -std::numeric_limits<INT>::min() : std::numeric_limits<INT>::max()) || digits_.size() > 1) return std::nullopt;
		return (is_negative() ? -digits_.front() : digits_.front());
	} else {
		if (digits_.size() * BASE_INT_SIZE > sizeof(INT)) return std::nullopt;
		
		if (detail::opr_comp(*this, (is_negative() ? -std::numeric_limits<INT>::min() : std::numeric_limits<INT>::max())) > 0) return std::nullopt;
		
		/*INT converted (digits_.back());
		for (size_t i = digits_.size() - 1; i --> 0;) {
			converted <<= BASE_INT_BITS;
		  	converted |= digits_[i];
		}*/
	
		INT converted (digits_.front());
	
		for (size_t i = 1; i < digits_.size(); ++i) {
			converted += (INT)((std::make_unsigned_t<INT>)digits_[i] << (BASE_INT_BITS * i));
		}
		
		if (is_negative()) -converted;
		else return converted;	
	}
}

template <typename INT>
requires std::is_integral_v<INT>
Z & Z::operator = (INT rhs) {
	set_sign_(sign_type::handle_int_(rhs));
	N::opr_assign_(rhs);
	return *this;
}
	
} // /namespace jmaths

#undef REPEAT

#include "jmaths_undef.cfg"