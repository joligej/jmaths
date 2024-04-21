#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>

#define TO_STRING_HELPER(x) #x
#define TO_STRING(x) TO_STRING_HELPER(x)

#include TO_STRING(DEFAULTS_LIST)

int main (int argc, char * argv[]) {
    namespace fs = std::filesystem;
    const bool default_state = argc > 1 && strcmp(argv[1], "DEFAULT") == 0;
    
    constexpr auto user_settings = TO_STRING(USER_SETTINGS);
    constexpr auto default_user_settings = TO_STRING(DEFAULT_USER_SETTINGS);
    
    fs::remove(user_settings);
    
    if (default_state) {
        fs::copy(default_user_settings, user_settings);
    } else {
        
        std::ofstream config_file (user_settings);
        
        std::cout << "\nCurrently writing to: " << user_settings << '\n';
        
        config_file << "#include <cstdint> // for uint*_t\n\n";
        
    DEFAULT_BASE:
        
        std::cout << "\nWhat would you like your default base to be?\nIf you don't enter anything, it will default to " TO_STRING(D_DEFAULT_BASE) ".\nEnter base: ";
        
        std::string default_base;
        std::getline(std::cin, default_base);
        
        config_file << "#define DEFAULT_BASE (unsigned)";
        if (default_base.empty()) config_file << TO_STRING(D_DEFAULT_BASE);
        else {
            try {
                std::stoull(default_base);
            } catch (...) {
                std::cout << "\nOops, that didn't work. Please try again.\n";
                goto DEFAULT_BASE;
            }
            config_file << default_base;
        }
        
        config_file << '\n';
        
        std::cout << "\nWhat would you like your base type to be?\nIf you don't enter anything, it will default to " TO_STRING(D_BASE_INT) ".\nEnter type: ";
        
        std::string base_int;
        std::getline(std::cin, base_int);
        
        config_file << "#define BASE_INT ";
        if (base_int.empty()) config_file << TO_STRING(D_BASE_INT);
        else config_file << base_int;
        
        config_file << '\n';
        
        std::cout << "\nWhat would you like your bigger-than-base-type type to be?\nIf you don't enter anything, it will default to " TO_STRING(D_BASE_INT_BIG) ".\nEnter type: ";
        
        std::string base_int_big;
        std::getline(std::cin, base_int_big);
        
        config_file << "#define BASE_INT_BIG ";
        if (base_int_big.empty()) config_file << TO_STRING(D_BASE_INT_BIG);
        else config_file << base_int_big;
        
        config_file << '\n';
        
    BASE:
        
        std::cout << "\nWhat is the base corresponding to your base type?\nIf you don't enter anything, it will default to " TO_STRING(D_BASE) ".\nEnter base: ";
        
        std::string base;
        std::getline(std::cin, base);
        
        config_file << "#define BASE (BASE_INT_BIG)";
        if (base.empty()) config_file << TO_STRING(D_BASE);
        else {
            try {
                std::stoull(base);
            } catch (...) {
                std::cout << "\nOops, that didn't work. Please try again.\n";
                goto BASE;
            }
            config_file << base;
        }
        
        config_file << '\n';
        
    MAX_RATIO:
        
        std::cout << "\nWhat is the ratio between your smallest type and your base type?\nIf you don't enter anything, it will default to " TO_STRING(D_MAX_RATIO) ".\nMake sure to enter a ratio a little smaller.\nEnter ratio: ";
        
        std::string max_ratio;
        std::getline(std::cin, max_ratio);
        
        config_file << "#define MAX_RATIO (double)";
        if (max_ratio.empty()) config_file << TO_STRING(D_MAX_RATIO);
        else {
            try {
                std::stold(max_ratio);
            } catch (...) {
                std::cout << "\nOops, that didn't work. Please try again.\n";
                goto MAX_RATIO;
            }
            config_file << max_ratio;
        }
        
        config_file << '\n';
        
        std::cout << "\nWhat is the type you would like to use to count bits?\nIf you don't enter anything, it will default to " TO_STRING(D_BIT_TYPE) ".\nEnter type: ";
        
        std::string bit_type;
        std::getline(std::cin, bit_type);
        
        config_file << "#define BIT_TYPE ";
        if (bit_type.empty()) config_file << TO_STRING(D_BIT_TYPE);
        else config_file << bit_type;
        
        config_file << '\n';
        
    BITS_IN_BYTE:
        
        std::cout << "\nWhat is the amount of bits in a byte on this particular system?\nIf you don't enter anything, it will default to " TO_STRING(D_BITS_IN_BYTE) ".\nEnter number: ";
        
        std::string bits_in_byte;
        std::getline(std::cin, bits_in_byte);
        
        config_file << "#define BITS_IN_BYTE (BIT_TYPE)";
        if (bits_in_byte.empty()) config_file << TO_STRING(D_BITS_IN_BYTE);
        else {
            try {
                std::stoull(bits_in_byte);
            } catch (...) {
                std::cout << "\nOops, that didn't work. Please try again.\n\n";
                goto BITS_IN_BYTE;
            }
            config_file << bits_in_byte;
        }
        
        config_file << '\n';
        
    }
    
    std::cout << "Configuration succesful\n";
    
    return 0;
}
