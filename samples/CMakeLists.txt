add_library(examples INTERFACE)
target_link_libraries(examples INTERFACE compilation_flags jmaths)

add_executable(fig_terminal_comparison)
target_sources(fig_terminal_comparison PRIVATE fig_terminal_comparison_example.cpp)
target_link_libraries(fig_terminal_comparison PRIVATE examples)
list(APPEND sample_targets fig_terminal_comparison)

add_executable(fig_terminal_q_fits_into)
target_sources(fig_terminal_q_fits_into PRIVATE fig_terminal_q_fits_into_example.cpp)
target_link_libraries(fig_terminal_q_fits_into PRIVATE examples)
list(APPEND sample_targets fig_terminal_q_fits_into)

add_executable(fig_terminal_z_random)
target_sources(fig_terminal_z_random PRIVATE fig_terminal_z_random_example.cpp)
target_link_libraries(fig_terminal_z_random PRIVATE examples)
list(APPEND sample_targets fig_terminal_z_random)

add_executable(my_little_sample)
target_sources(my_little_sample PRIVATE my_little_sample.cpp)
target_link_libraries(my_little_sample PRIVATE examples)

install(TARGETS ${sample_targets} EXPORT jmathsTargets DESTINATION lib/jmaths/samples/)
