#include <iostream>
#include <fstream>
//#include <cstring>
#include <bit>

#define TO_STRING_HELPER(x) #x
#define TO_STRING(x) TO_STRING_HELPER(x)

int main (/*int argc, char * argv[]*/) {
    //const bool default_state = argc > 1 && strcmp(argv[1], "DEFAULT") == 0;
    // not necessary anymore since the cleanup

    constexpr auto settings_file_name = TO_STRING(SETTINGS_FILE);

    std::ofstream settings_file (settings_file_name);

    std::cout << "Currently writing to: " << settings_file_name << '\n';

    if constexpr (std::endian::native == std::endian::big) {
        settings_file << "#define NATIVELY_BIG_ENDIAN\n";
    }
    if constexpr (std::endian::native == std::endian::little) {
        settings_file << "#define NATIVELY_LITTLE_ENDIAN\n";
    }

    std::cout << "Configuration succesful\n";
}
