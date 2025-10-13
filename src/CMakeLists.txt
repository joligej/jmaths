add_library(jmaths INTERFACE)

set(headers_dir "${CMAKE_CURRENT_SOURCE_DIR}/headers/")
file(GLOB_RECURSE header_files "${headers_dir}/*.hpp" "${headers_dir}/*.hh")

option(use_karatsuba "Use Karatsuba algorithm to multiply 2 numbers (this option is defective, please do not use it)" OFF)
option(periodically_reseed_rand "Periodically reseed the random number generator" OFF)
option(enable_benchmarking "Enable benchmarking for functions in the library" OFF)

if (use_karatsuba)
    target_compile_definitions(jmaths INTERFACE JMATHS_KARATSUBA=1)
else ()
    target_compile_definitions(jmaths INTERFACE JMATHS_KARATSUBA=0)
endif ()

if (periodically_reseed_rand)
    target_compile_definitions(jmaths INTERFACE JMATHS_PERIODICALLY_RESEED_RAND=1)
else ()
    target_compile_definitions(jmaths INTERFACE JMATHS_PERIODICALLY_RESEED_RAND=0)
endif ()

if (enable_benchmarking)
    target_compile_definitions(jmaths INTERFACE JMATHS_BENCHMARKING=1)
else ()
    target_compile_definitions(jmaths INTERFACE JMATHS_BENCHMARKING=0)
endif ()

configure_file("${headers_dir}/constants_and_types.hpp.in" constants_and_types.hpp)

target_include_directories(jmaths INTERFACE
        "$<BUILD_INTERFACE:${headers_dir}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:include/jmaths/>")

install(TARGETS jmaths EXPORT jmathsTargets DESTINATION lib/)
install(FILES ${header_files} "${CMAKE_CURRENT_BINARY_DIR}/constants_and_types.hpp" DESTINATION include/jmaths/)
