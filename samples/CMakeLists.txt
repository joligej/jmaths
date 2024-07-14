add_library(examples INTERFACE)
target_link_libraries(examples INTERFACE jmaths)

add_executable(fig_terminal_comparison)
target_sources(fig_terminal_comparison PRIVATE fig_terminal_comparison_example.cpp)
target_link_libraries (fig_terminal_comparison PRIVATE examples)

add_executable(fig_terminal_q_fits_into)
target_sources(fig_terminal_q_fits_into PRIVATE fig_terminal_q_fits_into_example.cpp)
target_link_libraries (fig_terminal_q_fits_into PRIVATE examples)

set(sample_targets
        fig_terminal_comparison
        fig_terminal_q_fits_into)

install(TARGETS ${sample_targets} EXPORT jmaths_targets DESTINATION lib/jmaths/samples/)
