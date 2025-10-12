#define BOOST_TEST_MODULE jmaths_unit_test
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <functional>

#include "all.hpp"

#if 0

struct test_data {
    static constexpr jmaths::base_int_big small_values[] = {0U,
                                                            1U,
                                                            2U,
                                                            3U,
                                                            4U,
                                                            5U,
                                                            6U,
                                                            7U,
                                                            8U,
                                                            9U,
                                                            10U,
                                                            11U,
                                                            12U,
                                                            14U,
                                                            16U,
                                                            20U,
                                                            24U,
                                                            28U,
                                                            32U,
                                                            40U,
                                                            48U,
                                                            56U,
                                                            64U,
                                                            80U,
                                                            96U,
                                                            112U,
                                                            128U,
                                                            160U,
                                                            192U,
                                                            224U,
                                                            255U
    #if defined(UINT32_MAX)
                                                            ,
                                                            256U,
                                                            320U,
                                                            384U,
                                                            448U,
                                                            512U,
                                                            640U,
                                                            768U,
                                                            896U,
                                                            1024U,
                                                            1280U,
                                                            1536U,
                                                            1792U,
                                                            2048U,
                                                            2560U,
                                                            3072U,
                                                            3584U,
                                                            4096U,
                                                            5120U,
                                                            6144U,
                                                            7168U,
                                                            8192U,
                                                            10240U,
                                                            12288U,
                                                            14336U,
                                                            16384U,
                                                            20480U,
                                                            24576U,
                                                            28672U,
                                                            32768U,
                                                            40960U,
                                                            49152U,
                                                            57344U,
                                                            65535U
    #endif
    #if defined(UINT64_MAX)
                                                            ,
                                                            65536U,
                                                            81920U,
                                                            98304U,
                                                            114688U,
                                                            131072U,
                                                            163840U,
                                                            196608U,
                                                            229376U,
                                                            262144U,
                                                            327680U,
                                                            393216U,
                                                            458752U,
                                                            524288U,
                                                            655360U,
                                                            786432U,
                                                            917504U,
                                                            1048576U,
                                                            1310720U,
                                                            1572864U,
                                                            1835008U,
                                                            2097152U,
                                                            2621440U,
                                                            3145728U,
                                                            3670016U,
                                                            4194304U,
                                                            5242880U,
                                                            6291456U,
                                                            7340032U,
                                                            8388608U,
                                                            10485760U,
                                                            12582912U,
                                                            14680064U,
                                                            16777216U,
                                                            20971520U,
                                                            25165824U,
                                                            29360128U,
                                                            33554432U,
                                                            41943040U,
                                                            50331648U,
                                                            58720256U,
                                                            67108864U,
                                                            83886080U,
                                                            100663296U,
                                                            117440512U,
                                                            134217728U,
                                                            167772160U,
                                                            201326592U,
                                                            234881024U,
                                                            268435456U,
                                                            335544320U,
                                                            402653184U,
                                                            469762048U,
                                                            536870912U,
                                                            671088640U,
                                                            805306368U,
                                                            939524096U,
                                                            1073741824U,
                                                            1342177280U,
                                                            1610612736U,
                                                            1879048192U,
                                                            2147483648U,
                                                            2684354560U,
                                                            3221225472U,
                                                            3758096384U,
                                                            4294967295U
    #endif
    };

    static constexpr jmaths::base_int_big big_values[] = {
    // start 16
    #if !defined(UINT64_MAX) && !defined(UINT32_MAX) && defined(UINT16_MAX)
        256U,
        300U,
        350U,
        400U,
        512U,
        600U,
        700U,
        800U,
        1024U,
        1200U,
        1400U,
        1600U,
        2048U,
        2400U,
        2800U,
        3200U,
        4096U,
        4800U,
        5600U,
        6400U,
        8192U,
        9600U,
        11200U,
        12800U,
        16384U,
        19200U,
        22400U,
        25600U,
        32768U,
        38400U,
        44800U,
        51200U,
        65535U
    #endif
    #if !defined(UINT64_MAX) && defined(UINT32_MAX)
        65536U,
        76800U,
        89600U,
        102400U,
        131072U,
        153600U,
        179200U,
        204800U,
        262144U,
        307200U,
        358400U,
        409600U,
        524288U,
        614400U,
        716800U,
        819200U,
        1048576U,
        1228800U,
        1433600U,
        1638400U,
        2097152U,
        2457600U,
        2867200U,
        3276800U,
        4194304U,
        4915200U,
        5734400U,
        6553600U,
        8388608U,
        9830400U,
        11468800U,
        13107200U,
        16777216U,
        19660800U,
        22937600U,
        26214400U,
        33554432U,
        39321600U,
        45875200U,
        52428800U,
        67108864U,
        78643200U,
        91750400U,
        104857600U,
        134217728U,
        157286400U,
        183500800U,
        209715200U,
        268435456U,
        314572800U,
        367001600U,
        419430400U,
        536870912U,
        629145600U,
        734003200U,
        838860800U,
        1073741824U,
        1258291200U,
        1468006400U,
        1677721600U,
        2147483648U,
        2516582400U,
        2936012800U,
        3355443200U,
        4294967295U
    #endif
    #if defined(UINT64_MAX)
        4294967296U,
        5000000000U,
        6000000000U,
        7000000000U,
        8589934592U,
        10000000000U,
        12000000000U,
        15000000000U,
        17179869184U,
        20000000000U,
        25000000000U,
        30000000000U,
        34359738368U,
        40000000000U,
        50000000000U,
        60000000000U,
        68719476736U,
        80000000000U,
        100000000000U,
        120000000000U,
        137438953472U,
        160000000000U,
        200000000000U,
        250000000000U,
        274877906944U,
        300000000000U,
        350000000000U,
        400000000000U,
        549755813888U,
        600000000000U,
        700000000000U,
        800000000000U,
        1099511627776U,
        1200000000000U,
        1400000000000U,
        1600000000000U,
        2199023255552U,
        2400000000000U,
        2800000000000U,
        3200000000000U,
        4398046511104U,
        4800000000000U,
        5600000000000U,
        6400000000000U,
        8796093022208U,
        9600000000000U,
        11200000000000U,
        12800000000000U,
        17592186044416U,
        19200000000000U,
        22400000000000U,
        25600000000000U,
        35184372088832U,
        38400000000000U,
        44800000000000U,
        51200000000000U,
        70368744177664U,
        76800000000000U,
        89600000000000U,
        102400000000000U,
        140737488355328U,
        153600000000000U,
        179200000000000U,
        204800000000000U,
        281474976710656U,
        307200000000000U,
        358400000000000U,
        409600000000000U,
        562949953421312U,
        614400000000000U,
        716800000000000U,
        819200000000000U,
        1125899906842624U,
        1228800000000000U,
        1433600000000000U,
        1638400000000000U,
        2251799813685248U,
        2457600000000000U,
        2867200000000000U,
        3276800000000000U,
        4503599627370496U,
        4915200000000000U,
        5734400000000000U,
        6553600000000000U,
        9007199254740992U,
        9830400000000000U,
        11468800000000000U,
        13107200000000000U,
        18014398509481984U,
        19660800000000000U,
        22937600000000000U,
        26214400000000000U,
        36028797018963968U,
        39321600000000000U,
        45875200000000000U,
        52428800000000000U,
        72057594037927936U,
        78643200000000000U,
        91750400000000000U,
        104857600000000000U,
        144115188075855872U,
        157286400000000000U,
        183500800000000000U,
        209715200000000000U,
        288230376151711744U,
        314572800000000000U,
        367001600000000000U,
        419430400000000000U,
        576460752303423488U,
        629145600000000000U,
        734003200000000000U,
        838860800000000000U,
        1152921504606846976U,
        1258291200000000000U,
        1468006400000000000U,
        1677721600000000000U,
        2305843009213693952U,
        2516582400000000000U,
        2936012800000000000U,
        3355443200000000000U,
        4611686018427387904U,
        5033164800000000000U,
        5872025600000000000U,
        6710886400000000000U,
        9223372036854775808U,
        10066329600000000000U,
        11744051200000000000U,
        13421772800000000000U,
        18446744073709551615U
    #endif
    };
};

namespace {

using jmaths::base_int_big;

template <typename TEST_TYPE, typename OPERATOR_TYPE, typename EXTRA_TYPE>
void test_binary_operator(base_int_big lhs,
                          base_int_big rhs,
                          OPERATOR_TYPE binary_operator,
                          EXTRA_TYPE extra_test) {
    const auto result_test = binary_operator(TEST_TYPE(lhs), TEST_TYPE(rhs));
    const auto result_check = binary_operator(lhs, rhs);
    BOOST_CHECK(result_test == result_check ||
                extra_test(result_test) == result_check);
}

void for_all_number_pairs(auto test_action) {
    for (auto * number_list_lhs :
         {&test_data::small_values, &test_data::big_values}) {
        for (auto number_lhs : *number_list_lhs) {
            for (auto * number_list_rhs :
                 {&test_data::small_values, &test_data::big_values}) {
                for (auto number_rhs : *number_list_rhs) {
                    test_action(number_lhs, number_rhs);
                }
            }
        }
    }
}

template <typename TEST_TYPE>
void binary_operator_complete_test(auto binary_operator, auto extra_test) {
    using namespace std::placeholders;
    for_all_number_pairs(
        std::bind(test_binary_operator<TEST_TYPE,
                                       decltype(binary_operator),
                                       decltype(extra_test)>,
                  _1,
                  _2,
                  binary_operator,
                  extra_test));
}

}  // namespace

BOOST_AUTO_TEST_CASE(jmaths_N_operator_add) {
    using test_type = jmaths::N;
    auto binary_operator = std::plus{};

    using jmaths::base_int_big;
    auto extra_test = [](const test_type & val) {
        return val & test_type(~(base_int_big)0);
    };

    binary_operator_complete_test<test_type>(binary_operator, extra_test);
}

BOOST_AUTO_TEST_CASE(jmaths_Z_operator_add) {
    using test_type = jmaths::Z;
    auto binary_operator = std::plus{};

    using jmaths::base_int_big;
    auto extra_test = [](const test_type & val) {
        return val & test_type(~(base_int_big)0);
    };

    binary_operator_complete_test<test_type>(binary_operator, extra_test);
}

BOOST_AUTO_TEST_CASE(jmaths_N_operator_subtr) {
    using test_type = jmaths::N;
    auto binary_operator = std::minus{};

    using jmaths::base_int_big;
    auto extra_test = [](const test_type & val) {
        return -*val.fits_into<base_int_big>();
    };

    binary_operator_complete_test<test_type>(binary_operator, extra_test);
}

BOOST_AUTO_TEST_CASE(jmaths_Z_operator_subtr) {
    using test_type = jmaths::Z;
    auto binary_operator = std::minus{};

    using jmaths::base_int_big;
    auto extra_test = [](const test_type & val) {
        return (val + ~(base_int_big)0) + 1;
    };

    binary_operator_complete_test<test_type>(binary_operator, extra_test);
}

BOOST_AUTO_TEST_CASE(PassTest) {
    BOOST_CHECK_EQUAL(9U, 9);
}

#endif
