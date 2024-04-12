#include "jmaths.hpp"

#include "jmaths_def.cfg"

namespace jmaths {

Q::Q (const N & num, const N & denom, sign_bool sign) : sign_type(sign), num_(num), denom_(denom) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise();
}

void Q::canonicalise() {
	N && gcd = calc::gcd(num_, denom_);
	num_ = detail::opr_div(num_, gcd).first;
	denom_ = detail::opr_div(denom_, gcd).first;
}

size_t Q::dynamic_size_() const {
	return (num_.dynamic_size_() + denom_.dynamic_size_());
}

Q::Q() : denom_(1) {}

Q::Q (std::string_view num_str, unsigned base) : sign_type(num_str), num_(handle_fraction_string_(num_str), base), denom_(num_str, base) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise();
	if (is_zero()) set_sign_(positive);
}

Q::Q (const N & n) : num_(n), denom_(1) {}

Q::Q (N && n) : num_(std::move(n)), denom_(1) {}

Q::Q (const Z & z) : sign_type(z.sign_), num_(z.abs()), denom_(1) {}

Q::Q (Z && z) : sign_type(z.sign_), num_(std::move(std::move(z).abs())), denom_(1) {}

Q::Q (const N & num, const N & denom) : num_(num), denom_(denom) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise();
}

Q::Q (const N & num, N && denom) : num_(num), denom_(std::move(denom)) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise();
}

Q::Q (N && num, const N & denom) : num_(std::move(num)), denom_(denom) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise();
}

Q::Q (N && num, N && denom) : num_(std::move(num)), denom_(std::move(denom)) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise();
}

Q::Q (const Z & num, const Z & denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(num.abs()), denom_(denom.abs()) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise();
}

Q::Q (const Z & num, Z && denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(num.abs()), denom_(std::move(std::move(denom).abs())) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise();
}

Q::Q (Z && num, const Z & denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(std::move(std::move(num).abs())), denom_(denom.abs()) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise();
}

Q::Q (Z && num, Z && denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(std::move(std::move(num).abs())), denom_(std::move(std::move(denom).abs())) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	canonicalise();
}

bool Q::is_zero() const {
	return num_.is_zero();
}

bool Q::is_one() const {
	return (is_positive() && num_.is_one() && denom_.is_one());
}

bool Q::is_neg_one() const {
	return (is_negative() && num_.is_one() && denom_.is_one());
}

Q Q::abs() const & {
	return Q(num_, denom_, positive);
}

Q && Q::abs() && {
	set_sign_(positive);
	return std::move(*this);
}

Q Q::inverse() const & {
	return Q(denom_, num_, sign_);
}

Q && Q::inverse() && {
	if (num_.is_zero()) throw error::division_by_zero("Denominator cannot be zero!");
	std::swap(num_, denom_);
	return std::move(*this);
}

size_t Q::size() const {
	return (sizeof(*this) + dynamic_size_());
}

std::string Q::to_str (unsigned base) const {
	if (is_negative()) {
		return (std::string{'-'} + num_.to_str(base) + '/' + denom_.to_str(base));
	} else {
		return (num_.to_str(base) + '/' + denom_.to_str(base));
	}
}

Q::operator std::string() const {
	if (is_negative()) {
		return (std::string{'-'} + (std::string)num_ + '/' + (std::string)denom_);
	} else {
		return ((std::string)num_ + '/' + (std::string)denom_);
	}
}

std::string Q::to_base16() const {
	if (is_negative()) {
		return (std::string{'-'} + num_.to_base16() + '/' + denom_.to_base16());
	} else {
		return (num_.to_base16() + '/' + denom_.to_base16());
	}
}

Q::operator bool() const {
	return !is_zero();
}

Q & Q::operator ++ () {
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
	
	canonicalise();
	
	return *this;
}

Q & Q::operator -= (const Q & rhs) {
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
	
	canonicalise();
	
	return *this;
}

Q & Q::operator *= (const Q & rhs) {
	num_.opr_mult_assign_(rhs.num_);
	denom_.opr_mult_assign_(rhs.denom_);
	set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
	canonicalise();
	return *this;
}

Q & Q::operator &= (const Q & rhs) {
	num_.opr_and_assign_(rhs.num_);
	denom_.opr_and_assign_(rhs.denom_);
	set_sign_(is_zero() ? positive : this->sign_ & rhs.sign_);
	canonicalise();
	return *this;
}

Q & Q::operator |= (const Q & rhs) {
	num_.opr_or_assign_(rhs.num_);
	denom_.opr_or_assign_(rhs.denom_);
	set_sign_(is_zero() ? positive : this->sign_ | rhs.sign_);
	canonicalise();
	return *this;
}

Q & Q::operator ^= (const Q & rhs) {
	num_.opr_xor_assign_(rhs.num_);
	denom_.opr_xor_assign_(rhs.denom_);
	set_sign_(is_zero() ? positive : this->sign_ ^ rhs.sign_);
	canonicalise();
	return *this;
}

Q Q::operator - () const & {
	if (is_zero()) return *this;
	return Q(num_, denom_, static_cast<sign_bool>(!sign_));
}

Q && Q::operator - () && {
	flip_sign();
	return std::move(*this);
}

Q Q::operator ~ () const {
	N && num_complemented = num_.opr_compl_();
	
	if (num_complemented.is_zero()) {
		return Q();
	} else {
		return Q(std::move(num_complemented), denom_.opr_compl_(), static_cast<sign_bool>(!sign_));
	}
}

Q Q::operator << (BIT_TYPE pos) const {
	return Q(num_.opr_bitshift_l_(pos), denom_, sign_);
}

Q Q::operator >> (BIT_TYPE pos) const {
	if (is_zero()) return Q();
	return Q(num_, denom_.opr_bitshift_l_(pos), sign_);
}

Q & Q::operator <<= (BIT_TYPE pos) {
	num_.opr_bitshift_l_assign_(pos);
	return *this;
}

Q & Q::operator >>= (BIT_TYPE pos) {
	if (is_zero()) return *this;	
	denom_.opr_bitshift_l_assign_(pos);
	return *this;
}

Q & Q::operator = (std::string_view num_str) {
	set_sign_(sign_type::handle_string_(num_str));
	num_.opr_assign_(sign_type::handle_fraction_string_(num_str));
	denom_.opr_assign_(num_str);
	if (is_zero()) set_sign_(positive);
	return *this;
}

Q & Q::operator = (const N & n) {
	set_sign_(positive);
	num_ = n;
	denom_.opr_assign_(1);
	return *this;
}

Q & Q::operator = (N && n) {
	set_sign_(positive);
	num_ = std::move(n);
	denom_.opr_assign_(1);
	return *this;
}
        
Q & Q::operator = (const Z & z) {
	set_sign_(z.sign_);
	num_ = z.abs();
	denom_.opr_assign_(1);
	return *this;
}

Q & Q::operator = (Z && z) {
	set_sign_(z.sign_);
	num_ = std::move(std::move(z).abs());
	denom_.opr_assign_(1);
	return *this;
}

} // /namespace jmaths