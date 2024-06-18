#include "../src/jmaths.hpp"

#include "../config/jmaths_def.cfg"

namespace jmaths {

/**********************************************************/
// implementation functions

std::ostream & detail::opr_ins (std::ostream & os, const Q & q) {
	FUNCTION_TO_STDERR;

	return os << (std::string)q;
}

std::istream & detail::opr_extr (std::istream & is, Q & q) {
	FUNCTION_TO_STDERR;

	std::string num_str;
	is >> num_str;
	q = num_str;
	return is;
}

Q detail::opr_add (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	using sign_type::positive, sign_type::negative;
	
	if (lhs.is_positive()) {
		if (rhs.is_positive()) {
			N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
			first_product.opr_add_assign_(detail::opr_mult(lhs.denom_, rhs.num_));
			return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
		} else {
			N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
			N && second_product = detail::opr_mult(lhs.denom_, rhs.num_);
			
			const auto difference = detail::opr_comp(first_product, second_product);
			
			if (difference == 0) {
				return Q();
			} else if (difference > 0) {
				first_product.opr_subtr_assign_(second_product);
				return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
			} else {
				second_product.opr_subtr_assign_(first_product);
				return Q(std::move(second_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
			}
		}
	} else {
		if (rhs.is_positive()) {
			N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
			N && second_product = detail::opr_mult(lhs.denom_, rhs.num_);
			
			const auto difference = detail::opr_comp(first_product, second_product);
			
			if (difference == 0) {
				return Q();
			} else if (difference > 0) {
				first_product.opr_subtr_assign_(second_product);
				return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
			} else {
				second_product.opr_subtr_assign_(first_product);
				return Q(std::move(second_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
			}
		} else {			
			N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
			first_product.opr_add_assign_(detail::opr_mult(lhs.denom_, rhs.num_));
			return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
		}
	}
}

Q detail::opr_subtr (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	using sign_type::positive, sign_type::negative;
	
	if (lhs.is_positive()) {
		if (rhs.is_positive()) {
			N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
			N && second_product = detail::opr_mult(lhs.denom_, rhs.num_);
			
			const auto difference = detail::opr_comp(first_product, second_product);
			
			if (difference == 0) {
				return Q();
			} else if (difference > 0) {
				first_product.opr_subtr_assign_(second_product);
				return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
			} else {
				second_product.opr_subtr_assign_(first_product);
				return Q(std::move(second_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
			}
		} else {
			N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
			first_product.opr_add_assign_(detail::opr_mult(lhs.denom_, rhs.num_));
			return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
		}
	} else {
		if (rhs.is_positive()) {
			N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
			first_product.opr_add_assign_(detail::opr_mult(lhs.denom_, rhs.num_));
			return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
		} else {			
			N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
			N && second_product = detail::opr_mult(lhs.denom_, rhs.num_);
			
			const auto difference = detail::opr_comp(first_product, second_product);
			
			if (difference == 0) {
				return Q();
			} else if (difference > 0) {
				first_product.opr_subtr_assign_(second_product);
				return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
			} else {
				second_product.opr_subtr_assign_(first_product);
				return Q(std::move(second_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
			}
		}
	}
}

Q detail::opr_mult (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	N && numerator = lhs.num_ * rhs.num_;
	
	if (numerator.is_zero()) return Q();

	return Q(std::move(numerator), lhs.denom_ * rhs.denom_, static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_));
}

Q detail::opr_div (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	N && numerator = lhs.num_ * rhs.denom_;
	
	if (numerator.is_zero()) return Q();

	return Q(std::move(numerator), lhs.denom_ * rhs.num_, static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_));
}

Q detail::opr_and (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	N && denominator = lhs.denom_ & rhs.denom_;
	
	if (denominator.is_zero()) throw error::division_by_zero();
	
	N && numerator = lhs.num_ & rhs.num_;
	
	if (numerator.is_zero()) return Q();

	return Q(std::move(numerator), std::move(denominator), static_cast<sign_type::sign_bool>(lhs.sign_ & rhs.sign_));
}

Q detail::opr_or (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	N && numerator = lhs.num_ | rhs.num_;
	
	if (numerator.is_zero()) return Q();

	return Q(std::move(numerator), lhs.denom_ | rhs.denom_, static_cast<sign_type::sign_bool>(lhs.sign_ | rhs.sign_));
}

Q detail::opr_xor (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	N && denominator = lhs.denom_ ^ rhs.denom_;
	
	if (denominator.is_zero()) throw error::division_by_zero();
	
	N && numerator = lhs.num_ ^ rhs.num_;
	
	if (numerator.is_zero()) return Q();

	return Q(std::move(numerator), std::move(denominator), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_));
}

bool detail::opr_eq (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	return (lhs.sign_ == rhs.sign_ && lhs.num_ == rhs.num_ && lhs.denom_ == rhs.denom_);
}

std::strong_ordering detail::opr_comp (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	if (lhs.is_positive()) {
		if (rhs.is_positive()) {
			return detail::opr_comp(lhs.num_ * rhs.denom_, rhs.num_ * lhs.denom_);
		} else {
			return std::strong_ordering::greater;
		}
	} else {
		if (rhs.is_positive()) {
			return std::strong_ordering::less;
		} else {
			return detail::opr_comp(rhs.num_ * lhs.denom_, lhs.num_ * rhs.denom_);
		}
	}
}

/**********************************************************/
// forwarding functions

std::ostream & operator << (std::ostream & os, const Q & q) {
	FUNCTION_TO_STDERR;

	return detail::opr_ins(os, q);
}

std::istream & operator >> (std::istream & is, Q & q) {
	FUNCTION_TO_STDERR;

	return detail::opr_extr(is, q);
}

Q operator + (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	return detail::opr_add(lhs, rhs);
}

Q operator - (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	return detail::opr_subtr(lhs, rhs);
}

Q operator * (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	return detail::opr_mult(lhs, rhs);
}

Q operator / (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	if (rhs.is_zero()) throw error::division_by_zero();
	return detail::opr_div(lhs, rhs);
}

Q operator & (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	return detail::opr_and(lhs, rhs);
}

Q operator | (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	return detail::opr_or(lhs, rhs);
}

Q operator ^ (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	return detail::opr_xor(lhs, rhs);
}

bool operator == (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	return detail::opr_eq(lhs, rhs);
}

std::strong_ordering operator <=> (const Q & lhs, const Q & rhs) {
	FUNCTION_TO_STDERR;

	return detail::opr_comp(lhs, rhs);
}

} // /namespace jmaths

namespace jmaths {

Q::Q (N && num, N && denom, sign_bool sign) : sign_type(sign), num_(std::move(num)), denom_(std::move(denom)) {
	FUNCTION_TO_STDERR;

	canonicalise_();
}

Q::Q (const N & num, const N & denom, sign_bool sign) : sign_type(sign), num_(num), denom_(denom) {
	FUNCTION_TO_STDERR;

	canonicalise_();
}

Q::Q (std::tuple<N, N, sign_bool> && fraction_info) : Q(std::move(std::get<0>(fraction_info)), std::move(std::get<1>(fraction_info)), std::move(std::get<2>(fraction_info))) {
	FUNCTION_TO_STDERR;
}

void Q::canonicalise_() {
	FUNCTION_TO_STDERR;

	const N gcd = calc::gcd(num_, denom_);
	num_ = detail::opr_div(num_, gcd).first;
	denom_ = detail::opr_div(denom_, gcd).first;
}

std::size_t Q::dynamic_size_() const {
	FUNCTION_TO_STDERR;

	return (num_.dynamic_size_() + denom_.dynamic_size_());
}

Q::Q() : denom_(1) {
	FUNCTION_TO_STDERR;
}

Q::Q (std::string_view num_str, unsigned base) : sign_type(num_str), num_(handle_fraction_string_(num_str), base), denom_(num_str, base) {
	FUNCTION_TO_STDERR;

	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise_();
	if (is_zero()) set_sign_(positive);
}

Q::Q (const N & n) : num_(n), denom_(1) {
	FUNCTION_TO_STDERR;
}

Q::Q (N && n) : num_(std::move(n)), denom_(1) {
	FUNCTION_TO_STDERR;
}

Q::Q (const Z & z) : sign_type(z.sign_), num_(z.abs()), denom_(1) {
	FUNCTION_TO_STDERR;
}

Q::Q (Z && z) : sign_type(z.sign_), num_(std::move(std::move(z).abs())), denom_(1) {
	FUNCTION_TO_STDERR;
}

Q::Q (const N & num, const N & denom) : num_(num), denom_(denom) {
	FUNCTION_TO_STDERR;

	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise_();
}

Q::Q (const N & num, N && denom) : num_(num), denom_(std::move(denom)) {
	FUNCTION_TO_STDERR;

	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise_();
}

Q::Q (N && num, const N & denom) : num_(std::move(num)), denom_(denom) {
	FUNCTION_TO_STDERR;

	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise_();
}

Q::Q (N && num, N && denom) : num_(std::move(num)), denom_(std::move(denom)) {
	FUNCTION_TO_STDERR;

	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise_();
}

Q::Q (const Z & num, const Z & denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(num.abs()), denom_(denom.abs()) {
	FUNCTION_TO_STDERR;

	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise_();
}

Q::Q (const Z & num, Z && denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(num.abs()), denom_(std::move(std::move(denom).abs())) {
	FUNCTION_TO_STDERR;

	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise_();
}

Q::Q (Z && num, const Z & denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(std::move(std::move(num).abs())), denom_(denom.abs()) {
	FUNCTION_TO_STDERR;

	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise_();
}

Q::Q (Z && num, Z && denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(std::move(std::move(num).abs())), denom_(std::move(std::move(denom).abs())) {
	FUNCTION_TO_STDERR;

	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise_();
}

bool Q::is_zero() const {
	FUNCTION_TO_STDERR;

	return num_.is_zero();
}

bool Q::is_one() const {
	FUNCTION_TO_STDERR;

	return (is_positive() && num_.is_one() && denom_.is_one());
}

bool Q::is_neg_one() const {
	FUNCTION_TO_STDERR;

	return (is_negative() && num_.is_one() && denom_.is_one());
}

Q Q::abs() const & {
	FUNCTION_TO_STDERR;

	return Q(num_, denom_, positive);
}

Q && Q::abs() && {
	FUNCTION_TO_STDERR;

	set_sign_(positive);
	return std::move(*this);
}

Q Q::inverse() const & {
	FUNCTION_TO_STDERR;

	if (num_.is_zero()) throw error::division_by_zero("Cannot take the inverse of zero!");
	return Q(denom_, num_, sign_);
}

Q && Q::inverse() && {
	FUNCTION_TO_STDERR;

	if (num_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	std::swap(num_, denom_);
	return std::move(*this);
}

std::size_t Q::size() const {
	FUNCTION_TO_STDERR;

	return (sizeof(*this) + dynamic_size_());
}

std::string Q::to_str (unsigned base) const {
	FUNCTION_TO_STDERR;

	if (is_negative()) {
		return (std::string{'-'} + num_.to_str(base) + '/' + denom_.to_str(base));
	} else {
		return (num_.to_str(base) + '/' + denom_.to_str(base));
	}
}

Q::operator std::string() const {
	FUNCTION_TO_STDERR;

	if (is_negative()) {
		return (std::string{'-'} + (std::string)num_ + '/' + (std::string)denom_);
	} else {
		return ((std::string)num_ + '/' + (std::string)denom_);
	}
}

std::string Q::to_base16() const {
	FUNCTION_TO_STDERR;

	if (is_negative()) {
		return (std::string{'-'} + num_.to_base16() + '/' + denom_.to_base16());
	} else {
		return (num_.to_base16() + '/' + denom_.to_base16());
	}
}

Q::operator bool() const {
	FUNCTION_TO_STDERR;

	return !is_zero();
}

Q & Q::operator ++ () {
	FUNCTION_TO_STDERR;

	if (is_positive()) {
		num_.opr_add_assign_(denom_);
	} else {
		const auto difference = detail::opr_comp(num_, denom_);
		
		if (difference == 0) {
			num_.digits_.clear();
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

Q & Q::operator -- () {
	FUNCTION_TO_STDERR;

	if (is_positive()) {
		const auto difference = detail::opr_comp(num_, denom_);
			
		if (difference == 0) {
			num_.digits_.clear();
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

Q & Q::operator += (const Q & rhs) {
	FUNCTION_TO_STDERR;

	if (this->is_positive()) {
		if (rhs.is_positive()) {
			N && first_product = detail::opr_mult(num_, rhs.denom_);
			first_product.opr_add_assign_(detail::opr_mult(denom_, rhs.num_));
			num_ = std::move(first_product);
		} else {
			N && first_product = detail::opr_mult(num_, rhs.denom_);
			N && second_product = detail::opr_mult(denom_, rhs.num_);
			
			const auto difference = detail::opr_comp(first_product, second_product);
			
			if (difference == 0) {
				num_.digits_.clear();
				denom_.opr_assign_(1);
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
			N && first_product = detail::opr_mult(num_, rhs.denom_);
			N && second_product = detail::opr_mult(denom_, rhs.num_);
			
			const auto difference = detail::opr_comp(first_product, second_product);
			
			if (difference == 0) {
				num_.digits_.clear();
				denom_.opr_assign_(1);
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
			N && first_product = detail::opr_mult(num_, rhs.denom_);
			first_product.opr_add_assign_(detail::opr_mult(denom_, rhs.num_));
			num_ = std::move(first_product);
		}
	}
	
	denom_.opr_mult_assign_(rhs.denom_);
	
	canonicalise_();
	
	return *this;
}

Q & Q::operator -= (const Q & rhs) {
	FUNCTION_TO_STDERR;

	if (this->is_positive()) {
		if (rhs.is_positive()) {
			N && first_product = detail::opr_mult(num_, rhs.denom_);
			N && second_product = detail::opr_mult(denom_, rhs.num_);
			
			const auto difference = detail::opr_comp(first_product, second_product);
			
			if (difference == 0) {
				num_.digits_.clear();
				denom_.opr_assign_(1);
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
			N && first_product = detail::opr_mult(num_, rhs.denom_);
			first_product.opr_add_assign_(detail::opr_mult(denom_, rhs.num_));
			num_ = std::move(first_product);
		}
	} else {
		if (rhs.is_positive()) {
			N && first_product = detail::opr_mult(num_, rhs.denom_);
			first_product.opr_add_assign_(detail::opr_mult(denom_, rhs.num_));
			num_ = std::move(first_product);
		} else {			
			N && first_product = detail::opr_mult(num_, rhs.denom_);
			N && second_product = detail::opr_mult(denom_, rhs.num_);
			
			const auto difference = detail::opr_comp(first_product, second_product);
			
			if (difference == 0) {
				num_.digits_.clear();
				denom_.opr_assign_(1);
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

Q & Q::operator *= (const Q & rhs) {
	FUNCTION_TO_STDERR;

	num_.opr_mult_assign_(rhs.num_);
	denom_.opr_mult_assign_(rhs.denom_);
	set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
	canonicalise_();
	return *this;
}

Q & Q::operator /= (const Q & rhs) {
	FUNCTION_TO_STDERR;

	if (rhs.is_zero()) throw error::division_by_zero();
	
	num_.opr_mult_assign_(rhs.denom_);
	denom_.opr_mult_assign_(rhs.num_);
	set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
	canonicalise_();
	return *this;
}

Q & Q::operator &= (const Q & rhs) {
	FUNCTION_TO_STDERR;

	denom_.opr_and_assign_(rhs.denom_);
	if (denom_.is_zero()) throw error::division_by_zero();
	num_.opr_and_assign_(rhs.num_);
	set_sign_(is_zero() ? positive : this->sign_ & rhs.sign_);
	canonicalise_();
	return *this;
}

Q & Q::operator |= (const Q & rhs) {
	FUNCTION_TO_STDERR;

	num_.opr_or_assign_(rhs.num_);
	denom_.opr_or_assign_(rhs.denom_);
	set_sign_(is_zero() ? positive : this->sign_ | rhs.sign_);
	canonicalise_();
	return *this;
}

Q & Q::operator ^= (const Q & rhs) {
	FUNCTION_TO_STDERR;

	denom_.opr_xor_assign_(rhs.denom_);
	if (denom_.is_zero()) throw error::division_by_zero();
	num_.opr_xor_assign_(rhs.num_);
	set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
	canonicalise_();
	return *this;
}

Q Q::operator - () const & {
	FUNCTION_TO_STDERR;

	if (is_zero()) return *this;
	return Q(num_, denom_, static_cast<sign_bool>(!sign_));
}

Q && Q::operator - () && {
	FUNCTION_TO_STDERR;

	flip_sign();
	return std::move(*this);
}

Q Q::operator ~ () const {
	FUNCTION_TO_STDERR;

	N && num_complemented = num_.opr_compl_();
	
	if (num_complemented.is_zero()) {
		return Q();
	} else {
		N && denom_complemented = denom_.opr_compl_();
		if (denom_complemented.is_zero()) throw error::division_by_zero("Denominator of complemented fraction cannot be zero!");
		return Q(std::move(num_complemented), std::move(denom_complemented), static_cast<sign_bool>(!sign_));
	}
}

Q Q::operator << (BIT_TYPE pos) const {
	FUNCTION_TO_STDERR;

	return Q(num_.opr_bitshift_l_(pos), denom_, sign_);
}

Q Q::operator >> (BIT_TYPE pos) const {
	FUNCTION_TO_STDERR;

	if (is_zero()) return Q();
	return Q(num_, denom_.opr_bitshift_l_(pos), sign_);
}

Q & Q::operator <<= (BIT_TYPE pos) {
	FUNCTION_TO_STDERR;

	num_.opr_bitshift_l_assign_(pos);
	canonicalise_();
	return *this;
}

Q & Q::operator >>= (BIT_TYPE pos) {
	FUNCTION_TO_STDERR;

	if (is_zero()) return *this;	
	denom_.opr_bitshift_l_assign_(pos);
	return *this;
}

Q & Q::operator = (std::string_view num_str) {
	FUNCTION_TO_STDERR;

	set_sign_(sign_type::handle_string_(num_str));
	num_.opr_assign_(sign_type::handle_fraction_string_(num_str));
	denom_.opr_assign_(num_str);
	canonicalise_();
	if (is_zero()) set_sign_(positive);
	return *this;
}

Q & Q::operator = (const N & n) {
	FUNCTION_TO_STDERR;

	set_sign_(positive);
	num_ = n;
	denom_.opr_assign_(1);
	return *this;
}

Q & Q::operator = (N && n) {
	FUNCTION_TO_STDERR;

	set_sign_(positive);
	num_ = std::move(n);
	denom_.opr_assign_(1);
	return *this;
}
        
Q & Q::operator = (const Z & z) {
	FUNCTION_TO_STDERR;

	set_sign_(z.sign_);
	num_ = z.abs();
	denom_.opr_assign_(1);
	return *this;
}

Q & Q::operator = (Z && z) {
	FUNCTION_TO_STDERR;

	set_sign_(z.sign_);
	num_ = std::move(std::move(z).abs());
	denom_.opr_assign_(1);
	return *this;
}

} // /namespace jmaths

#include "../config/jmaths_undef.cfg"
