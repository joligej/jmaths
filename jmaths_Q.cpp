#include "jmaths.hpp"

#include "jmaths_def.cfg"

namespace jmaths {

Q::Q (const N & num, const N & denom, sign_bool sign) : sign_type(sign), num_(num), denom_(denom) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
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

Q::Q() = default;

Q::Q (std::string_view num_str, unsigned base) : sign_type(num_str), num_(handle_fraction_string_(num_str), base), denom_(num_str, base) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
	canonicalise();
	if (is_zero()) set_sign_(positive);
}

Q::Q (const N & n) : num_(n), denom_(1) {}

Q::Q (N && n) : num_(std::move(n)), denom_(1) {}

Q::Q (const Z & z) : sign_type(z.sign_), num_(z.abs()), denom_(1) {}

Q::Q (Z && z) : sign_type(z.sign_), num_(std::move(std::move(z).abs())), denom_(1) {}

Q::Q (const N & num, const N & denom) : num_(num), denom_(denom) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
	canonicalise();
}

Q::Q (const N & num, N && denom) : num_(num), denom_(std::move(denom)) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
	canonicalise();
}

Q::Q (N && num, const N & denom) : num_(std::move(num)), denom_(denom) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
	canonicalise();
}

Q::Q (N && num, N && denom) : num_(std::move(num)), denom_(std::move(denom)) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
	canonicalise();
}

Q::Q (const Z & num, const Z & denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(num.abs()), denom_(denom.abs()) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
	canonicalise();
}

Q::Q (const Z & num, Z && denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(num.abs()), denom_(std::move(std::move(denom).abs())) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
	canonicalise();
}

Q::Q (Z && num, const Z & denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(std::move(std::move(num).abs())), denom_(denom.abs()) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
	canonicalise();
}

Q::Q (Z && num, Z && denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(std::move(std::move(num).abs())), denom_(std::move(std::move(denom).abs())) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
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
			N && first_product = num_ * rhs.denom_;
			first_product.opr_add_assign_(denom_ * rhs.num_);
			num_ = std::move(first_product);
		} else {
			const auto difference = (num_ * rhs.denom_ <=> denom_ * rhs.num_);
			
			if (difference == 0) {
				num_.digits_.clear();
				denom_.opr_assign_(1);
				return *this;
			} else if (difference > 0) {
				N && first_product = num_ * rhs.denom_;
				first_product.opr_subtr_assign_(denom_ * rhs.num_);
				num_ = std::move(first_product);
			} else {
				N && second_product = denom_ * rhs.num_;
				second_product.opr_subtr_assign_(num_ * rhs.denom_);
				num_ = std::move(second_product);
				set_sign_(negative);
			}
		}
	} else {
		if (rhs.is_positive()) {
			const auto difference = (num_ * rhs.denom_ <=> denom_ * rhs.num_);
			
			if (difference == 0) {
				num_.digits_.clear();
				denom_.opr_assign_(1);
				set_sign_(positive);
			} else if (difference > 0) {
				N && first_product = num_ * rhs.denom_;
				first_product.opr_subtr_assign_(denom_ * rhs.num_);
				num_ = std::move(first_product);
			} else {
				N && second_product = denom_ * rhs.num_;
				second_product.opr_subtr_assign_(num_ * rhs.denom_);
				num_ = std::move(second_product);
				set_sign_(positive);
			}
		} else {			
			N && first_product = num_ * rhs.denom_;
			first_product.opr_add_assign_(denom_ * rhs.num_);
			num_ = std::move(first_product);
		}
	}
	
	denom_.opr_mult_assign_(rhs.denom_);
	
	canonicalise();
	
	return *this;
}

/*Q & Q::operator -= (const Q & rhs);

Q & Q::operator *= (const Q & rhs);

Q & Q::operator &= (const Q & rhs);

Q & Q::operator |= (const Q & rhs);

Q & Q::operator ^= (const Q & rhs);

Q Q::operator - () const &;

Q && Q::operator - () &&;

Q Q::operator ~ () const;

Q Q::operator << (BIT_TYPE pos) const;

Q Q::operator >> (BIT_TYPE pos) const;

Q & Q::operator <<= (BIT_TYPE pos);

Q & Q::operator >>= (BIT_TYPE pos);

Q & Q::operator = (std::string_view num_str);

Q & Q::operator = (const N & n);

Q & Q::operator = (N && n);
        
Q & Q::operator = (const Z & z);

Q & Q::operator = (Z && z);
*/

} // /namespace jmaths