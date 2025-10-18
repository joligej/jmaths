# jmaths Benchmarking System

This directory contains the comprehensive benchmarking suite for the jmaths library.

## Contents

- **benchmark_comprehensive.cpp** - Main benchmark program that exercises all API functions
- **analyze_benchmark.py** - Python script to analyze benchmark results
- **BENCHMARKING_GUIDE.md** - Complete guide on how to use the benchmarking system
- **CMakeLists.txt** - Build configuration for benchmarks

## Quick Usage

### 1. Build the Benchmark

```bash
cd /path/to/jmaths
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -Denable_benchmarking=ON
cmake --build . --target benchmark_comprehensive -j
```

### 2. Run the Benchmark

```bash
./benchmark_comprehensive
```

This creates a `benchmark_results.log` file with performance data.

### 3. Analyze Results

```bash
# Human-readable report
python3 ../benchmarks/analyze_benchmark.py benchmark_results.log

# CSV format (stdout)
python3 ../benchmarks/analyze_benchmark.py benchmark_results.log --csv

# CSV format (to file)
python3 ../benchmarks/analyze_benchmark.py benchmark_results.log --output results.csv
```

## What Gets Benchmarked

The comprehensive benchmark exercises:

- **N (unsigned integer)** - All arithmetic, bitwise, comparison operations
- **Z (signed integer)** - All operations including sign handling
- **Q (rational numbers)** - Fraction arithmetic and reduction
- **calc namespace** - GCD, sqrt, pow, pow_mod
- **Conversions** - String parsing/formatting, type conversions
- **Random generation** - Random number generation for N and Z
- **I/O operations** - Stream input/output
- **Edge cases** - Zero operations, large numbers, etc.

The benchmark is comprehensive and will take time to complete to gather extensive performance data.

## Requirements

- CMake 3.28.1 or later
- Python 3.6 or later (for analysis script)
- jmaths library built in Release mode
- Benchmarking toggle enabled (`-Denable_benchmarking=ON`)

## Documentation

See **BENCHMARKING_GUIDE.md** for complete documentation including:
- How benchmarking works internally
- Understanding the output format
- Interpreting analysis reports
- Performance optimization workflows
- Integration with CI/CD
- Troubleshooting

## Performance Notes

**IMPORTANT**: Always benchmark in Release mode for realistic performance data. Debug builds will show significantly slower execution times.

The benchmarking system uses a compile-time toggle, so when disabled (default), there is **zero performance overhead** in production code.

## Build Modes

### With Benchmarking (Development/Testing)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -Denable_benchmarking=ON
```

### Without Benchmarking (Production)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
# or explicitly:
cmake .. -DCMAKE_BUILD_TYPE=Release -Denable_benchmarking=OFF
```

## Example Output

The analysis script provides multiple views:
1. **Summary** - Total calls and unique functions
2. **Most Frequently Called** - Hot paths in the code
3. **Slowest by Total Time** - Cumulative performance impact
4. **Slowest by Average Time** - Per-call performance
5. **Performance Hotspots** - Best optimization targets (high frequency + slow)

## Support

For questions or issues, see BENCHMARKING_GUIDE.md or open an issue on GitHub.
