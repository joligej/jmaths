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

/**
 * @file benchmark_comprehensive.cpp
 * @brief Comprehensive benchmarking program for jmaths library
 *
 * This program exercises all major API functions to collect performance data.
 * Build with: cmake -Denable_benchmarking=ON
 * Run: ./benchmark_comprehensive [output_file]
 * Default output: benchmark_results.log
 */

#include <fstream>
#include <iostream>
#include <string>

#include "all.hpp"
#include "scoped_timer.hpp"

using namespace jmaths;
using namespace jmaths::literals;

// Redirect benchmarking output to file
void setup_benchmark_logging(const std::string& filename) {
    static std::ofstream log_file(filename);
    if (log_file.is_open()) {
        internal::scoped_timer::set_ostream(&log_file);
        std::cout << "Benchmarking output redirected to: " << filename << std::endl;
    } else {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

void benchmark_basic_n_operations() {
    std::cout << "\n=== Benchmarking N (unsigned integer) operations ===" << std::endl;

    // Construction
    N n1;
    N n2(42);
    N n3("123456789012345678901234567890");
    N n4 = 999_N;

    // Arithmetic
    N sum = n2 + n4;
    N diff = n4 - n2;
    N prod = n2 * N(100);
    auto [quot, rem] = n4 / n2;

    // Comparison
    bool eq = (n2 == n4);
    bool lt = (n2 < n4);
    bool gt = (n4 > n2);
    (void)eq; (void)lt; (void)gt;  // Suppress unused warnings

    // Compound operations
    N compound(10);
    compound += N(5);
    compound -= N(3);
    compound *= N(2);

    // Increment/Decrement
    ++compound;
    --compound;

    // Conversions
    std::string str = n2.to_str();
    auto opt_val = n2.fits_into<unsigned int>();
    (void)str; (void)opt_val;  // Suppress unused warnings

    // Large number operations
    N large1("999999999999999999999999999999");
    N large2("888888888888888888888888888888");
    N large_sum = large1 + large2;
    N large_prod = N(123456) * N(789012);

    std::cout << "N operations completed" << std::endl;
}

void benchmark_basic_z_operations() {
    std::cout << "\n=== Benchmarking Z (signed integer) operations ===" << std::endl;

    // Construction
    Z z1;
    Z z2(42);
    Z z3(-42);
    Z z4("12345678901234567890");
    Z z5 = -999_Z;

    // Arithmetic
    Z sum = z2 + z3;
    Z diff = z2 - z3;
    Z prod = z3 * Z(10);
    auto [quot, rem] = z2 / Z(7);

    // Negation
    Z neg = -z2;
    N abs_val = z3.abs();  // abs() returns N, not Z

    // Comparison
    bool eq = (z2 == z3);
    bool lt = (z3 < z2);
    (void)eq; (void)lt;  // Suppress unused warnings

    // Sign operations
    bool is_pos = z2.is_positive();
    bool is_neg = z3.is_negative();
    bool is_zero = Z(0).is_zero();
    (void)is_pos; (void)is_neg; (void)is_zero;  // Suppress unused warnings

    // Compound operations
    Z compound(-10);
    compound += Z(5);
    compound -= Z(-3);
    compound *= Z(2);

    // Mixed sign operations
    Z mixed1 = Z(100) + Z(-50);
    Z mixed2 = Z(-100) * Z(5);

    std::cout << "Z operations completed" << std::endl;
}

void benchmark_basic_q_operations() {
    std::cout << "\n=== Benchmarking Q (rational number) operations ===" << std::endl;

    // Construction
    Q q1("1/2");
    Q q2("2/3");
    Q q3(Z(3), Z(4));
    Q q4 = 5_Q;

    // Arithmetic
    Q sum = q1 + q2;       // 7/6
    Q diff = q2 - q1;      // 1/6
    Q prod = q1 * q2;      // 1/3
    Q quot = q1 / q2;      // 3/4

    // Comparison
    bool eq = (q1 == q2);
    bool lt = (q1 < q2);
    bool gt = (q2 > q1);
    (void)eq; (void)lt; (void)gt;  // Suppress unused warnings

    // Reduction
    Q unreduced("10/15");  // Should auto-reduce to 2/3
    Q negative("-3/4");

    // Complex fractions
    Q improper("7/3");
    Q result = (q1 + q2) * improper;

    // Conversion
    std::string str = q1.to_str();

    // Operations with integers
    Q from_z(Z(42), Z(7));
    Q mixed = q1 + Q("3/1");

    std::cout << "Q operations completed" << std::endl;
}

void benchmark_calc_functions() {
    std::cout << "\n=== Benchmarking calc namespace functions ===" << std::endl;

    // GCD tests
    N gcd1 = calc::gcd(N(48), N(18));
    N gcd2 = calc::gcd(N(100), N(75));
    N gcd3 = calc::gcd(N("123456789"), N("987654321"));

    // Square root
    auto [sqrt_root1, sqrt_rem1] = calc::sqrt(N(144));
    auto [sqrt_root2, sqrt_rem2] = calc::sqrt(N(150));
    N sqrt_whole = calc::sqrt_whole(N(1000));

    // Power (N)
    N pow1 = calc::pow(N(2), N(10));      // 2^10 = 1024
    N pow2 = calc::pow(N(3), N(20));      // 3^20
    N pow3 = calc::pow(N(10), N(50));     // 10^50

    // Power (Z)
    Z pow_z1 = calc::pow(Z(5), N(3));
    Z pow_z2 = calc::pow(Z(-2), N(10));

    // Modular exponentiation
    N pow_mod1 = calc::pow_mod(N(2), N(10), N(1000));
    N pow_mod2 = calc::pow_mod(N(3), N(100), N(97));

    // Edge cases
    N gcd_zero = calc::gcd(N(42), N(0));
    N pow_zero = calc::pow(N(42), N(0));  // Should be 1
    N pow_one = calc::pow(N(42), N(1));   // Should be 42

    std::cout << "calc functions completed" << std::endl;
}

void benchmark_bitwise_operations() {
    std::cout << "\n=== Benchmarking bitwise operations ===" << std::endl;

    N a(0b11110000);
    N b(0b10101010);

    // Basic bitwise
    N and_result = a & b;
    N or_result = a | b;
    N xor_result = a ^ b;
    N not_result = ~a;

    // Shifts
    N left_shift = a << 4;
    N right_shift = a >> 2;

    // Compound bitwise
    N compound(0b1111);
    compound &= N(0b1100);
    compound |= N(0b0011);
    compound ^= N(0b0101);
    compound <<= 5;
    compound >>= 2;

    // Bit access
    bool bit0 = a[0];
    bool bit4 = a[4];
    (void)bit0; (void)bit4;  // Suppress unused warnings

    // Large shifts
    N large_shift = N(1) << 50;
    N shift_back = large_shift >> 25;

    std::cout << "Bitwise operations completed" << std::endl;
}

void benchmark_random_generation() {
    std::cout << "\n=== Benchmarking random number generation ===" << std::endl;

    // Generate random N values
    N rand_n1 = jmaths::rand<N>::generate(10);
    N rand_n2 = jmaths::rand<N>::generate(100);
    N rand_n3 = jmaths::rand<N>::generate(1000);

    // Generate random Z values
    Z rand_z1 = jmaths::rand<Z>::generate(10);
    Z rand_z2 = jmaths::rand<Z>::generate(100);
    Z rand_z3 = jmaths::rand<Z>::generate(1000);

    // Multiple generations
    for (int i = 0; i < 10; ++i) {
        N temp = jmaths::rand<N>::generate(50);
    }

    std::cout << "Random generation completed" << std::endl;
}

void benchmark_conversions() {
    std::cout << "\n=== Benchmarking conversions ===" << std::endl;

    // String conversions
    N n(12345);
    std::string dec = n.to_str();
    std::string hex = n.to_str(16);
    std::string bin = n.to_str(2);

    // From string
    N from_dec("12345");
    N from_hex("3039", 16);
    N from_bin("11000000111001", 2);

    // fits_into
    N small(42);
    auto uint_val = small.fits_into<unsigned int>();
    auto ull_val = small.fits_into<unsigned long long>();

    N large("999999999999999999999");
    auto overflow = large.fits_into<unsigned int>();  // Should be empty
    (void)uint_val; (void)ull_val; (void)overflow;  // Suppress unused warnings

    // Z conversions
    Z z_pos(123);
    Z z_neg(-456);
    auto z_int = z_pos.fits_into<int>();
    (void)z_int;  // Suppress unused warnings

    // Q conversions
    Q q("3/4");
    std::string q_str = q.to_str();

    std::cout << "Conversions completed" << std::endl;
}

void benchmark_stream_operations() {
    std::cout << "\n=== Benchmarking stream operations ===" << std::endl;

    // Output streams
    std::ostringstream oss;
    oss << N(42) << " " << Z(-42) << " " << Q("1/2");

    // Input streams
    std::istringstream iss("123 -456 7/8");
    N n_in;
    Z z_in;
    iss >> n_in >> z_in;

    // Multiple values
    std::ostringstream oss2;
    for (int i = 0; i < 10; ++i) {
        oss2 << N(i * 100) << " ";
    }

    std::cout << "Stream operations completed" << std::endl;
}

void benchmark_literals() {
    std::cout << "\n=== Benchmarking user-defined literals ===" << std::endl;

    // N literals
    auto n1 = 0_N;
    auto n2 = 42_N;
    auto n3 = 123456789_N;

    // Z literals
    auto z1 = 0_Z;
    auto z2 = 42_Z;
    auto z3 = -42_Z;
    auto z4 = 123456789_Z;

    // Q literals
    auto q1 = 0_Q;
    auto q2 = 42_Q;
    auto q3 = -42_Q;

    // Arithmetic with literals
    auto sum = 10_N + 20_N;
    auto diff = 100_Z - 42_Z;
    auto prod = 3_N * 7_N;

    std::cout << "Literals completed" << std::endl;
}

void benchmark_edge_cases() {
    std::cout << "\n=== Benchmarking edge cases ===" << std::endl;

    // Zero operations
    N zero_n(0);
    Z zero_z(0);
    N zero_sum = zero_n + N(1);
    N zero_prod = zero_n * N(1000);

    // Division by 1
    auto [q1, r1] = N(100) / N(1);

    // Power edge cases
    N pow_0_0 = calc::pow(N(0), N(0));  // 1 by convention
    N pow_base_1 = calc::pow(N(1), N(1000));
    N pow_exp_0 = calc::pow(N(999), N(0));

    // Very large numbers
    N huge1("99999999999999999999999999999999999999999999999999");
    N huge2("88888888888888888888888888888888888888888888888888");
    N huge_sum = huge1 + huge2;

    // GCD edge cases
    N gcd_same = calc::gcd(N(42), N(42));
    N gcd_zero = calc::gcd(N(42), N(0));

    std::cout << "Edge cases completed" << std::endl;
}

void benchmark_complex_expressions() {
    std::cout << "\n=== Benchmarking complex expressions ===" << std::endl;

    // Complex arithmetic chains
    N result1 = ((N(10) + N(20)) * N(3)) - N(5);
    Z result2 = (Z(100) - Z(50)) * Z(-2) + Z(75);
    Q result3 = (Q("1/2") + Q("1/3")) * Q("6/1");

    // Nested operations
    N nested = calc::pow(N(2), N(5)) + calc::pow(N(3), N(4));
    N gcd_complex = calc::gcd(calc::pow(N(2), N(10)), calc::pow(N(2), N(8)));

    // Mixed type operations
    Q mixed1 = Q(Z(42), Z(7)) + Q("1/2");
    Q mixed2 = Q("3/4") * Q(Z(8), Z(3));

    // Chain operations
    N chain = N(1);
    for (int i = 0; i < 10; ++i) {
        chain = chain * N(2) + N(1);
    }

    std::cout << "Complex expressions completed" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string log_file = "benchmark_results.log";
    if (argc > 1) {
        log_file = argv[1];
    }

    std::cout << "========================================" << std::endl;
    std::cout << "  jmaths Comprehensive Benchmark Suite" << std::endl;
    std::cout << "========================================" << std::endl;

#ifdef JMATHS_BENCHMARKING
    #if JMATHS_BENCHMARKING == 1
        std::cout << "Benchmarking: ENABLED" << std::endl;
        setup_benchmark_logging(log_file);
    #else
        std::cout << "Warning: JMATHS_BENCHMARKING is not enabled!" << std::endl;
        std::cout << "Rebuild with: cmake -Denable_benchmarking=ON" << std::endl;
        return 1;
    #endif
#else
    std::cout << "Warning: JMATHS_BENCHMARKING is not defined!" << std::endl;
    std::cout << "Rebuild with: cmake -Denable_benchmarking=ON" << std::endl;
    return 1;
#endif

    try {
        benchmark_basic_n_operations();
        benchmark_basic_z_operations();
        benchmark_basic_q_operations();
        benchmark_calc_functions();
        benchmark_bitwise_operations();
        benchmark_random_generation();
        benchmark_conversions();
        benchmark_stream_operations();
        benchmark_literals();
        benchmark_edge_cases();
        benchmark_complex_expressions();

        std::cout << "\n========================================" << std::endl;
        std::cout << "  Benchmark Complete!" << std::endl;
        std::cout << "  Results saved to: " << log_file << std::endl;
        std::cout << "========================================" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error during benchmark: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
