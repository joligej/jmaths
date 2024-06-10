#include <iostream>
#include <cstdint>
#include <cassert>
#include <string>
#include <limits>
#include <optional>

#include "../build/debug_jmaths.hpp"

typedef std::uint16_t check_type;
typedef std::numeric_limits<check_type> nlim;

typedef struct jmaths::detail::unit_testing {
    typedef jmaths::N testing_type;

    static void check_add();
} test;

void test::check_add() {
    for (check_type a = 0; a <= nlim::max(); ++a) {
        std::cerr << "a: " << a << '\n';
        for (check_type b = 0; b <= nlim::max(); ++b) {
            //std::cerr << "b: " << b << '\n';

            if (a > nlim::max() - b) {
                std::cerr << "b: " << b << '\n';
                std::cerr << "Skipping the rest due to overflow\n";
                goto next_a;
            }

            {

            const testing_type a_big = testing_type(a);
            const testing_type b_big = testing_type(b);

            
            const auto result = a_big + b_big;
            const auto expected = a + b;
            const auto fits = result.fits_into<check_type>();

            assert(fits.has_value());

            assert(result == expected);
            assert(result.to_str(10) == std::to_string(expected));
            assert(*fits == expected);
            }

            if (b == nlim::max()) {
                goto next_a;
            }
        }

        next_a:;
    }
}

int main (int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    std::cout << nlim::max() << '\n';

    std::cin.get();

    test::check_add();


    assert(true);
    assert(false);

    return 0;
}