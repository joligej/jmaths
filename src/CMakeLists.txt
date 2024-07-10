add_library(jmaths STATIC)

file(GLOB src_files ${CMAKE_CURRENT_SOURCE_DIR}/*)

target_sources(jmaths PRIVATE ${src_files})

set(UNITY_BUILD True)

option(use_karatsuba "Use Karatsuba algorithm to multiply 2 numbers (this option is defective, please do not use it)" OFF)

if(use_karatsuba)
    target_compile_definitions(jmaths PRIVATE KARATSUBA=1)
else()
    target_compile_definitions(jmaths PRIVATE KARATSUBA=0)
endif()

target_include_directories(jmaths INTERFACE
        $<BUILD_INTERFACE:${headers_dir}>
        $<INSTALL_INTERFACE:include/jmaths/>)

install(TARGETS jmaths EXPORT jmaths_targets DESTINATION lib/)
install(FILES ${header_files} DESTINATION include/jmaths/)
install(EXPORT jmaths_targets FILE jmaths_targets.cmake DESTINATION lib/cmake/jmaths/)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE.txt)
set(CPACK_PACKAGE_VERSION_MAJOR ${jmaths_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${jmaths_VERSION_MINOR})
set(CPACK_GENERATOR TGZ)
set(CPACK_SOURCE_GENERATOR TGZ)
include(CPack)
