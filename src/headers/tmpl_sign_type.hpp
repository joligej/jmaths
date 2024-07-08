#pragma once

#include <type_traits>

#include "jmaths_sign_type.hpp"

#include "jmaths_def.hh"

// member function templates of sign_type
namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
sign_type::sign_type (INT & num) : sign_(handle_int_(num)) {
	FUNCTION_TO_LOG;
}

template <typename INT>
requires std::is_integral_v<INT>
sign_type::sign_bool sign_type::handle_int_ (INT & num) {
	FUNCTION_TO_LOG;

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
	FUNCTION_TO_LOG;

	sign_ = static_cast<sign_bool>(val);
}

} // /namespace jmaths

#include "jmaths_undef.hh"
