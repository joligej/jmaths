#include "jmaths.hpp"

namespace jmaths {
	
sign_type::sign_type() : sign_(positive) {}

sign_type::sign_type (sign_bool sign) : sign_(sign) {}

sign_type::sign_type (std::string_view & num_str) : sign_(handle_string_(num_str)) {}

sign_type::sign_bool sign_type::handle_string_ (std::string_view & num_str) {
	if (!num_str.empty() && num_str.front() == '-') {
		num_str.remove_prefix(1);
		return num_str.empty() ? positive : negative;
	} else {
		return positive;
	}
}

bool sign_type::is_positive() const {
	return (sign_ == positive);
}

bool sign_type::is_negative() const {
	return (sign_ == negative);
}

void sign_type::flip_sign() {
	if (is_zero()) return;
	set_sign_(!sign_);
}

} // /namespace jmaths