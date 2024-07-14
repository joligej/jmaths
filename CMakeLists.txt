cmake_minimum_required(VERSION 3.28.1)

project(jmaths VERSION 1.0 DESCRIPTION "The jmaths library for C++" LANGUAGES CXX)

set(product_name "jmaths")

add_library(compilation_flags INTERFACE)
target_compile_features(compilation_flags INTERFACE cxx_std_23)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(compilation_flags INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wpedantic;-Wshadow;-Werror>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W4;-WX>>")

install(TARGETS compilation_flags EXPORT jmaths_targets)

install(FILES LICENSE.txt DESTINATION lib/jmaths/)

add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(test)
add_subdirectory(doc)

enable_testing()
add_test(NAME unit_test COMMAND jmaths_unit_test)
