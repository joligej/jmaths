# CMake 3.30+ prefers Boost's config package; 'system' is header-only now.
if(POLICY CMP0167)
	cmake_policy(SET CMP0167 NEW)
endif()

# Do NOT request 'system'; keep filesystem + unit_test_framework.
find_package(Boost 1.70 REQUIRED CONFIG COMPONENTS filesystem unit_test_framework)

add_executable(jmaths_unit_test)
target_sources(jmaths_unit_test PRIVATE test.cpp)

# Homebrew Boost is shared by default; keep this for Boost.Test
target_compile_definitions(jmaths_unit_test PRIVATE BOOST_TEST_DYN_LINK)

# Use imported targets instead of old ${Boost_*} variables.
target_link_libraries(jmaths_unit_test
		PRIVATE
		compilation_flags
		jmaths
		Boost::filesystem
		Boost::unit_test_framework
)
