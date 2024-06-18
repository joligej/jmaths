#include "../src/jmaths.hpp"

#include "../config/jmaths_def.cfg"

// functions for jmaths::sign_type
namespace jmaths {
	
sign_type::sign_type() : sign_(positive) {
	FUNCTION_TO_STDERR;
}

sign_type::sign_type (sign_bool sign) : sign_(sign) {
	FUNCTION_TO_STDERR;
}

sign_type::sign_type (std::string_view & num_str) : sign_(handle_string_(num_str)) {
	FUNCTION_TO_STDERR;
}

sign_type::sign_bool sign_type::handle_string_ (std::string_view & num_str) {
	FUNCTION_TO_STDERR;

	if (!num_str.empty() && num_str.front() == '-') {
		num_str.remove_prefix(1);
		return num_str.empty() ? positive : negative;
	} else {
		return positive;
	}
}

std::string_view sign_type::handle_fraction_string_ (std::string_view & num_str) {
	FUNCTION_TO_STDERR;

	const auto fraction_bar = num_str.find('/');
	std::string_view numerator = num_str.substr(0, fraction_bar);
	num_str.remove_prefix(fraction_bar + 1);
	return numerator;
}

bool sign_type::is_positive() const {
	FUNCTION_TO_STDERR;

	return (sign_ == positive);
}

bool sign_type::is_negative() const {
	FUNCTION_TO_STDERR;

	return (sign_ == negative);
}

void sign_type::flip_sign() {
	FUNCTION_TO_STDERR;

	if (is_zero()) return;
	set_sign_(!sign_);
}

} // /namespace jmaths

#include "../config/jmaths_undef.cfg"
