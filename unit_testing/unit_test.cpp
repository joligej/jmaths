#include <iostream>
#include <cassert>

#include "../build/jmaths.hpp"

typedef struct jmaths::detail::unit_testing {
    static int check_add (int a, int b);
} test;

int test::check_add (int a, int b) {
    return a + b;
}

int main (int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    assert(true);
    assert(test::check_add(1, 5) == 6);
    assert(false);

    return 0;
}