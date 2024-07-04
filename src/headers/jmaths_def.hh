#ifndef NDEBUG
    #define FUNCTION_TO_LOG \
    jmaths::utils::function_timer TIME_THIS_FUNCTION (std::source_location::current().function_name())
#else
    #define FUNCTION_TO_LOG ((void)0)
#endif
