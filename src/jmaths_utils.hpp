#pragma once

#include <iostream>
#include <chrono>

namespace jmaths {

struct utils {

class function_timer {
    private:
        typedef std::chrono::high_resolution_clock clock_type;

        const char * const function_name_;
        const clock_type::time_point start_time_;

    public:
        function_timer (const char * function_name) : function_name_{function_name}, start_time_{clock_type::now()} {
            std::cerr << "call\t" << function_name_ << '\n';
        }

        ~function_timer() {
            const clock_type::time_point end_time = clock_type::now();
            const std::chrono::duration<double, std::milli> total_time = end_time - start_time_;
            
            std::cerr << "time\t" << total_time.count() << "ms\tfrom\t" << function_name_ << '\n';
        }
};

} // /namespace struct utils

} // /namespace jmaths
