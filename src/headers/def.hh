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

#ifndef JMATHS_BENCHMARKING
    #error "JMATHS_BENCHMARKING is not defined, please define it when compiling!"
#endif

#if JMATHS_BENCHMARKING
    #include <source_location>

    #include "scoped_timer.hpp"

    #define JMATHS_FUNCTION_TO_LOG                                                \
        static constexpr const char * JMATHS_MACRO_IMPL_FUNCTION_NAME =           \
            ::std::source_location::current().function_name();                    \
        ::jmaths::internal::scoped_timer JMATHS_MACRO_IMPL_TIME_THIS_FUNCTION (   \
            JMATHS_MACRO_IMPL_FUNCTION_NAME                                       \
        )
#else
    #define JMATHS_FUNCTION_TO_LOG ((void)0)
#endif

#include <type_traits>

#define JMATHS_MACRO_IMPL_CONCAT2_HELP(x, y) x##y
#define JMATHS_CONCAT2(x, y) JMATHS_MACRO_IMPL_CONCAT2_HELP(x, y)

#define JMATHS_REPEAT(x)                                                                \
    for (::std::enable_if_t<not ::std::is_pointer_v<::std::decay_t<decltype(x)>>,       \
                            ::std::decay_t<decltype(x)>>                                \
             JMATHS_CONCAT2(JMATHS_MACRO_IMPL_FOR_LOOP_REPETITION_COUNTER, __LINE__){}; \
         JMATHS_CONCAT2(JMATHS_MACRO_IMPL_FOR_LOOP_REPETITION_COUNTER, __LINE__) < (x); \
         ++JMATHS_CONCAT2(JMATHS_MACRO_IMPL_FOR_LOOP_REPETITION_COUNTER, __LINE__))
