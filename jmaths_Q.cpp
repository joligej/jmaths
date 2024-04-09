#include "jmaths.hpp"

#include "jmaths_def.cfg"

namespace jmaths {

Q::Q (const N & num, const N & denom, sign_bool sign) : sign_type(sign), num_(num), denom_(denom) {
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

Q::Q (const Z & num, const Z & denom) : sign_type(num.is_zero() ? positive : static_cast<sign_bool>(num.sign_ ^ denom.sign_)), num_(num.abs()), denom_(denom.abs()) {
	if (denom_.is_zero()) throw error::division_by_zero("Denominator cannot be negative!");
	canonicalise();
}

} // /namespace jmaths