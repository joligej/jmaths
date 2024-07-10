#include <ostream>
#include <istream>
#include <utility>
#include <compare>
#include <cstddef>
#include <string>
#include <string_view>

#include "headers/constants_and_types.hpp"
#include "headers/Z.hpp"
#include "headers/N.hpp"
#include "headers/sign_type.hpp"
#include "headers/detail.hpp"

#include "headers/def.hh"

namespace jmaths {

/**********************************************************/
// forwarding functions

std::ostream & operator << (std::ostream & os, const Z & z) {
	FUNCTION_TO_LOG;

	return detail::opr_ins(os, z);
}

std::istream & operator >> (std::istream & is, Z & z) {
	FUNCTION_TO_LOG;

	return detail::opr_extr(is, z);
}

Z operator + (const Z & lhs, const Z & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_add(lhs, rhs);
}

Z operator - (const Z & lhs, const Z & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_subtr(lhs, rhs);
}

Z operator * (const Z & lhs, const Z & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_mult(lhs, rhs);
}

std::pair<Z, Z> operator / (const Z & lhs, const Z & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_div(lhs, rhs);
}

Z operator & (const Z & lhs, const Z & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_and(lhs, rhs);
}

Z operator | (const Z & lhs, const Z & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_or(lhs, rhs);
}

Z operator ^ (const Z & lhs, const Z & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_xor(lhs, rhs);
}

bool operator == (const Z & lhs, const Z & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_eq(lhs, rhs);
}

std::strong_ordering operator <=> (const Z & lhs, const Z & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_comp(lhs, rhs);
}

} // /namespace jmaths

namespace jmaths {

Z::Z (N && n, sign_bool sign) : sign_type(sign), N(std::move(n)) {
	FUNCTION_TO_LOG;
}

Z::Z (const N & n, sign_bool sign) : sign_type(sign), N(n) {
	FUNCTION_TO_LOG;
}

std::size_t Z::dynamic_size_() const {
	FUNCTION_TO_LOG;

	return N::dynamic_size_();
}

Z::Z() = default;

Z::Z (std::string_view num_str, unsigned base) : sign_type(num_str), N(num_str, base) {
	FUNCTION_TO_LOG;

	if (is_zero()) set_sign_(positive);
}

Z::Z (const N & n) : N(n) {
	FUNCTION_TO_LOG;
}

Z::Z (N && n) : N(std::move(n)) {
	FUNCTION_TO_LOG;
}

const N & Z::abs() const & {
	FUNCTION_TO_LOG;

	return static_cast<const N&>(*this);
}

N && Z::abs() && {
	FUNCTION_TO_LOG;

	return static_cast<N&&>(*this);
}

bool Z::is_zero() const {
	FUNCTION_TO_LOG;

	return N::is_zero();
}

bool Z::is_one() const {
	FUNCTION_TO_LOG;

	return (is_positive() && N::is_one());
}

bool Z::is_neg_one() const {
	FUNCTION_TO_LOG;

	return (is_negative() && N::is_one());
}

std::size_t Z::size() const {
	FUNCTION_TO_LOG;

	return (sizeof(*this) + dynamic_size_());
}

std::string Z::to_str (unsigned base) const {
	FUNCTION_TO_LOG;

	if (is_negative()) {
		return negative_sign + N::to_str(base);
	} else {
		return N::to_str(base);
	}
}

std::string Z::to_hex() const {
	FUNCTION_TO_LOG;

	if (is_negative()) {
		return negative_sign + N::to_hex();
	} else {
		return N::to_hex();
	}
}

Z & Z::operator ++ () {
	FUNCTION_TO_LOG;

	if (is_positive()) {
		N::opr_incr_();
	} else {
		if (N::is_one()) set_sign_(positive);
		N::opr_decr_();
	}

	return *this;
}

Z & Z::operator -- () {
	FUNCTION_TO_LOG;

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

Z & Z::operator += (const Z & rhs) {
	FUNCTION_TO_LOG;

	if (this->is_positive()) {
		if (rhs.is_positive()) {
			N::opr_add_assign_(rhs);
		} else {
			const auto difference = detail::opr_comp(this->abs(), rhs.abs());

			if (difference == 0) {
				digits_.clear();
			} else if (difference > 0) {
				N::opr_subtr_assign_(rhs.abs());
			} else {
				N::operator=(detail::opr_subtr(rhs.abs(), this->abs()));
				set_sign_(negative);
			}
		}
	} else {
		if (rhs.is_positive()) {
			const auto difference = detail::opr_comp(this->abs(), rhs.abs());

			if (difference == 0) {
				digits_.clear();
				set_sign_(positive);
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

Z & Z::operator -= (const Z & rhs) {
	FUNCTION_TO_LOG;

	if (this->is_positive()) {
		if (rhs.is_positive()) {
			const auto difference = detail::opr_comp(this->abs(), rhs.abs());

			if (difference == 0) {
				digits_.clear();
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
			const auto difference = detail::opr_comp(this->abs(), rhs.abs());

			if (difference == 0) {
				digits_.clear();
				set_sign_(positive);
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

Z & Z::operator *= (const Z & rhs) {
	FUNCTION_TO_LOG;

	N::opr_mult_assign_(rhs);
	set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
	return *this;
}

Z & Z::operator &= (const Z & rhs) {
	FUNCTION_TO_LOG;

	N::opr_and_assign_(rhs);
	set_sign_(is_zero() ? positive : this->sign_ & rhs.sign_);
	return *this;
}

Z & Z::operator |= (const Z & rhs) {
	FUNCTION_TO_LOG;

	N::opr_or_assign_(rhs);
	set_sign_(is_zero() ? positive : this->sign_ | rhs.sign_);
	return *this;
}

Z & Z::operator ^= (const Z & rhs) {
	FUNCTION_TO_LOG;

	N::opr_xor_assign_(rhs);
	set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
	return *this;
}

Z Z::operator - () const & {
	FUNCTION_TO_LOG;

	if (is_zero()) return *this;
	return Z(abs(), static_cast<sign_bool>(!sign_));
}

Z && Z::operator - () && {
	FUNCTION_TO_LOG;

	flip_sign();
	return std::move(*this);
}

Z Z::operator ~ () const {
	FUNCTION_TO_LOG;

	N && complemented = N::opr_compl_();

	if (complemented.is_zero()) {
		return Z{};
	} else {
		return Z(std::move(complemented), static_cast<sign_bool>(!sign_));
	}
}

Z Z::operator << (bit_type pos) const {
	FUNCTION_TO_LOG;

	return Z(N::opr_bitshift_l_(pos), sign_);
}

Z Z::operator >> (bit_type pos) const {
	FUNCTION_TO_LOG;

	N && shifted = N::opr_bitshift_r_(pos);

	if (shifted.is_zero()) {
		return Z{};
	} else {
		return Z(std::move(shifted), sign_);
	}
}

Z & Z::operator <<= (bit_type pos) {
	FUNCTION_TO_LOG;

	N::opr_bitshift_l_assign_(pos);
	return *this;
}

Z & Z::operator >>= (bit_type pos) {
	FUNCTION_TO_LOG;

	N::opr_bitshift_r_assign_(pos);

	if (N::is_zero()) {
		set_sign_(positive);
	}

	return *this;
}

Z & Z::operator = (std::string_view num_str) {
	FUNCTION_TO_LOG;

	set_sign_(sign_type::handle_string_(num_str));
	N::opr_assign_(num_str);
	if (is_zero()) set_sign_(positive);
	return *this;
}

Z & Z::operator = (const N & n) {
	FUNCTION_TO_LOG;

	N::operator=(n);
	set_sign_(positive);
	return *this;
}

Z & Z::operator = (N && n) {
	FUNCTION_TO_LOG;

	N::operator=(std::move(n));
	set_sign_(positive);
	return *this;
}

} // /namespace jmaths

#include "headers/undef.hh"