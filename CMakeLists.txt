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

set(UNITY_BUILD True)

option(use_karatsuba "Use Karatsuba algorithm to multiply 2 numbers (this option is defective, please do not use it)" OFF)

if(use_karatsuba)
    target_compile_definitions(jmaths PRIVATE KARATSUBA=1)
else()
    target_compile_definitions(jmaths PRIVATE KARATSUBA=0)
endif()

target_include_directories(jmaths INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

install(TARGETS jmaths EXPORT jmaths_targets DESTINATION lib/jmaths)
install(FILES ${header_files} DESTINATION include/jmaths)
install(EXPORT jmaths_targets FILE jmaths_targets.cmake DESTINATION lib/cmake/jmaths)

include(InstallRequiredSystemLibraries)
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${jmaths_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${jmaths_VERSION_MINOR}")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

#add_executable(unit_test ${PROJECT_SOURCE_DIR}/test/unit_test.cpp)
#target_link_libraries(unit_test PUBLIC jmaths)
#target_include_directories(unit_test PUBLIC ${PROJECT_SOURCE_DIR}/src/headers/)
