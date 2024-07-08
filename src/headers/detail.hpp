#pragma once

#include <ostream>
#include <istream>
#include <utility>
#include <compare>
#include <type_traits>

#include "declarations.hpp"

// declarations of namespace structs calc and detail
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

struct test;

}; // /namespace struct detail

} // /namespace jmaths
