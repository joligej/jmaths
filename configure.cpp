#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ofstream config_file (USER_SETTINGS);
    
    std::cout << "\nCurrently writing to: " << USER_SETTINGS << '\n';
    
    config_file << "#include <cstdint> // for uint*_t\n\n";
    
    DEFAULT_BASE:
    
    std::cout << "\nWhat would you like your default base to be?\nIf you don't enter anything, it will default to 10.\nEnter base: ";
    
    std::string default_base;
    std::getline(std::cin, default_base);
    
    config_file << "#define DEFAULT_BASE (unsigned)";
    if (default_base.empty()) config_file << 10;
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
    
    std::cout << "\nWhat would you like your base type to be?\nIf you don't enter anything, it will default to uint16_t.\nEnter type: ";
    
    std::string base_int;
    std::getline(std::cin, base_int);
    
    config_file << "#define BASE_INT ";
    if (base_int.empty()) config_file << "uint16_t";
    else config_file << base_int;
    
    config_file << '\n';
    
    std::cout << "\nWhat would you like your bigger-than-base-type type to be?\nIf you don't enter anything, it will default to uint32_t.\nEnter type: ";
    
    std::string base_int_big;
    std::getline(std::cin, base_int_big);
    
    config_file << "#define BASE_INT_BIG ";
    if (base_int_big.empty()) config_file << "uint32_t";
    else config_file << base_int_big;
    
    config_file << '\n';
    
    BASE:
    
    std::cout << "\nWhat is the base corresponding to your base type?\nIf you don't enter anything, it will default to 65,536.\nEnter base: ";
    
    std::string base;
    std::getline(std::cin, base);
    
    config_file << "#define BASE (BASE_INT_BIG)";
    if (base.empty()) config_file << "65'536";
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
    
    std::cout << "\nWhat is the ratio between your smallest type and your base type?\nIf you don't enter anything, it will default to 0.000125.\nMake sure to enter a ratio a little smaller.\nEnter ratio: ";
    
    std::string max_ratio;
    std::getline(std::cin, max_ratio);
    
    config_file << "#define MAX_RATIO (double)";
    if (max_ratio.empty()) config_file << "0.000125";
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
    
    std::cout << "\nWhat is the type you would like to use to count bits?\nIf you don't enter anything, it will default to unsigned long long.\nEnter type: ";
    
    std::string bit_type;
    std::getline(std::cin, bit_type);
    
    config_file << "#define BIT_TYPE ";
    if (bit_type.empty()) config_file << "unsigned long long";
    else config_file << bit_type;
    
    config_file << '\n';
    
    BITS_IN_BYTE:
    
    std::cout << "\nWhat is the amount of bits in a byte on this particular system?\nIf you don't enter anything, it will default to 8.\nEnter number: ";
    
    std::string bits_in_byte;
    std::getline(std::cin, bits_in_byte);
    
    config_file << "#define BITS_IN_BYTE (BIT_TYPE)";
    if (bits_in_byte.empty()) config_file << "8";
    else {
        try {
            std::stoull(bits_in_byte);
        } catch (...) {
            std::cout << "\nOops, that didn't work. Please try again.\n";
            goto BITS_IN_BYTE;
        }
        config_file << bits_in_byte;
    }
    
    config_file << '\n';
    
    std::cout << "\nConfiguration succesful!\n\n";
    
    return 0;
}
