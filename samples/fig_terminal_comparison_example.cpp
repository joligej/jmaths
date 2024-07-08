#include <iostream>
#include <string>
#include "jmaths/all.hpp"

/********************
compiled with:
-std=c++2b -I/usr/local/include/ -L/usr/local/lib/ -ljmaths samples/fig_terminal_comparison_example.cpp -o samples/fig_terminal_comparison_example
********************/

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
