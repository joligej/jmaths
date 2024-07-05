cmake_minimum_required(VERSION 3.28.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

add_compile_options(
    "$<${gcc_like_cxx}:-Wall;-Wextra;-Wpedantic;-Werror>"
    "$<${msvc_cxx}:-W4;-WX>")

project(jmaths VERSION 1.0 DESCRIPTION "The jmaths library for C++" LANGUAGES CXX)

file(GLOB src_files ${PROJECT_SOURCE_DIR}/src/*.c*)
file(GLOB header_files ${PROJECT_SOURCE_DIR}/src/headers/*.h*)

add_library(jmaths STATIC ${src_files})

option(use_karatsuba "Use Karatsuba algorithm to multiply 2 numbers (this option is defective, please do not use it)" OFF)

if(use_karatsuba)
    target_compile_definitions(jmaths PRIVATE KARATSUBA=1)
else()
    target_compile_definitions(jmaths PRIVATE KARATSUBA=0)
endif()

install(TARGETS jmaths DESTINATION lib/jmaths)
install(FILES ${header_files} DESTINATION include/jmaths)
