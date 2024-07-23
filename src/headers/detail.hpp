// The jmaths library for C++
// Copyright (C) 2024  Jasper de Smaele

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <compare>
#include <concepts>
#include <istream>
#include <ostream>
#include <utility>

#include "declarations.hpp"

namespace jmaths {

struct detail {
    static std::ostream & opr_ins(std::ostream & os, const N & n);
    static std::istream & opr_extr(std::istream & is, N & n);

    static N opr_add(const N & lhs, const N & rhs);
    static N opr_subtr(N lhs, const N & rhs);
    static N opr_mult(const N & lhs, const N & rhs);
    static std::pair<N, N> opr_div(const N & lhs, const N & rhs);

    static N opr_and(const N & lhs, const N & rhs);
    static N opr_or(const N & lhs, const N & rhs);
    static N opr_xor(const N & lhs, const N & rhs);

    static bool opr_eq(const N & lhs, const N & rhs);
    static bool opr_eq(const N & lhs, std::integral auto rhs);

    static std::strong_ordering opr_comp(const N & lhs, const N & rhs);
    static std::strong_ordering opr_comp(const N & lhs, std::integral auto rhs);

    static std::ostream & opr_ins(std::ostream & os, const Z & z);
    static std::istream & opr_extr(std::istream & is, Z & z);

    static Z opr_add(const Z & lhs, const Z & rhs);
    static Z opr_subtr(const Z & lhs, const Z & rhs);
    static Z opr_mult(const Z & lhs, const Z & rhs);
    static std::pair<Z, Z> opr_div(const Z & lhs, const Z & rhs);

    static Z opr_and(const Z & lhs, const Z & rhs);
    static Z opr_or(const Z & lhs, const Z & rhs);
    static Z opr_xor(const Z & lhs, const Z & rhs);

    static bool opr_eq(const Z & lhs, const Z & rhs);
    static bool opr_eq(const Z & lhs, std::integral auto rhs);

    static std::strong_ordering opr_comp(const Z & lhs, const Z & rhs);
    static std::strong_ordering opr_comp(const Z & lhs, std::integral auto rhs);

    static std::ostream & opr_ins(std::ostream & os, const Q & q);
    static std::istream & opr_extr(std::istream & is, Q & q);

    static Q opr_add(const Q & lhs, const Q & rhs);
    static Q opr_subtr(const Q & lhs, const Q & rhs);
    static Q opr_mult(const Q & lhs, const Q & rhs);
    static Q opr_div(const Q & lhs, const Q & rhs);

    static Q opr_and(const Q & lhs, const Q & rhs);
    static Q opr_or(const Q & lhs, const Q & rhs);
    static Q opr_xor(const Q & lhs, const Q & rhs);

    static bool opr_eq(const Q & lhs, const Q & rhs);
    static bool opr_eq(const Q & lhs, std::floating_point auto rhs);

    static std::strong_ordering opr_comp(const Q & lhs, const Q & rhs);
    static std::strong_ordering opr_comp(const Q & lhs, std::floating_point auto rhs);
};  // namespace struct detail

}  // namespace jmaths
