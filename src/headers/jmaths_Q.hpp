#pragma once

#include <ostream>
#include <istream>
#include <type_traits>
#include <compare>
#include <string>
#include <string_view>
#include <tuple>
#include <cstddef>
#include <optional>

#include "jmaths_constants_and_types.hpp"
#include "jmaths_declarations.hpp"
#include "jmaths_sign_type.hpp"
#include "jmaths_N.hpp"
#include "jmaths_hash.hpp"

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
	friend struct std::hash<Q>;

    private:
        N num_, denom_;

		Q (N && num, N && denom, sign_bool sign);
        Q (const N & num, const N & denom, sign_bool sign);
		Q (std::tuple<N, N, sign_bool> && fraction_info);

        void canonicalise_();

		template <typename FLOAT> requires std::is_floating_point_v<FLOAT> static std::tuple<N, N, sign_bool> handle_float_ (FLOAT num);

		std::size_t dynamic_size_() const;

    public:
        Q();
        Q (std::string_view num_str, unsigned base = default_base);
		template <typename FLOAT> requires std::is_floating_point_v<FLOAT> Q (FLOAT num);

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

		std::size_t size() const; // size of this object in bytes

		std::string to_str (unsigned base) const; // convert to string in any base >= 2 and <= 64
		operator std::string() const; // convert to string in default base
		std::string to_hex() const; // convert to string in base 16 (assumes base is a power of 2)
		explicit operator bool() const;
		template <typename FLOAT> requires std::is_floating_point_v<FLOAT> && std::numeric_limits<FLOAT>::is_iec559 std::optional<FLOAT> fits_into() const;

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
		Q operator << (bit_type pos) const;
		Q operator >> (bit_type pos) const;

		Q & operator <<= (bit_type pos);
		Q & operator >>= (bit_type pos);

		Q & operator = (std::string_view num_str);
		template <typename FLOAT> requires std::is_floating_point_v<FLOAT> Q & operator = (FLOAT rhs);

        Q & operator = (const N & n);
        Q & operator = (N && n);

		Q & operator = (const Z & z);
        Q & operator = (Z && z);

};

} // /namespace jmaths

#include "jmaths_tmpl_Q.hpp"
