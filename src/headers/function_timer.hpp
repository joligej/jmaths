#pragma once

#include <chrono>
#include <iostream>

namespace jmaths {

namespace internal {

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
        const std::chrono::duration<double, std::milli> total_time =
            end_time - start_time_;

        std::clog << "log:time\t" << total_time.count() << "\tms\tfrom\t"
                  << function_name_ << '\n';
    }
};

};  // namespace internal

}  // namespace jmaths
