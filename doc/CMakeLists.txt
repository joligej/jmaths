file(GLOB documentation_files manual.*)

install(FILES ${documentation_files} DESTINATION lib/jmaths/doc/)
