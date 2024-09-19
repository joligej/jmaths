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

#include <cstddef>
#include <functional>

#include "TMP.hpp"
#include "declarations.hpp"

// declarations of std::hash specialisations

template <jmaths::TMP::instance_of<jmaths::basic_N> basic_N_type> struct std::hash<basic_N_type> {
    static std::size_t operator()(const basic_N_type & n);
};

template <jmaths::TMP::instance_of<jmaths::basic_Z> basic_Z_type> struct std::hash<basic_Z_type> {
    static std::size_t operator()(const basic_Z_type & z);
};

template <jmaths::TMP::instance_of<jmaths::basic_Q> basic_Q_type> struct std::hash<basic_Q_type> {
    static std::size_t operator()(const basic_Q_type & q);
};

#include "hash_impl.hpp"
