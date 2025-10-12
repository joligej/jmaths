// The jmaths library for C++
// Copyright (C) 2025  Jasper de Smaele

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

/**
 * @file scoped_timer.hpp
 * @brief RAII-based function timing utility for performance profiling
 *
 * This file provides a scoped timer that automatically measures and logs
 * the execution time of functions when JMATHS_BENCHMARKING is enabled.
 *
 * DESIGN: Uses RAII (Resource Acquisition Is Initialization) pattern to
 * automatically start timing on construction and log results on destruction.
 */

namespace jmaths::internal {

/**
 * @class scoped_timer
 * @brief RAII timer that logs function calls and execution times
 *
 * USAGE: Automatically instantiated by JMATHS_FUNCTION_TO_LOG macro
 * when JMATHS_BENCHMARKING is enabled.
 *
 * LOGGING FORMAT:
 * - On construction: "log:call\t<function_name>"
 * - On destruction: "log:time\t<duration_ms>\tms\tfrom\t<function_name>"
 *
 * OUTPUT: Logs to std::clog by default, configurable via set_ostream()
 */
class scoped_timer {
   public:
    /**
     * @brief Constructor - starts timing and logs function call
     * @param timed_obj_name Name of function being timed (usually from __func__ or source_location)
     *
     * ALGORITHM: Records start time using high_resolution_clock and logs call
     */
    explicit scoped_timer(const char * timed_obj_name) :
        timed_obj_name_{timed_obj_name}, start_time_(clock_type::now()) {
        if (ostream_ == nullptr) { return; }

        *ostream_ << "log:call\t" << timed_obj_name_ << '\n';
    }

    /**
     * @brief Destructor - calculates duration and logs execution time
     *
     * ALGORITHM: Computes elapsed time in milliseconds and logs result
     * This is automatically called when scope exits, providing accurate timing
     */
    ~scoped_timer() {
        if (ostream_ == nullptr) { return; }

        const clock_type::time_point end_time = clock_type::now();
        const std::chrono::duration<double, std::milli> total_time = end_time - start_time_;

        *ostream_ << "log:time\t" << total_time.count() << "\tms\tfrom\t" << timed_obj_name_ << '\n';
    }

    /**
     * @brief Configure output stream for logging
     * @param ostream Pointer to output stream (nullptr disables logging)
     *
     * NOTE: Default is std::clog. Can be set to file stream for permanent logs.
     */
    static void set_ostream(std::ostream * ostream) {
        ostream_ = ostream;
    }

   private:
    using clock_type = std::chrono::high_resolution_clock;

    const char * const timed_obj_name_;  ///< Name of function being timed
    const clock_type::time_point start_time_;  ///< Time when timer was created

    inline static std::ostream * ostream_ = std::addressof(std::clog);  ///< Output stream (shared)
};

}  // namespace jmaths::internal
