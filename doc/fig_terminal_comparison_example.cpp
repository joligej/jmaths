#include <iostream>
#include <string>
#include "/usr/local/jmaths/jmaths.hpp"

int main() {
    std::string input_str;
    std::cout << "Enter your number:\n";
    std::cin >> input_str;

    using jmaths::Z, jmaths::calc;

    Z z (input_str);
    std::cout << "z == " << z << '\n';
    ++z;
    std::cout << "z + 1 == " << z << '\n';
    z = calc::pow(z, 2); // or z *= z;
    std::cout << "(z + 1)^2 == " << z << '\n';
}
