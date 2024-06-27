#include <functional>
#include <array>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include <cstdint>
#include <utility>
#include <iostream>


#if __has_include("../build/dbgjmaths.hpp")
    #include "../build/dbgjmaths.hpp"
#endif



#if 0

using check_type = std::uint16_t;
using nlim = std::numeric_limits<check_type>;

typedef struct jmaths::detail::test {
    using testing_type = jmaths::N;

    static void check_add();
    static void check_multiply();
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

void test::check_multiply() {
    for (unsigned long long i = 2; i < jmaths::base; i *= 3) {
        const N a (std::to_string(i));

        for (unsigned long long j = 2; j < jmaths::base; j *= 3) {
            const N b (std::to_string(j));

            const auto result = a * b;

            std::cout << "a: " << a << '\n';
            std::cout << "b: " << b << '\n';
            std::cout << "result: " << result << '\n';

            assert((result / a).first == b);
        }
    }
}

int main (int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    using jmaths::N;

    test::check_multiply();

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

#endif

#if 0

using jmaths::N;

enum : std::size_t {
    add = 0,
    subtract = 1,
    multiply = 2,
    bit_and = 3,
    bit_or = 4,
    bit_xor = 5
};

enum : std::size_t {
    equal_to = 0,
    not_equal_to = 1,
    less = 2,
    less_equal = 3,
    greater = 4,
    greater_equal = 5
};

const std::function<N(const N&, const N&)> operators[] = {
    std::plus{},
    std::minus{},
    std::multiplies{},
    std::bit_and{},
    std::bit_or{},
    std::bit_xor{}
};

const std::function<N&(N&, const N&)> operators_assign[] = {
    [](N & n1, const N & n2) -> N& { return n1 += n2; },
    [](N & n1, const N & n2) -> N& { return n1 -= n2; },
    [](N & n1, const N & n2) -> N& { return n1 *= n2; },
    [](N & n1, const N & n2) -> N& { return n1 &= n2; },
    [](N & n1, const N & n2) -> N& { return n1 |= n2; },
    [](N & n1, const N & n2) -> N& { return n1 ^= n2; }
};

const std::function<bool(const N&, const N&)> operators_compare[] = {
    std::equal_to{},
    std::not_equal_to{},
    std::less{},
    std::less_equal{},
    std::greater{},
    std::greater_equal{}
};

const std::function<N&(N&)> operators_incr_decr[] = {
    [](N & n) -> N& { return ++n; },
    [](N & n) -> N& { return --n; }
};

const std::function<N(const N&, jmaths::bit_type)> operators_bit[] = {
    [](const N & n, jmaths::bit_type bit) -> N { return n << bit; },
    [](const N & n, jmaths::bit_type bit) -> N { return n >> bit; }
};

const std::function<N&(N&, jmaths::bit_type)> operators_bit_assign[] = {
    [](N & n, jmaths::bit_type bit) -> N& { return n <<= bit; },
    [](N & n, jmaths::bit_type bit) -> N& { return n >>= bit; }
};

const std::function<N(const N&)> operators_misc[] = {
    std::bit_not{},
};

constexpr jmaths::base_int special_values[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    16,
    20,
    32,
    40,
    64,
    70,
    128,
    130,
    256,
    260,
    512,
    520,
    1024,
    1030,
    2048,
    2050,
    4096,
    4100,
    8192,
    8200,
    16384,
    16400,
    32768,
    32800,
    65500,
    65535
};

const std::array<N, std::size(special_values)> special_values_converted = []{
    std::array<N, std::size(special_values)> copy;
    std::copy(special_values, special_values + std::size(special_values), copy.begin());
    return copy;
}();

constexpr std::array<std::pair<jmaths::base_int, jmaths::base_int>, std::size(special_values) * std::size(special_values)> special_value_pairs = []{
    constexpr auto special_values_size = std::size(special_values);
    std::array<std::pair<jmaths::base_int, jmaths::base_int>, special_values_size * special_values_size> pairs;
    for (std::size_t i = 0; i < special_values_size; ++i) {
        for (std::size_t j = 0; j < special_values_size; ++j) {
            pairs[i * special_values_size + j] = {special_values[i], special_values[j]};
        }
    }
    return pairs;
}();

const std::array<std::pair<N, N>, std::size(special_values) * std::size(special_values)> special_value_pairs_converted = []{
    constexpr auto special_values_size = std::size(special_values);
    std::array<std::pair<N, N>, special_values_size * special_values_size> pairs;
    for (std::size_t i = 0; i < special_values_size; ++i) {
        for (std::size_t j = 0; j < special_values_size; ++j) {
            pairs[i * special_values_size + j] = {special_values_converted[i], special_values_converted[j]};
        }
    }
    return pairs;
}();

typedef struct jmaths::detail::test {
    using testing_type = jmaths::N;

    static void check2arg (const std::function<N(const N&, const N&)> & f);
} test;

void test::check2arg (const std::function<N(const N&, const N&)> & f) {
    for (std::size_t i = 0; i < std::size(special_value_pairs); ++i) {
        using simple_type = decltype(special_value_pairs)::value_type::first_type;
        const simple_type expected_result = ((std::function<simple_type(const simple_type&, const simple_type&)>&)f)(special_value_pairs[i].first, special_value_pairs[i].second);
        const N actual_result = f(special_value_pairs_converted[i].first, special_value_pairs_converted[i].second);

        assert(actual_result == expected_result);


    }
}

int main() {
    (void)special_values_converted;
    (void)special_value_pairs;

    test::check2arg(operators[add]);

    std::cout << "Hello World!\n";
}

#endif

static_assert(std::is_same_v<jmaths::base_int, std::uint16_t>, "The unit test is specialised only for 32-bit architectures");

using namespace jmaths;

struct testables {
    static constexpr base_int values1[] = {
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        16,
        17,
        20,
        21,
        32,
        40,
        41,
        64,
        70,
        71,
        128,
        130,
        137,
        256,
        260,
        271,
        328,
        421,
        512,
        520,
        680,
        681,
        721,
        730,
        890,
        899,
        1024,
        1030,
        1220,
        1281,
        2048,
        2050,
        2991,
        3660,
        4096,
        4100,
        6214,
        7234,
        8192,
        8200,
        11'234,
        13'651,
        16'384,
        16'400,
        21'147,
        32'768,
        32'800,
        39'123,
        45'126,
        65'500,
        65'535
    };  
    
    static constexpr auto values1_size = std::size(values1);

    static constexpr base_int_big values2[] = {
        65'536,
        68'000,
        69'000,
        70'000,
        131'072,
        145'000,
        150'000,
        160'000,
        262'144,
        275'000,
        285'000,
        295'000,
        524'288,
        540'000,
        550'000,
        560'000,
        1'048'576,
        1'060'000,
        1'070'000,
        1'080'000,
        2'097'152,
        2'100'000,
        2'110'000,
        2'120'000,
        4'194'304,
        4'200'000,
        4'210'000,
        4'220'000,
        8'388'608,
        8'390'000,
        8'400'000,
        8'410'000,
        16'777'216,
        16'780'000,
        16'790'000,
        16'800'000,
        33'554'432,
        33'560'000,
        33'570'000,
        33'580'000,
        67'108'864,
        67'110'000,
        67'120'000,
        67'130'000,
        134'217'728,
        134'220'000,
        134'230'000,
        134'240'000,
        268'435'456,
        268'440'000,
        268'450'000,
        268'460'000,
        536'870'912,
        536'880'000,
        536'890'000,
        536'900'000,
        1'073'741'824,
        1'073'750'000,
        1'073'760'000,
        1'073'770'000,
        2'147'483'648,
        2'147'490'000,
        2'147'500'000,
        2'147'510'000,
        4'294'967'295
    };

    static constexpr auto values2_size = std::size(values2);
    
    static std::array<N, values1_size> values1_conv;
    static std::array<N, values2_size> values2_conv;

    static constexpr std::array<std::pair<base_int, base_int>, values1_size * values1_size> values1_pairs = []{
        std::remove_cv_t<decltype(values1_pairs)> pairs;
        for (std::size_t i = 0; i < values1_size; ++i) {
            for (std::size_t j = 0; j < values1_size; ++j) {
                pairs[i * values1_size + j] = {values1[i], values1[j]};
            }
        }
        return pairs;
    }();

    static constexpr std::array<std::pair<base_int_big, base_int_big>, values2_size * values2_size> values2_pairs = []{
        std::remove_cv_t<decltype(values2_pairs)> pairs;
        for (std::size_t i = 0; i < values2_size; ++i) {
            for (std::size_t j = 0; j < values2_size; ++j) {
                pairs[i * values2_size + j] = {values2[i], values2[j]};
            }
        }
        return pairs;
    }();

    static std::array<std::pair<N, N>, values1_size * values1_size> values1_conv_pairs;

    static std::array<std::pair<N, N>, values2_size * values2_size> values2_conv_pairs;

    static constexpr auto combined_size = values1_size * values2_size;

    static constexpr std::array<std::pair<base_int, base_int_big>, combined_size> combined_pairs = []{
        std::remove_cv_t<decltype(combined_pairs)> pairs;
        for (std::size_t i = 0; i < values1_size; ++i) {
            for (std::size_t j = 0; j < values2_size; ++j) {
                pairs[i * values2_size + j] = {values1[i], values2[j]};
            }
        }
        return pairs;
    }();

    static std::array<std::pair<N, N>, combined_size> combined_conv_pairs;

};

decltype(testables::values1_conv) testables::values1_conv = []{
    decltype(values1_conv) converted;
    std::copy(values1, values1 + values1_size, converted.begin());
    return converted;
}();

decltype(testables::values2_conv) testables::values2_conv = []{
    decltype(values2_conv) converted;
    std::copy(values2, values2 + values2_size, converted.begin());
    return converted;
}();

decltype(testables::values1_conv_pairs) testables::values1_conv_pairs = []{
    decltype(values1_conv_pairs) pairs;
    for (std::size_t i = 0; i < values1_size; ++i) {
        for (std::size_t j = 0; j < values1_size; ++j) {
            pairs[i * values1_size + j] = {values1_conv[i], values1_conv[j]};
        }
    }
    return pairs;
}();

decltype(testables::values2_conv_pairs) testables::values2_conv_pairs = []{
    decltype(values2_conv_pairs) pairs;
    for (std::size_t i = 0; i < values2_size; ++i) {
        for (std::size_t j = 0; j < values2_size; ++j) {
            pairs[i * values2_size + j] = {values2_conv[i], values2_conv[j]};
        }
    }
    return pairs;
}();

decltype(testables::combined_conv_pairs) testables::combined_conv_pairs = []{
    decltype(combined_conv_pairs) converted;
    std::copy(combined_pairs.begin(), combined_pairs.end(), converted.begin());
    return converted;
}();

struct arg2res {
    template <typename T>
    using func = std::function<T(const T&, const T&)>;

    enum value : std::size_t {
        add = 0,
        subtract,
        multiply,
        bit_and,
        bit_or,
        bit_xor,
        max_value
    };
};

struct arg2comp {
    template <typename T>
    using func = std::function<bool(const T&, const T&)>;

    enum value : std::size_t {
        equal_to = 0,
        not_equal_to,
        less,
        less_equal,
        greater,
        greater_equal,
        max_value
    };
};

struct arg2assign : arg2res {
    template <typename T>
    using func = std::function<T&(T&, const T&)>;
};

struct arg1single {
    template <typename T>
    using func = std::function<T&(T&)>;

    enum value : std::size_t {
        increment = 0,
        decrement,
        max_value
    };
};

struct arg2bit {
    template <typename T>
    using func = std::function<T(const T&, bit_type)>;

    enum value : std::size_t {
        left = 0,
        right,
        max_value
    };
};

struct arg2bitassign : arg2bit {
    template <typename T>
    using func = std::function<T&(T&, bit_type)>;
};

struct arg1bitwise {
    template <typename T>
    using func = std::function<T(const T&)>;
    
    enum value : std::size_t {
        complement = 0,
        max_value
    };
};

struct arg2div {
    template <typename T>
    using func = std::function<T(const T&, const T&)>;

    template <typename T>
    using func_special = std::function<std::pair<T, T>(const T&, const T&)>;

    enum value : std::size_t {
        division = 0,
        max_value
    };
};

template <typename T>
struct check_operators {
    static const T res[];
    static const T comp[];
    static const T assign[];
    static const T single[];
    static const T bit[];
    static const T bitassign[];
    static const T bitwise[];
    static const T division[];
};

template <typename T>
const T check_operators<T>::res[] = {
    std::plus{},
    std::minus{},
    std::multiplies{},
    std::bit_and{},
    std::bit_or{},
    std::bit_xor{}
};

template <typename T>
const T check_operators<T>::comp[] = {
    std::equal_to{},
    std::not_equal_to{},
    std::less{},
    std::less_equal{},
    std::greater{},
    std::greater_equal{}
};

template <typename T>
const T check_operators<T>::assign[] = {
    [](auto & n1, const auto & n2) -> auto& { return n1 += n2; },
    [](auto & n1, const auto & n2) -> auto& { return n1 -= n2; },
    [](auto & n1, const auto & n2) -> auto& { return n1 *= n2; },
    [](auto & n1, const auto & n2) -> auto& { return n1 &= n2; },
    [](auto & n1, const auto & n2) -> auto& { return n1 |= n2; },
    [](auto & n1, const auto & n2) -> auto& { return n1 ^= n2; }
};

template <typename T>
const T check_operators<T>::single[] = {
    [](auto & n) -> auto& { return ++n; },
    [](auto & n) -> auto& { return --n; }
};

template <typename T>
const T check_operators<T>::bit[] = {
    [](const auto & n, bit_type bit) -> auto { return n << bit; },
    [](const auto & n, bit_type bit) -> auto { return n >> bit; }
};

template <typename T>
const T check_operators<T>::bitassign[] = {
    [](auto & n, bit_type bit) -> auto& { return n <<= bit; },
    [](auto & n, bit_type bit) -> auto& { return n >>= bit; }
};

template <typename T>
const T check_operators<T>::bitwise[] = {
    std::bit_not{}
};

template <typename T>
const T check_operators<T>::division[] = {
    std::divides{}
};

typedef struct detail::test {
    static void run_all();

    private:
        static void check2res (arg2res::value check_func);
        static void check2comp (arg2comp::value check_func);
        static void check2assign (arg2assign::value check_func);
        static void check1single (arg1single::value check_func);
        static void check2bit (arg2bit::value check_func);
        static void check2bitassign (arg2bitassign::value check_func);
        static void check1bitwise (arg1bitwise::value check_func);
        static void check2div (arg2div::value check_func);

        static void checkbitmanip();

        static void check_all_funcs (auto && func_container, auto check_func);
        static void check_single_func (auto check_func);
        static void for_all_lists_pairs (auto func);
        static void for_all_lists_single (auto func);
} tests;

void tests::check2res (arg2res::value check_func) {
    const auto check_list = [check_func] (auto && value_list, auto && value_list_conv) {
        for (std::size_t i = 0; i < std::size(value_list); ++i) {
            base_int_big result_primitive;
            if (check_func == arg2res::subtract && value_list[i].first < value_list[i].second) {
                result_primitive = check_operators<arg2res::func<base_int_big>>::res[check_func](value_list[i].second, value_list[i].first);
            } else {
                result_primitive = check_operators<arg2res::func<base_int_big>>::res[check_func](value_list[i].first, value_list[i].second);
            }

            auto result_special = check_operators<arg2res::func<N>>::res[check_func](value_list_conv[i].first, value_list_conv[i].second);

            if (result_primitive != result_special) {
                assert(result_special.front_() == (base_int)result_primitive); // assert this is due to overflow
            }
        }
    };

    for_all_lists_pairs(check_list);
}

void tests::check2comp (arg2comp::value check_func) {
    const auto check_list = [check_func] (auto && value_list, auto && value_list_conv) {
        for (std::size_t i = 0; i < std::size(value_list); ++i) {
            bool result_primitive = check_operators<arg2comp::func<base_int_big>>::comp[check_func](value_list[i].first, value_list[i].second);
            bool result_special = check_operators<arg2comp::func<N>>::comp[check_func](value_list_conv[i].first, value_list_conv[i].second);

            assert (result_primitive == result_special);
        }
    };

    for_all_lists_pairs(check_list);
}

void tests::check2assign (arg2assign::value check_func) {
    const auto check_list = [check_func] (auto && value_list, auto && value_list_conv) {
        for (std::size_t i = 0; i < std::size(value_list); ++i) {
            base_int_big result_primitive;
            if (check_func == arg2assign::subtract && value_list[i].first < value_list[i].second) {
                result_primitive = value_list[i].second;
                check_operators<arg2assign::func<base_int_big>>::assign[check_func](result_primitive, value_list[i].first);
            } else {
                result_primitive = value_list[i].first;
                check_operators<arg2assign::func<base_int_big>>::assign[check_func](result_primitive, value_list[i].second);
            }


            N result_special = value_list_conv[i].first;
            check_operators<arg2assign::func<N>>::assign[check_func](result_special, value_list_conv[i].second);

            if (result_primitive != result_special) {
                assert(result_special.front_() == (base_int)result_primitive); // assert this is due to overflow
            }
        }
    };

    for_all_lists_pairs(check_list);
}

void tests::check1single (arg1single::value check_func) {
    const auto check_list = [check_func] (auto && value_list, auto && value_list_conv) {
        for (std::size_t i = 0; i < std::size(value_list); ++i) {
            base_int_big result_primitive;
            if (check_func == arg1single::decrement && value_list[i] == 0) {
                result_primitive = 0;
            } else {
                result_primitive = value_list[i];
                check_operators<arg1single::func<base_int_big>>::single[check_func](result_primitive);
            }


            N result_special = value_list_conv[i];
            check_operators<arg1single::func<N>>::single[check_func](result_special);

            if (result_primitive != result_special) {
                assert(result_special.front_() == (base_int)result_primitive); // assert this is due to overflow
            }
        }
    };

    for_all_lists_single(check_list);
}

void tests::check2bit (arg2bit::value check_func) {
    static constexpr bit_type max_bitshift = 32;

    const auto check_list = [check_func] (auto && value_list, auto && value_list_conv) {
        for (std::size_t i = 0; i < std::size(value_list); ++i) {
            for (bit_type shift = 0; shift < max_bitshift; ++shift) {
                base_int_big result_primitive = check_operators<arg2bit::func<base_int_big>>::bit[check_func](value_list[i], shift);

                auto result_special = check_operators<arg2bit::func<N>>::bit[check_func](value_list_conv[i], shift);

                if (result_primitive != result_special) {
                    assert(result_special.front_() == (base_int)result_primitive); // assert this is due to overflow
                }
            }
        }
    };

    for_all_lists_single(check_list);
}

void tests::check2bitassign (arg2bitassign::value check_func) {
    static constexpr bit_type max_bitshift = 32;

    const auto check_list = [check_func] (auto && value_list, auto && value_list_conv) {
        for (std::size_t i = 0; i < std::size(value_list); ++i) {
            for (bit_type shift = 0; shift < max_bitshift; ++shift) {
                base_int_big result_primitive = value_list[i];
                check_operators<arg2bit::func<base_int_big>>::bit[check_func](result_primitive, shift);

                N result_special = value_list_conv[i];
                check_operators<arg2bit::func<N>>::bit[check_func](result_special, shift);

                if (result_primitive != result_special) {
                    assert(result_special.front_() == (base_int)result_primitive); // assert this is due to overflow
                }
            }
        }
    };

    for_all_lists_single(check_list);
}

void tests::check1bitwise (arg1bitwise::value check_func) {
    const auto check_list = [check_func] (auto && value_list, auto && value_list_conv) {
        for (std::size_t i = 0; i < std::size(value_list); ++i) {
            base_int_big result_primitive = value_list[i] == 0 ? 0 : check_operators<arg1bitwise::func<base_int_big>>::bitwise[check_func](value_list[i]);

            N result_special = check_operators<arg1bitwise::func<N>>::bitwise[check_func](value_list_conv[i]);

            if (result_primitive != result_special) {
                assert(result_special.front_() == (base_int)result_primitive); // assert this is due to overflow
            }
        }
    };

    for_all_lists_single(check_list);
}

void tests::check2div (arg2div::value check_func) {
    const auto check_list = [check_func] (auto && value_list, auto && value_list_conv) {
        for (std::size_t i = 0; i < std::size(value_list); ++i) {
            base_int_big result_primitive_div = value_list[i].second == 0 ? 0 : check_operators<arg2div::func<base_int_big>>::division[check_func](value_list[i].first, value_list[i].second);
            base_int_big result_primitive_mod = value_list[i].second == 0 ? 0 : value_list[i].first - result_primitive_div * value_list[i].second;

            try {
                auto result_special = check_operators<arg2div::func_special<N>>::division[check_func](value_list_conv[i].first, value_list_conv[i].second);

                assert(result_primitive_div == result_special.first);
                assert(result_primitive_mod == result_special.second);
            } catch (const error::division_by_zero & e) {
                assert(value_list[i].second == 0);
            }
        }
    };

    for_all_lists_pairs(check_list);
}

void tests::checkbitmanip() {
    static constexpr bit_type max_bit = 32;

    const auto check_list = [](auto && value_list, auto && value_list_conv) {
        for (std::size_t i = 0; i < std::size(value_list); ++i) {
            for (bit_type shift = 0; shift < max_bit; ++shift) {
                base_int_big result_primitive_on = value_list[i];
                base_int_big result_primitive_off = value_list[i];

                result_primitive_on |= (base_int_big)1 << shift;
                result_primitive_off &= ~((base_int_big)1 << shift);

                N result_special_on = value_list_conv[i];
                N result_special_off = value_list_conv[i];

                result_special_on[shift] = 1;
                result_special_off[shift] = 0;

                if (result_primitive_on != result_special_on) {
                    assert(result_special_on.front_() == (base_int)result_primitive_on); // assert this is due to overflow
                }

                if (result_primitive_off != result_special_off) {
                    assert(result_special_off.front_() == (base_int)result_primitive_off); // assert this is due to overflow
                }
            }
        }
    };

    for_all_lists_single(check_list);
}

void tests::check_all_funcs (auto && func_container, auto check_func) {
    using func_type = std::remove_reference_t<decltype(func_container)>;
    using value_type = typename func_type::value;

    for (std::underlying_type_t<value_type> i = 0; i < func_type::max_value; ++i) {
        check_func((value_type)i);
    }
}

void tests::check_single_func (auto check_func) {
    check_func();
}

void tests::run_all() {
    check_single_func(checkbitmanip);

    check_all_funcs(arg2comp{}, check2comp);
    check_all_funcs(arg2res{}, check2res);
    check_all_funcs(arg2assign{}, check2assign);
    check_all_funcs(arg1single{}, check1single);
    check_all_funcs(arg2bit{}, check2bit);
    check_all_funcs(arg2bitassign{}, check2bitassign);
    check_all_funcs(arg1bitwise{}, check1bitwise);
    check_all_funcs(arg2div{}, check2div);
}

void tests::for_all_lists_pairs (auto func) {
    func(testables::combined_pairs, testables::combined_conv_pairs);
    func(testables::values1_pairs, testables::values1_conv_pairs);
    func(testables::values2_pairs, testables::values2_conv_pairs);
}

void tests::for_all_lists_single (auto func) {
    func(testables::values1, testables::values1_conv);
    func(testables::values2, testables::values2_conv);
}

int main() {
    std::cout << "Starting unit test...\n";

    tests::run_all();

    std::cout << "Succesfully finished unit test\n";
}
