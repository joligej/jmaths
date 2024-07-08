#ifndef NDEBUG
    #include <source_location>
    #include "jmaths_function_timer.hpp"
    #define FUNCTION_TO_LOG \
    ::jmaths::internal::function_timer TIME_THIS_FUNCTION (::std::source_location::current().function_name())
#else
    #define FUNCTION_TO_LOG ((void)0)
#endif

#define REPEAT(x) \
for (::std::remove_cv_t<decltype(x)> FOR_LOOP_REPETITION_COUNTER = 0; FOR_LOOP_REPETITION_COUNTER < x; ++FOR_LOOP_REPETITION_COUNTER)
