# jmaths Benchmarking Guide

## Overview

The jmaths library includes a comprehensive benchmarking system that allows you to measure the performance of all library functions. The benchmarking system uses a toggle mechanism, so it can be enabled during development and testing, then disabled for production release builds.

## Quick Start

### 1. Build with Benchmarking Enabled

```bash
# Configure CMake with benchmarking enabled
cd /path/to/jmaths
mkdir build-benchmark && cd build-benchmark
cmake .. -DCMAKE_BUILD_TYPE=Release -Denable_benchmarking=ON

# Build the benchmark executable
cmake --build . --target benchmark_comprehensive -j
```

### 2. Run the Benchmark

```bash
# Run with default output file (benchmark_results.log)
./benchmark_comprehensive

# Or specify a custom output file
./benchmark_comprehensive my_results.log
```

### 3. Analyze the Results

```bash
# Analyze the benchmark log file
python3 ../analyze_benchmark.py benchmark_results.log
```

## How Benchmarking Works

### Toggle Mechanism

Benchmarking is controlled by the CMake option `enable_benchmarking`:

- **OFF (default)**: No performance overhead. Functions execute normally without any timing code.
- **ON**: Performance measurement enabled. Each function call is logged with timing information.

This is implemented via the `JMATHS_BENCHMARKING` preprocessor macro which is set to 0 or 1 based on the CMake option.

### What Gets Measured

When benchmarking is enabled, the system tracks:

1. **Function Calls**: Every call to instrumented functions is logged
2. **Execution Time**: Precise timing in milliseconds for each function call
3. **Call Frequency**: How many times each function is called
4. **Performance Patterns**: Which functions consume the most time

### Instrumented Functions

The benchmarking system measures performance across all major areas:

- **N (unsigned integer)**: Construction, arithmetic, comparison, bitwise operations
- **Z (signed integer)**: Construction, arithmetic, negation, sign operations
- **Q (rational numbers)**: Construction, arithmetic, reduction
- **calc functions**: GCD, square root, power, modular exponentiation
- **Conversions**: String parsing and formatting, type conversions
- **Random generation**: Random number generation for N and Z types
- **I/O operations**: Stream input/output

## Understanding the Output

### Benchmark Log Format

The log file contains tab-separated entries:

```
log:call    <function_name>
log:time    <duration_ms>    ms    from    <function_name>
```

Example:
```
log:call    basic_N::operator+
log:time    0.042    ms    from    basic_N::operator+
log:call    basic_N::operator*
log:time    0.156    ms    from    basic_N::operator*
```

### Analysis Report Sections

The Python analysis script generates several views of the data:

#### 1. Summary Statistics
- Total number of function calls
- Number of unique functions called

#### 2. Top 20 Most Frequently Called Functions
Shows which functions are called most often, helping identify hot paths in your code.

#### 3. Top 20 Slowest Functions (by total time)
Shows functions that consume the most cumulative time across all calls.

#### 4. Top 20 Slowest Functions (by average time)
Shows functions with the highest per-call execution time.

#### 5. Performance Hotspots
Identifies functions that are both frequently called AND slow - the prime targets for optimization.

## Comprehensive Benchmark Suite

The `benchmark_comprehensive` executable runs an extensive test suite that exercises:

### Basic Operations
- N, Z, Q construction from various sources
- All arithmetic operators (+, -, *, /)
- Comparison operators (==, !=, <, >, <=, >=)
- Compound assignment operators (+=, -=, *=, /=)
- Increment/decrement operators

### Advanced Operations
- Bitwise operations (&, |, ^, ~, <<, >>)
- Power operations (pow, pow_mod)
- Mathematical functions (gcd, sqrt)
- Random number generation

### Edge Cases
- Zero operations
- Division by 1
- Very large numbers (50+ digits)
- Complex expression evaluation

### Conversions
- String to number (decimal, hex, binary)
- Number to string (various bases)
- Type fitting checks (fits_into<T>)

### I/O Operations
- Stream input/output
- Multiple value processing

The benchmark is designed to take some time to complete (typically 30 seconds to a few minutes) to gather comprehensive performance data.

## Interpreting Results

### What to Look For

1. **Unexpectedly High Call Counts**: If a simple operation calls a function thousands of times, there may be inefficiency in the implementation.

2. **Slow Individual Operations**: Functions with high average execution times may need algorithmic improvements.

3. **Performance Hotspots**: Functions that score high on the hotspot metric (high time * log(calls)) are the best candidates for optimization.

### Example Optimization Workflow

1. Run benchmark: `./benchmark_comprehensive results.log`
2. Analyze results: `python3 analyze_benchmark.py results.log`
3. Identify top 3 hotspots
4. Profile those specific functions
5. Implement optimizations
6. Re-run benchmark to verify improvements

## Building Without Benchmarking

For production/release builds, configure without benchmarking:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -Denable_benchmarking=OFF
```

Or simply omit the option (OFF is the default):

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

This ensures zero performance overhead from the benchmarking system.

## Adding Benchmarking to Your Own Code

If you're using jmaths in your project and want to benchmark your own code:

### Option 1: Use the scoped_timer directly

```cpp
#include "scoped_timer.hpp"

void my_function() {
#if JMATHS_BENCHMARKING == 1
    jmaths::internal::scoped_timer timer(__func__);
#endif
    // Your code here
}
```

### Option 2: Use JMATHS_FUNCTION_TO_LOG macro

```cpp
void my_function() {
    JMATHS_FUNCTION_TO_LOG;
    // Your code here
}
```

This macro automatically expands to nothing when benchmarking is disabled.

## Advanced Usage

### Custom Output Streams

The benchmark system can redirect output to different streams:

```cpp
#include "scoped_timer.hpp"

// Redirect to a file
std::ofstream log_file("custom.log");
jmaths::internal::scoped_timer::set_ostream(&log_file);

// Disable logging
jmaths::internal::scoped_timer::set_ostream(nullptr);

// Restore default (std::clog)
jmaths::internal::scoped_timer::set_ostream(&std::clog);
```

### Benchmark Subsets

To benchmark specific operations, modify `benchmark_comprehensive.cpp` to comment out sections you don't need:

```cpp
int main() {
    // ... setup code ...

    benchmark_basic_n_operations();
    benchmark_basic_z_operations();
    // benchmark_basic_q_operations();  // Skip Q operations
    benchmark_calc_functions();
    // ...
}
```

### Integration with CI/CD

You can integrate benchmarking into your CI/CD pipeline:

```bash
#!/bin/bash
# benchmark_regression.sh

# Build with benchmarking
cmake .. -Denable_benchmarking=ON
cmake --build . --target benchmark_comprehensive

# Run benchmark
./benchmark_comprehensive current.log

# Compare with baseline
if [ -f baseline.log ]; then
    python3 compare_benchmarks.py baseline.log current.log
fi
```

## Performance Considerations

### Overhead

When enabled, benchmarking adds:
- ~20-50 nanoseconds per function call for timing
- Disk I/O for writing log entries
- Minimal memory overhead for the timer objects

### Optimization Notes

1. Use Release builds for benchmarking to get realistic performance data
2. Run benchmarks multiple times and average results
3. Close unnecessary applications to reduce system noise
4. Consider CPU frequency scaling and thermal throttling

## Troubleshooting

### Benchmark won't build

Error: `JMATHS_BENCHMARKING is not defined`

Solution: Ensure you configured with `-Denable_benchmarking=ON`

### Empty or missing log file

Issue: Benchmark runs but produces no output

Solution: Check that the benchmark was actually compiled with benchmarking enabled. Verify with:
```bash
./benchmark_comprehensive
# Should print "Benchmarking: ENABLED"
```

### Analysis script fails

Error: `File not found: benchmark_results.log`

Solution: Ensure the log file was created. Check the benchmark output for the actual filename used.

## See Also

- [API Reference](API_Reference.md) - Complete function documentation
- [Algorithm Documentation](algorithm_documentation.md) - Implementation details
- [WORKFLOWS.md](WORKFLOWS.md) - Development workflows

## Support

For questions or issues with benchmarking:
1. Check this guide thoroughly
2. Review existing GitHub issues
3. Open a new issue with benchmark logs and system info
