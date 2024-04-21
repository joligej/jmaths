#pragma once

#include <iostream> // for std::ostream
#include <string> // for std::string
#include <string_view> //for std::string_view
#include <vector> // for std::vector
#include <utility> // for std::pair and std::swap
#include <compare> // for std::strong_ordering
#include <exception> // for std::exception
#include <type_traits> // for std::is_integral_v and std::is_floating_point_v and std::is_(un)signed_v and std::is_convertible_v and std::underlying_type_t
#include <optional> // for std::optional
#include <limits> // for std::numeric_limits<T>::max

#include "../config/jmaths_def.cfg"

// all the types
namespace jmaths {

// !!! make N, Z and Q a template with "typename Allocator = DEFAULT_ALLOCATOR", before which #define DEFAULT_ALLOCATOR std::allocator<BASE_INT>

class N;
class sign_type;
class Z;
class Q;
class error;

} // /namespace jmaths

namespace jmaths {

struct detail {
	
static std::ostream & opr_ins (std::ostream & os, const N & n);
static std::istream & opr_extr (std::istream & is, N & n);

static N opr_add (const N & lhs, const N & rhs);
static N opr_subtr (N lhs, const N & rhs);
static N opr_mult (const N & lhs, const N & rhs);
static std::pair<N, N> opr_div (const N & lhs, const N & rhs);

static N opr_and (const N & lhs, const N & rhs);
static N opr_or (const N & lhs, const N & rhs);
static N opr_xor (const N & lhs, const N & rhs);

static bool opr_eq (const N & lhs, const N & rhs);
template <typename INT> requires std::is_integral_v<INT> static bool opr_eq (const N & lhs, INT rhs);

static std::strong_ordering opr_comp (const N & lhs, const N & rhs);
template <typename INT> requires std::is_integral_v<INT> static std::strong_ordering opr_comp (const N & lhs, INT rhs);

static std::ostream & opr_ins (std::ostream & os, const Z & z);
static std::istream & opr_extr (std::istream & is, Z & z);

static Z opr_add (const Z & lhs, const Z & rhs);
static Z opr_subtr (const Z & lhs, const Z & rhs);
static Z opr_mult (const Z & lhs, const Z & rhs);
static std::pair<Z, Z> opr_div (const Z & lhs, const Z & rhs);

static Z opr_and (const Z & lhs, const Z & rhs);
static Z opr_or (const Z & lhs, const Z & rhs);
static Z opr_xor (const Z & lhs, const Z & rhs);

static bool opr_eq (const Z & lhs, const Z & rhs);
template <typename INT> requires std::is_integral_v<INT> static bool opr_eq (const Z & lhs, INT rhs);

static std::strong_ordering opr_comp (const Z & lhs, const Z & rhs);
template <typename INT> requires std::is_integral_v<INT> static std::strong_ordering opr_comp (const Z & lhs, INT rhs);

static std::ostream & opr_ins (std::ostream & os, const Q & q);
static std::istream & opr_extr (std::istream & is, Q & q);

static Q opr_add (const Q & lhs, const Q & rhs);
static Q opr_subtr (const Q & lhs, const Q & rhs);
static Q opr_mult (const Q & lhs, const Q & rhs);
static Q opr_div (const Q & lhs, const Q & rhs);

static Q opr_and (const Q & lhs, const Q & rhs);
static Q opr_or (const Q & lhs, const Q & rhs);
static Q opr_xor (const Q & lhs, const Q & rhs);

static bool opr_eq (const Q & lhs, const Q & rhs);
template <typename FLOAT> requires std::is_floating_point_v<FLOAT> static bool opr_eq (const Q & lhs, FLOAT rhs);

static std::strong_ordering opr_comp (const Q & lhs, const Q & rhs);
template <typename FLOAT> requires std::is_floating_point_v<FLOAT> static std::strong_ordering opr_comp (const Q & lhs, FLOAT rhs);

}; // /namespace struct detail

} // /namespace jmaths

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

struct calc {

static N gcd (N a, N b);
static std::pair<N, N> sqrt (const N & num);
static N pow (N base, N exponent);
static Z pow (Z base, N exponent);

}; // /namespace struct calc

class N {
	friend struct detail;
	friend struct calc;
	
	friend class Q;
	
	private:
		void remove_leading_zeroes_();
		BASE_INT front_() const;
		std::string conv_to_base_ (unsigned base) const;
		void handle_str_ (std::string_view num_str, unsigned base);
		
		bool bit_ (BIT_TYPE pos) const;
		void bit_ (BIT_TYPE pos, bool val);
				
		template <typename T> requires std::is_same_v<N, T> || std::is_same_v<const N, T> class bit_reference_base_;
		
	protected:
		size_t dynamic_size_() const;
		
		std::vector<BASE_INT> digits_;
		
		void opr_incr_();
		void opr_decr_();
		
		void opr_add_assign_ (const N & rhs);
		void opr_subtr_assign_ (const N & rhs);
		void opr_mult_assign_ (const N & rhs);
		
		void opr_and_assign_ (const N & rhs);
		void opr_or_assign_ (const N & rhs);
		void opr_xor_assign_ (const N & rhs);
		
		N opr_compl_() const;
		N opr_bitshift_l_ (BIT_TYPE pos) const;
		N opr_bitshift_r_ (BIT_TYPE pos) const;
		
		void opr_bitshift_l_assign_ (BIT_TYPE pos);
		void opr_bitshift_r_assign_ (BIT_TYPE pos);
		
		void opr_assign_ (std::string_view num_str);
		
		template <typename INT> requires std::is_integral_v<INT> void opr_assign_ (INT rhs);
		
	public:
		class bit_reference;
		class const_bit_reference;
		
		N();
		N (std::string_view num_str, unsigned base = DEFAULT_BASE);
		template <typename INT> requires std::is_integral_v<INT> N (INT num);
		
		bool is_zero() const;
		bool is_one() const;
		
		bool is_even() const;
		bool is_odd() const;
		
		BIT_TYPE ctz() const; // count trailing zeroes
		BIT_TYPE bits() const; // count number of base 2 digits
		
		size_t size() const; // size of this object in bytes
		
		std::string to_str (unsigned base) const; // convert to string in any base >= 2 and <= 64
		operator std::string() const; // convert to string in default base
		std::string to_base16() const; // convert to string in base 16 (assumes BASE is a power of 2)
		explicit operator bool() const;
		template <typename INT> requires std::is_integral_v<INT> && std::is_unsigned_v<INT> std::optional<INT> fits_into() const;
		
		bit_reference operator [] (BIT_TYPE pos);
		const_bit_reference operator [] (BIT_TYPE pos) const;
		
		N & operator ++ ();
		N & operator -- ();

		N & operator += (const N & rhs);
		N & operator -= (const N & rhs);
		N & operator *= (const N & rhs);
		
		N & operator &= (const N & rhs);
		N & operator |= (const N & rhs);
		N & operator ^= (const N & rhs);
		
		N operator ~ () const;
		N operator << (BIT_TYPE pos) const;
		N operator >> (BIT_TYPE pos) const;
		
		N & operator <<= (BIT_TYPE pos);
		N & operator >>= (BIT_TYPE pos);
		
		N & operator = (std::string_view num_str);
		template <typename INT> requires std::is_integral_v<INT> N & operator = (INT rhs);
};

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
class N::bit_reference_base_ {
	friend class N::bit_reference;
	friend class N::const_bit_reference;
	
	private:
		T & num_;
		const BIT_TYPE pos_;
		
	public:
		bit_reference_base_() = delete;
		bit_reference_base_ (T &&, BIT_TYPE) = delete;
		bit_reference_base_ (T & num, BIT_TYPE pos);
		
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

// declarations of sign_type and associated functions and types
namespace jmaths {

class sign_type {
	
	friend struct detail;
	friend struct calc;
		
	protected: // !!! moet protected zijn
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
		
		virtual bool is_zero() const = 0; // !!! niet nodig? !!!
		void flip_sign(); // !!! ook niet nodig? !!!

};

// !!! using sign_type::positive, sign_type::negative;

} // /namespace jmaths

// declarations of Z and associated functions and types
namespace jmaths {

std::ostream & operator << (std::ostream & os, const Z & z);
std::istream & operator >> (std::istream & is, Z & z);

Z operator + (const Z & lhs, const Z & rhs);
Z operator - (const Z & lhs, const Z & rhs);
Z operator * (const Z & lhs, const Z & rhs);
std::pair<Z, Z> operator / (const Z & lhs, const Z & rhs);

Z operator & (const Z & lhs, const Z & rhs);
Z operator | (const Z & lhs, const Z & rhs);
Z operator ^ (const Z & lhs, const Z & rhs);

bool operator == (const Z & lhs, const Z & rhs);
template <typename INT> requires std::is_integral_v<INT> bool operator == (const Z & lhs, INT rhs);
template <typename INT> requires std::is_integral_v<INT> bool operator == (INT lhs, const Z & rhs);

std::strong_ordering operator <=> (const Z & lhs, const Z & rhs);
template <typename INT> requires std::is_integral_v<INT> std::strong_ordering operator <=> (const Z & lhs, INT rhs);
template <typename INT> requires std::is_integral_v<INT> std::strong_ordering operator <=> (INT lhs, const Z & rhs);

class Z : public sign_type, private N {
	
	friend struct detail;
	friend struct calc;
	
	friend class Q;

	private:
		Z (N && n, sign_bool sign);
		Z (const N & n, sign_bool sign);
		
		size_t dynamic_size_() const;
		
	public:
		using N::bit_reference, N::const_bit_reference;
		using N::is_even, N::is_odd;
		using N::ctz, N::bits, N::operator bool, N::operator[];
		
		Z();
		Z (std::string_view num_str, unsigned base = DEFAULT_BASE);
		template <typename INT> requires std::is_integral_v<INT> Z (INT num);
		
		Z (const N & n);
		Z (N && n);
		
		const N & abs() const &;
		N && abs() &&;
		
		virtual bool is_zero() const;
		bool is_one() const;
		bool is_neg_one() const;
		
		size_t size() const; // size of this object in bytes
		
		std::string to_str (unsigned base) const; // convert to string in any base >= 2 and <= 64
		operator std::string() const; // convert to string in default base
		std::string to_base16() const; // convert to string in base 16 (assumes BASE is a power of 2)
		template <typename INT> requires std::is_integral_v<INT> std::optional<INT> fits_into() const;
		
		Z & operator ++ ();
		Z & operator -- ();

		Z & operator += (const Z & rhs);
		Z & operator -= (const Z & rhs);
		Z & operator *= (const Z & rhs);
		
		Z & operator &= (const Z & rhs);
		Z & operator |= (const Z & rhs);
		Z & operator ^= (const Z & rhs);
		
		Z operator - () const &;
		Z && operator - () &&;
		Z operator ~ () const;
		Z operator << (BIT_TYPE pos) const;
		Z operator >> (BIT_TYPE pos) const;
		
		Z & operator <<= (BIT_TYPE pos);
		Z & operator >>= (BIT_TYPE pos);
		
		Z & operator = (std::string_view num_str);
		template <typename INT> requires std::is_integral_v<INT> Z & operator = (INT rhs);
		
		Z & operator = (const N & n);
		Z & operator = (N && n);
};

} // /namespace jmaths

// declarations of Q and associated functions and types
namespace jmaths {

std::ostream & operator << (std::ostream & os, const Q & q);
std::istream & operator >> (std::istream & is, Q & q);

Q operator + (const Q & lhs, const Q & rhs);
Q operator - (const Q & lhs, const Q & rhs);
Q operator * (const Q & lhs, const Q & rhs);
Q operator / (const Q & lhs, const Q & rhs);

Q operator & (const Q & lhs, const Q & rhs);
Q operator | (const Q & lhs, const Q & rhs);
Q operator ^ (const Q & lhs, const Q & rhs);

bool operator == (const Q & lhs, const Q & rhs);
template <typename FLOAT> requires std::is_floating_point_v<FLOAT> bool operator == (const Q & lhs, FLOAT rhs);
template <typename FLOAT> requires std::is_floating_point_v<FLOAT> bool operator == (FLOAT lhs, const Q & rhs);

std::strong_ordering operator <=> (const Q & lhs, const Q & rhs);
template <typename FLOAT> requires std::is_floating_point_v<FLOAT> std::strong_ordering operator <=> (const Q & lhs, FLOAT rhs);
template <typename FLOAT> requires std::is_floating_point_v<FLOAT> std::strong_ordering operator <=> (FLOAT lhs, const Q & rhs);

class Q : public sign_type {
    
    friend struct detail;
    friend struct calc;
    
    private:
        N num_, denom_;
    
		Q (N && num, N && denom, sign_bool sign);
        Q (const N & num, const N & denom, sign_bool sign);
		
        void canonicalise();
		
		size_t dynamic_size_() const;
    
    public:
        Q();
        Q (std::string_view num_str, unsigned base = DEFAULT_BASE);
		template <typename INT> requires std::is_integral_v<INT> Q (INT num);
		
        Q (const N & n);
        Q (N && n);
        
		Q (const Z & z);
        Q (Z && z);
		
		Q (const N & num, const N & denom);
		Q (const N & num, N && denom);
		Q (N && num, const N & denom);
		Q (N && num, N && denom);
		
		Q (const Z & num, const Z & denom);
		Q (const Z & num, Z && denom);
		Q (Z && num, const Z & denom);
		Q (Z && num, Z && denom);
    
        virtual bool is_zero() const;
        bool is_one() const;
        bool is_neg_one() const;
		
		Q abs() const &;
		Q && abs() &&;
		
		Q inverse() const &;
		Q && inverse() &&;
		
		size_t size() const; // size of this object in bytes
		
		std::string to_str (unsigned base) const; // convert to string in any base >= 2 and <= 64
		operator std::string() const; // convert to string in default base
		std::string to_base16() const; // convert to string in base 16 (assumes BASE is a power of 2)
		explicit operator bool() const;
		template <typename FLOAT> requires std::is_floating_point_v<FLOAT> std::optional<FLOAT> fits_into() const;
		
		Q & operator ++ ();
		Q & operator -- ();

		Q & operator += (const Q & rhs);
		Q & operator -= (const Q & rhs);
		Q & operator *= (const Q & rhs);
		Q & operator /= (const Q & rhs);
		
		Q & operator &= (const Q & rhs);
		Q & operator |= (const Q & rhs);
		Q & operator ^= (const Q & rhs);
		
		Q operator - () const &;
		Q && operator - () &&;
		Q operator ~ () const;
		Q operator << (BIT_TYPE pos) const;
		Q operator >> (BIT_TYPE pos) const;
		
		Q & operator <<= (BIT_TYPE pos);
		Q & operator >>= (BIT_TYPE pos);
		
		Q & operator = (std::string_view num_str);
		template <typename INT> requires std::is_integral_v<INT> Q & operator = (INT rhs);
		
        Q & operator = (const N & n);
        Q & operator = (N && n);
        
		Q & operator = (const Z & z);
        Q & operator = (Z && z);
        
};

} // /namespace jmaths

// declarations of error and associated types
namespace jmaths {

class error : public std::exception {
	private:
		const char * const message_;
		static const char default_message_[];
			
	public:
		class division_by_zero;
		class invalid_base;
			
		error();
		explicit error (const char * message);
		virtual const char * what() const noexcept;
};
		
class error::division_by_zero : public error {
	private:
		static const char default_message_[];
		
	public:		
		division_by_zero();
		explicit division_by_zero (const char * message);
};
	
class error::invalid_base : public error {
	private:
		static const char default_message_[];
		
	public:
		invalid_base();
		explicit invalid_base (const char * message);
};

} // /namespace jmaths

// declarations of literals
namespace jmaths {

inline namespace literals {

N operator ""_N (const char * num_str);

Z operator ""_Z (const char * num_str);
	
}

} // /namespace jmaths

#include "../config/jmaths_undef.cfg"

#include "../src/jmaths_tmpl.cpp"
