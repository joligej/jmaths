#pragma once

#include <string_view>
#include <type_traits>

#include "declarations.hpp"

// declarations of sign_type and associated functions and types
namespace jmaths {

class sign_type {

	friend struct detail;
	friend struct calc;

	protected:
		enum sign_bool : bool {
			positive = 0,
			negative = 1
		} sign_;

		sign_type();
		sign_type (sign_bool sign);
		sign_type (std::string_view & num_str);
		template <typename INT> requires std::is_integral_v<INT> sign_type (INT & num);

		static sign_bool handle_string_ (std::string_view & num_str);
		static std::string_view handle_fraction_string_ (std::string_view & num_str);
		template <typename INT> requires std::is_integral_v<INT> static sign_bool handle_int_ (INT & num);

		template <typename BOOL> requires std::is_convertible_v<BOOL, std::underlying_type_t<sign_bool>> void set_sign_ (BOOL val);

	public:
		bool is_positive() const;
		bool is_negative() const;

		virtual bool is_zero() const = 0;
		void flip_sign();

};

} // /namespace jmaths

#include "tmpl_sign_type.hpp"
