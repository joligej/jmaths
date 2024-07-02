#include <iostream>
#include <fstream>
#include <bit>

#define TO_STRING_HELPER(x) #x
#define TO_STRING(x) TO_STRING_HELPER(x)

int main() {
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
