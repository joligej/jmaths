#include <iostream>
#include "jmaths.hpp"

/********************
compiled with:
-std=c++2b -I/usr/local/include/ -L/usr/local/lib/ -ljmaths samples/fig_terminal_q_fits_into_example.cpp -o samples/fig_terminal_q_fits_into_example
********************/

int main() {
    using jmaths::Q;
    using flt_type = double;
    constexpr flt_type not_present = -1;

    const auto print = [not_present](const Q & q) {
        std::cout << q << '\n';
        const auto into_float = q.template fits_into<flt_type>().value_or(not_present);
        std::cout << into_float << '\n';
    };

    #define SHOW(x) \
    std::cout << "Results for " << #x << ":\n"; \
    print(Q(x));

    SHOW("3/1");
    SHOW("-3/1");
    SHOW(3.765787);

    SHOW("-1/12");
    SHOW(0.18);
    SHOW(-234.24);

    SHOW("12/17");
}
