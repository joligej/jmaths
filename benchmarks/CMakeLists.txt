# Benchmarking executable
# Only built when benchmarking is enabled

option(enable_benchmarking "Enable benchmarking (adds performance measurement overhead)" OFF)

if(enable_benchmarking)
    message(STATUS "Benchmarking: ENABLED")

    # Create the comprehensive benchmark executable
    add_executable(benchmark_comprehensive benchmark_comprehensive.cpp)

    target_link_libraries(benchmark_comprehensive
        PRIVATE
        jmaths
        compilation_flags
    )

    # Install benchmark executable
    install(TARGETS benchmark_comprehensive DESTINATION bin)

    # Install Python analysis script
    install(PROGRAMS analyze_benchmark.py
            DESTINATION bin
            RENAME jmaths_analyze_benchmark.py)

    # Install benchmarking guide
    install(FILES BENCHMARKING_GUIDE.md DESTINATION share/doc/jmaths/)

else()
    message(STATUS "Benchmarking: DISABLED (use -Denable_benchmarking=ON to enable)")
endif()
