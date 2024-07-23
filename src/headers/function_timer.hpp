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

#include <chrono>
#include <iostream>

namespace jmaths::internal {

class function_timer {
   private:
    using clock_type = std::chrono::high_resolution_clock;

    const char * const function_name_;
    const clock_type::time_point start_time_;

   public:
    explicit function_timer(const char * function_name) :
        function_name_{function_name}, start_time_{clock_type::now()} {
        std::clog << "log:call\t" << function_name_ << '\n';
    }

    ~function_timer() {
        const clock_type::time_point end_time = clock_type::now();
        const std::chrono::duration<double, std::milli> total_time = end_time - start_time_;

        std::clog << "log:time\t" << total_time.count() << "\tms\tfrom\t" << function_name_ << '\n';
    }
};

};  // namespace jmaths::internal
