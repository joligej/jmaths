#include <iostream>
#include <cstdint>
#include <cassert>
#include <ctime>
#include <vector>
#include <numeric>
#include <string>
#include <limits>
#include <optional>
#include <format>
#include <print>

#if __has_include("../build/dbgjmaths.hpp")
    #include "../build/dbgjmaths.hpp"
#endif

using check_type = std::uint16_t;
using nlim = std::numeric_limits<check_type>;

typedef struct jmaths::detail::test {
    using testing_type = jmaths::N;

    static void check_add();
} test;

void test::check_add() {
    std::vector<decltype(std::clock())> times;
    times.reserve(nlim::max() + 1);

    for (check_type a = 0; a <= nlim::max(); ++a) {
        std::clog << "a: " << a << '\n';

        const auto start = std::clock();

        const testing_type a_big = testing_type(a);

        for (check_type b = 0; b <= nlim::max(); ++b) {
            //std::clog << "b: " << b << '\n';

            if (a > nlim::max() - b) {
                std::clog << "b: " << b << '\n';
                std::clog << "Skipping the rest due to overflow\n";
                goto next_a;
            }

            {

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

        const auto end = std::clock();

        const auto time_in_sec = ((double)end - start) / CLOCKS_PER_SEC;

        times.emplace_back(time_in_sec);

        const auto to_do = (double)(nlim::max() - a - 1) * (nlim::max() - a - 1) * 0.5;

        const auto total = (double)(nlim::max() - 1) * (nlim::max() - 1) * 0.5;

        const auto done = total - to_do;

        const auto expected_time = std::accumulate(times.cbegin(), times.cend(), 0) * to_do / done;

        std::cout << "Expected time: " << expected_time << '\n';


    }
}

int main (int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    using jmaths::N;

    constexpr std::size_t break_value = 9999;
    constexpr unsigned amount_of_rands = 1;

    for (std::size_t n = 0; n != break_value; std::cin >> n) {
        for (unsigned i = 0; i < amount_of_rands; ++i) {
            std::cout << N::rand(n) << '\n';
        }
    }

    std::cin.get();


    assert(true);
    assert(false);

    return 0;
}