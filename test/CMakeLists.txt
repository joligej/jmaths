# CMake 3.30+ prefers Boost's config package
if(POLICY CMP0167)
cmake_policy(SET CMP0167 NEW)
endif()

# Find Boost Test library
find_package(Boost 1.70 REQUIRED CONFIG COMPONENTS unit_test_framework)

# Main test executable with all test suites
add_executable(jmaths_unit_test)

# Collect all test source files
file(GLOB TEST_SOURCES
    test_main.cpp
    test_basic_n.cpp
    test_basic_z.cpp
    test_basic_q.cpp
    test_calc.cpp
    test_literals.cpp
    test_rand.cpp
    test_conversions.cpp
    test_operators.cpp
    test_bitwise.cpp
    test_edge_cases.cpp
    # test_parametric.cpp  # Temporarily disabled - Boost.Test compatibility issue with GCC 15
    test_constructors_comprehensive.cpp
    test_api_complete_coverage.cpp
    test_exceptions.cpp
    boost_test_gcc_abi_fix.cpp
)

target_sources(jmaths_unit_test PRIVATE ${TEST_SOURCES})

# Homebrew Boost is shared by default; keep this for Boost.Test
target_compile_definitions(jmaths_unit_test PRIVATE BOOST_TEST_DYN_LINK)

# Use imported targets
target_link_libraries(jmaths_unit_test
PRIVATE
compilation_flags
jmaths
Boost::unit_test_framework
)
