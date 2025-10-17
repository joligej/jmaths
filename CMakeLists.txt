cmake_minimum_required(VERSION 3.28.1)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Force g++-15 as the compiler (must be before project())
if(NOT DEFINED CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER "g++-15")
endif()

# Check binutils version for GCC 15 compatibility
execute_process(
    COMMAND as --version
    OUTPUT_VARIABLE BINUTILS_VERSION_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

string(REGEX MATCH "[0-9]+\\.[0-9]+" BINUTILS_VERSION "${BINUTILS_VERSION_OUTPUT}")

message(STATUS "Using ${CMAKE_CXX_COMPILER}")
if(BINUTILS_VERSION)
    message(STATUS "Detected binutils version: ${BINUTILS_VERSION}")
else()
    set(BINUTILS_VERSION "unknown")
    message(STATUS "Could not detect binutils version")
endif()

# GCC 15 + binutils compatibility check
if(CMAKE_CXX_COMPILER MATCHES "g\\+\\+-15" OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 15))
    if(BINUTILS_VERSION STREQUAL "unknown")
        message(STATUS "GCC 15 detected - ensure binutils 2.43+ is available for Debug builds")
    elseif(BINUTILS_VERSION VERSION_LESS "2.43")
        message(WARNING "═════════════════════════════════════════════════════════════")
        message(WARNING " GCC 15 with binutils < 2.43 detected")
        message(WARNING " Your binutils version: ${BINUTILS_VERSION}")
        message(WARNING " ")
        message(WARNING " Debug builds may fail with '.base64' assembler errors")
        message(WARNING " ")
        message(WARNING " Solutions:")
        message(WARNING "   1. Use Release builds (works with any binutils)")
        message(WARNING "   2. Upgrade binutils to 2.43+")
        message(WARNING "   3. Use GCC 14: cmake .. -DCMAKE_CXX_COMPILER=g++-14")
        message(WARNING " ")
        message(WARNING " See GCC15_REQUIREMENTS.md for details")
        message(WARNING "═════════════════════════════════════════════════════════════")
    else()
        message(STATUS "GCC 15 + binutils ${BINUTILS_VERSION}: ✓ Full compatibility")
    endif()
endif()

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

# Sanitizers - Re-enable when needed
# Note: Sanitizers work fine with GCC 15 + binutils 2.43+
# Disabled by default to avoid confusion with older toolchains

message(STATUS "Sanitizers disabled by default (can be enabled with appropriate toolchain)")

install(TARGETS compilation_flags EXPORT jmathsTargets)

install(FILES "LICENSE" DESTINATION lib/jmaths/)

add_subdirectory(src)
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
