file(GLOB documentation_files "*.md")

install(FILES ${documentation_files} LICENSE.txt DESTINATION lib/jmaths/doc/)
