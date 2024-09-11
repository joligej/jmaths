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
#include <memory>
#include <ostream>

namespace jmaths::internal {

class scoped_timer {
   public:
    explicit scoped_timer(const char * timed_obj_name) :
        timed_obj_name_{timed_obj_name}, start_time_(clock_type::now()) {
        if (ostream_ == nullptr) { return; }

        *ostream_ << "log:call\t" << timed_obj_name_ << '\n';
    }

    ~scoped_timer() {
        if (ostream_ == nullptr) { return; }

        const clock_type::time_point end_time = clock_type::now();
        const std::chrono::duration<double, std::milli> total_time = end_time - start_time_;

        *ostream_ << "log:time\t" << total_time.count() << "\tms\tfrom\t" << timed_obj_name_ << '\n';
    }

    static void set_ostream(std::ostream * ostream) {
        ostream_ = ostream;
    }

   private:
    using clock_type = std::chrono::high_resolution_clock;

    const char * const timed_obj_name_;
    const clock_type::time_point start_time_;

    inline static std::ostream * ostream_ = std::addressof(std::clog);
};

}  // namespace jmaths::internal
