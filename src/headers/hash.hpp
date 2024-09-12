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

#include "declarations.hpp"

// declarations of std::hash specialisations

template <> struct std::hash<jmaths::N> {
    std::size_t operator()(const jmaths::N & n) const;
};

template <> struct std::hash<jmaths::Z> {
    std::size_t operator()(const jmaths::Z & z) const;
};

template <> struct std::hash<jmaths::Q> {
    std::size_t operator()(const jmaths::Q & q) const;
};

#include "hash_impl.hpp"
