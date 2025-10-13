// The jmaths library for C++
// Copyright (C) 2025  Jasper de Smaele

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <boost/test/unit_test.hpp>
#include <chrono>
#include <iostream>
#include "all.hpp"

using namespace jmaths;

BOOST_AUTO_TEST_SUITE(performance_tests)

// Helper to measure execution time
template<typename Func>
double measure_time(Func&& func, int iterations = 1000) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count() / iterations;
}

BOOST_AUTO_TEST_CASE(addition_performance) {
    N a("123456789012345678901234567890");
    N b("987654321098765432109876543210");
    
    auto avg_time = measure_time([&]() {
        volatile N c = a + b;
    });
    
    BOOST_TEST_MESSAGE("Average addition time: " << avg_time << " ms");
    // Just verify it completes in reasonable time (not a hard limit test)
    BOOST_TEST(avg_time < 1.0); // Should be much faster than 1ms
}

BOOST_AUTO_TEST_CASE(multiplication_performance) {
    N a("12345678901234567890");
    N b("98765432109876543210");
    
    auto avg_time = measure_time([&]() {
        volatile N c = a * b;
    }, 100); // Fewer iterations for slower operation
    
    BOOST_TEST_MESSAGE("Average multiplication time: " << avg_time << " ms");
    BOOST_TEST(avg_time < 10.0);
}

BOOST_AUTO_TEST_CASE(gcd_performance) {
    N a(1000000);
    N b(750000);
    
    auto avg_time = measure_time([&]() {
        volatile N c = calc::gcd(a, b);
    });
    
    BOOST_TEST_MESSAGE("Average GCD time: " << avg_time << " ms");
    BOOST_TEST(avg_time < 1.0);
}

BOOST_AUTO_TEST_CASE(power_performance) {
    N base(2);
    N exp(100);
    
    auto avg_time = measure_time([&]() {
        volatile N c = calc::pow(base, exp);
    });
    
    BOOST_TEST_MESSAGE("Average power time: " << avg_time << " ms");
    BOOST_TEST(avg_time < 1.0);
}

BOOST_AUTO_TEST_CASE(string_conversion_performance) {
    N n("123456789012345678901234567890123456789012345678901234567890");
    
    auto avg_time = measure_time([&]() {
        volatile std::string s = n.to_str();
    });
    
    BOOST_TEST_MESSAGE("Average string conversion time: " << avg_time << " ms");
    BOOST_TEST(avg_time < 1.0);
}

BOOST_AUTO_TEST_CASE(construction_performance) {
    auto avg_time = measure_time([]() {
        volatile N n("123456789012345678901234567890");
    });
    
    BOOST_TEST_MESSAGE("Average construction time: " << avg_time << " ms");
    BOOST_TEST(avg_time < 1.0);
}

BOOST_AUTO_TEST_CASE(copy_performance) {
    N original("123456789012345678901234567890123456789012345678901234567890");
    
    auto avg_time = measure_time([&]() {
        volatile N copy(original);
    });
    
    BOOST_TEST_MESSAGE("Average copy time: " << avg_time << " ms");
    BOOST_TEST(avg_time < 0.5);
}

BOOST_AUTO_TEST_CASE(comparison_performance) {
    N a("123456789012345678901234567890");
    N b("123456789012345678901234567891");
    
    auto avg_time = measure_time([&]() {
        volatile bool result = (a < b);
        (void)result; // Ensure result is used
    });
    
    BOOST_TEST_MESSAGE("Average comparison time: " << avg_time << " ms");
    BOOST_TEST(avg_time < 0.5);
}

BOOST_AUTO_TEST_CASE(modulo_performance) {
    N a("123456789012345678901234567890");
    N b(1000000007); // Large prime
    
    auto avg_time = measure_time([&]() {
        auto result = a / b;
        // Use result to prevent optimization
        volatile auto q_val = std::get<0>(result).to_str().length();
        (void)q_val;
    }, 100);
    
    BOOST_TEST_MESSAGE("Average modulo time: " << avg_time << " ms");
    BOOST_TEST(avg_time < 5.0);
}

BOOST_AUTO_TEST_CASE(bitwise_performance) {
    N a(0xDEADBEEF);
    N b(0xCAFEBABE);
    
    auto avg_time = measure_time([&]() {
        volatile N c = a & b;
        volatile N d = a | b;
        volatile N e = a ^ b;
    });
    
    BOOST_TEST_MESSAGE("Average bitwise ops time: " << avg_time << " ms");
    BOOST_TEST(avg_time < 1.0);
}

BOOST_AUTO_TEST_SUITE_END()
