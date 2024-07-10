cmake_minimum_required(VERSION 3.28.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

add_compile_options(
    "$<${gcc_like_cxx}:-Wall;-Wextra;-Wpedantic;-Werror>"
    "$<${msvc_cxx}:-W4;-WX>")

project(jmaths VERSION 1.0 DESCRIPTION "The jmaths library for C++" LANGUAGES CXX)

set(headers_dir ${PROJECT_SOURCE_DIR}/src/headers/)
file(GLOB_RECURSE header_files ${headers_dir}/*)

add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(test)
enable_testing()
add_test(NAME unit_test COMMAND jmaths_unit_test)
