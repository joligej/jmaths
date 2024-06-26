#pragma once

#ifndef PREPROCESSING_HEADER
    #include "../dep/dependencies.hpp"
#endif

namespace jmaths {

struct utils {

class function_timer {
    private:
        using clock_type = std::chrono::high_resolution_clock;

        const char * const function_name_;
        const clock_type::time_point start_time_;

    public:
        function_timer (const char * function_name) : function_name_{function_name}, start_time_{clock_type::now()} {
            std::clog << "log:call\t" << function_name_ << '\n';
        }

        ~function_timer() {
            const clock_type::time_point end_time = clock_type::now();
            const std::chrono::duration<double, std::milli> total_time = end_time - start_time_;

            std::clog << "log:time\t" << total_time.count() << "\tms\tfrom\t" << function_name_ << '\n';
        }
};

}; // /namespace struct utils

} // /namespace jmaths
