#pragma once

#include <ostream>
#include <istream>
#include <utility>
#include <type_traits>
#include <compare>
#include <string>
#include <string_view>
#include <vector>
#include <cstddef>
#include <optional>

#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "hash.hpp"

// declarations of N and associated functions and types
namespace jmaths {

std::ostream & operator << (std::ostream & os, const N & n);
std::istream & operator >> (std::istream & is, N & n);

N operator + (const N & lhs, const N & rhs);
N operator - (const N & lhs, const N & rhs);
N operator * (const N & lhs, const N & rhs);
std::pair<N, N> operator / (const N & lhs, const N & rhs);

N operator & (const N & lhs, const N & rhs);
N operator | (const N & lhs, const N & rhs);
N operator ^ (const N & lhs, const N & rhs);

bool operator == (const N & lhs, const N & rhs);
template <typename INT> requires std::is_integral_v<INT> bool operator == (const N & lhs, INT rhs);
template <typename INT> requires std::is_integral_v<INT> bool operator == (INT lhs, const N & rhs);

std::strong_ordering operator <=> (const N & lhs, const N & rhs);
template <typename INT> requires std::is_integral_v<INT> std::strong_ordering operator <=> (const N & lhs, INT rhs);
template <typename INT> requires std::is_integral_v<INT> std::strong_ordering operator <=> (INT lhs, const N & rhs);

class N {
	friend struct detail;
	friend struct calc;
	friend struct std::hash<N>;
	friend struct std::hash<Z>;
	friend struct std::hash<Q>;

	friend class Q;

	private:
		void remove_leading_zeroes_();
		base_int front_() const;
		std::string conv_to_base_ (unsigned base) const;
		void handle_str_ (std::string_view num_str, unsigned base);

		bool bit_ (bit_type pos) const;
		void bit_ (bit_type pos, bool val);

		template <typename T> requires std::is_same_v<N, T> || std::is_same_v<const N, T> class bit_reference_base_;

	protected:
		std::size_t dynamic_size_() const;

		std::vector<base_int, allocator<base_int>> digits_;

		void opr_incr_();
		void opr_decr_();

		void opr_add_assign_ (const N & rhs);
		void opr_subtr_assign_ (const N & rhs);
		void opr_mult_assign_ (const N & rhs);

		void opr_and_assign_ (const N & rhs);
		void opr_or_assign_ (const N & rhs);
		void opr_xor_assign_ (const N & rhs);

		N opr_compl_() const;
		N opr_bitshift_l_ (bit_type pos) const;
		N opr_bitshift_r_ (bit_type pos) const;

		void opr_bitshift_l_assign_ (bit_type pos);
		void opr_bitshift_r_assign_ (bit_type pos);

		void opr_assign_ (std::string_view num_str);

		template <typename INT> requires std::is_integral_v<INT> void opr_assign_ (INT rhs);

	public:
		class bit_reference;
		class const_bit_reference;

		N();
		N (std::string_view num_str, unsigned base = default_base);
		template <typename INT> requires std::is_integral_v<INT> N (INT num);

		bool is_zero() const;
		bool is_one() const;

		bool is_even() const;
		bool is_odd() const;

		bit_type ctz() const; // count trailing zeroes
		bit_type bits() const; // count number of base 2 digits

		std::size_t size() const; // size of this object in bytes

		std::string to_str (unsigned base = default_base) const; // convert to string in any base >= 2 and <= 64
		std::string to_hex() const; // convert to string in base 16 (assumes base is an integer power of 2)
		explicit operator bool() const;
		template <typename INT> requires std::is_integral_v<INT> && std::is_unsigned_v<INT> std::optional<INT> fits_into() const;

		bit_reference operator [] (bit_type pos);
		const_bit_reference operator [] (bit_type pos) const;

		N & operator ++ ();
		N & operator -- ();

		N & operator += (const N & rhs);
		N & operator -= (const N & rhs);
		N & operator *= (const N & rhs);

		N & operator &= (const N & rhs);
		N & operator |= (const N & rhs);
		N & operator ^= (const N & rhs);

		N operator ~ () const;
		N operator << (bit_type pos) const;
		N operator >> (bit_type pos) const;

		N & operator <<= (bit_type pos);
		N & operator >>= (bit_type pos);

		N & operator = (std::string_view num_str);
		template <typename INT> requires std::is_integral_v<INT> N & operator = (INT rhs);

		static N rand (bit_type upper_bound_exponent);
};

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
class N::bit_reference_base_ {
	friend class N::bit_reference;
	friend class N::const_bit_reference;

	private:
		T & num_;
		const bit_type pos_;

	public:
		bit_reference_base_() = delete;
		bit_reference_base_ (T &&, bit_type) = delete;
		bit_reference_base_ (T & num, bit_type pos);

		operator bool () const;
		explicit operator int () const;
};

class N::bit_reference : public N::bit_reference_base_<N> {
	public:
		using bit_reference_base_::bit_reference_base_;

		bit_reference (const bit_reference & ref);

		bit_reference & operator = (bool val);
		bit_reference & operator = (const bit_reference & ref);
		bit_reference & operator = (const const_bit_reference & ref);

};

class N::const_bit_reference : public bit_reference_base_<const N> {
	public:
		using bit_reference_base_::bit_reference_base_;

		const_bit_reference (const const_bit_reference & ref);
		const_bit_reference (const bit_reference & ref);

		auto operator = (bool) = delete;
		auto operator = (const bit_reference &) = delete;
		auto operator = (const const_bit_reference &) = delete;
};

} // /namespace jmaths

#include "tmpl_N.hpp"
