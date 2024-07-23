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

#ifndef NDEBUG
    #include <source_location>

    #include "function_timer.hpp"
    #define FUNCTION_TO_LOG \
        ::jmaths::internal::function_timer TIME_THIS_FUNCTION(::std::source_location::current().function_name())
#else
    #define FUNCTION_TO_LOG ((void)0)
#endif

#include <type_traits>

#define CONCAT2HELP(x, y) x##y
#define CONCAT2(x, y) CONCAT2HELP(x, y)

#define REPEAT(x)                                                                             \
    for (::std::remove_cvref_t<decltype(x)> CONCAT2(FOR_LOOP_REPETITION_COUNTER, __LINE__){}; \
         CONCAT2(FOR_LOOP_REPETITION_COUNTER, __LINE__) < x;                                  \
         ++CONCAT2(FOR_LOOP_REPETITION_COUNTER, __LINE__))
