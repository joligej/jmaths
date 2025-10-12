cmake_minimum_required(VERSION 3.28.1)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

project(jmaths
        VERSION 2.0
        DESCRIPTION "The jmaths library for C++"
        LANGUAGES CXX
        HOMEPAGE_URL "https://github.com/joligej/jmaths.git")

set(product_name "jmaths")

set(CMAKE_DEBUG_POSTFIX d)

add_library(compilation_flags INTERFACE)
target_compile_features(compilation_flags INTERFACE cxx_std_23)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(compilation_flags INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wpedantic;-Wshadow;-Werror;-Wuninitialized;-Winit-self;-Wconversion;-Wsign-conversion;-Wsign-compare>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W4;-WX>>")

if (NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm" OR NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    message(STATUS "Using address sanitizer with ${CMAKE_CXX_COMPILER_ID} on ${CMAKE_SYSTEM_PROCESSOR}")
    target_compile_options(compilation_flags INTERFACE "$<$<CONFIG:Debug>:-fsanitize=address;-fsanitize=undefined>")
    target_link_options(compilation_flags INTERFACE "$<$<CONFIG:Debug>:-fsanitize=address;-fsanitize=undefined>")
    target_link_libraries(compilation_flags INTERFACE "$<$<CONFIG:Debug>:-fsanitize=address;-fsanitize=undefined>")
else ()
    message(STATUS "Not using address sanitizer with ${CMAKE_CXX_COMPILER_ID} on ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

#include(CheckIPOSupported)
#check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

#if(ipo_supported)
#   message(STATUS "IPO / LTO supported in release builds")
#   set_property(TARGET compilation_flags PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE True)
#else()
#   message(STATUS "IPO / LTO not supported: <${ipo_error}>")
#endif()

install(TARGETS compilation_flags EXPORT jmathsTargets)

install(FILES "LICENSE" DESTINATION lib/jmaths/)

add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(test)
add_subdirectory(doc)

install(EXPORT jmathsTargets FILE jmathsTargets.cmake DESTINATION lib/cmake/jmaths/)

include(CMakePackageConfigHelpers)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/jmathsConfig.cmake"
        INSTALL_DESTINATION lib/cmake/jmaths/
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/jmathsConfigVersion.cmake"
        VERSION "${jmaths_VERSION_MAJOR}.${jmaths_VERSION_MINOR}"
        COMPATIBILITY AnyNewerVersion)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/jmathsConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/jmathsConfigVersion.cmake"
        DESTINATION lib/cmake/jmaths/)

export(EXPORT jmathsTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/jmathsTargets.cmake")

enable_testing()
add_test(NAME unit_test COMMAND jmaths_unit_test)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${jmaths_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${jmaths_VERSION_MINOR}")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
