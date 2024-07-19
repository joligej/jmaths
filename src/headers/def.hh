#ifndef NDEBUG
    #include <source_location>

    #include "function_timer.hpp"
    #define FUNCTION_TO_LOG                                    \
        ::jmaths::internal::function_timer TIME_THIS_FUNCTION( \
            ::std::source_location::current().function_name())
#else
    #define FUNCTION_TO_LOG ((void)0)
#endif

#define CONCAT2HELP(x, y) x##y
#define CONCAT2(x, y) CONCAT2HELP(x, y)

#define REPEAT(x)                                            \
    for (::std::remove_cvref_t<decltype(x)> CONCAT2(         \
             FOR_LOOP_REPETITION_COUNTER,                    \
             __LINE__){};                                    \
         CONCAT2(FOR_LOOP_REPETITION_COUNTER, __LINE__) < x; \
         ++CONCAT2(FOR_LOOP_REPETITION_COUNTER, __LINE__))
