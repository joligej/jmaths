# Quick Reference: Running Benchmarks

## One-Line Commands

### Build Benchmark (from project root)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -Denable_benchmarking=ON && cmake --build build --target benchmark_comprehensive -j
```

### Run Benchmark
```bash
./build/benchmarks/benchmark_comprehensive
```

### Analyze Results
```bash
python3 benchmarks/analyze_benchmark.py benchmark_results.log

# Or in CSV format:
python3 benchmarks/analyze_benchmark.py benchmark_results.log --csv

# Or save CSV to file:
python3 benchmarks/analyze_benchmark.py benchmark_results.log --output results.csv
```

## Complete Workflow

```bash
# From project root directory
cd /path/to/jmaths

# 1. Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release -Denable_benchmarking=ON
cmake --build build --target benchmark_comprehensive -j

# 2. Run benchmark
cd build/benchmarks
./benchmark_comprehensive my_results.log

# 3. Analyze
python3 ../../benchmarks/analyze_benchmark.py my_results.log

# 4. View results (saved in terminal output)
# Or redirect to file:
python3 ../../benchmarks/analyze_benchmark.py my_results.log > analysis_report.txt
```

## What You'll See

### During Benchmark Run
```
========================================
  jmaths Comprehensive Benchmark Suite
========================================
Benchmarking: ENABLED
Benchmarking output redirected to: my_results.log

=== Benchmarking N (unsigned integer) operations ===
N operations completed

=== Benchmarking Z (signed integer) operations ===
Z operations completed

... (more sections) ...

========================================
  Benchmark Complete!
  Results saved to: my_results.log
========================================
```

### Analysis Output
```
================================================================================
JMATHS BENCHMARK ANALYSIS REPORT
================================================================================

Summary:
  Total function calls: 17,175
  Unique functions:     150

--------------------------------------------------------------------------------
TOP 20 MOST FREQUENTLY CALLED FUNCTIONS
--------------------------------------------------------------------------------
   Calls  Function
--------------------------------------------------------------------------------
    5250  constexpr bool jmaths::basic_N<>::is_zero() const
    2482  constexpr void jmaths::basic_N<>::remove_leading_zeroes_()
    ...

--------------------------------------------------------------------------------
TOP 20 SLOWEST FUNCTIONS (by total time)
--------------------------------------------------------------------------------
  Total ms      Avg ms     Calls  Function
--------------------------------------------------------------------------------
     7.652    0.060730       126  static constexpr std::pair<...> opr_div(...)
    ...

--------------------------------------------------------------------------------
PERFORMANCE HOTSPOTS (high frequency + high time)
--------------------------------------------------------------------------------
   Score    Total ms     Calls  Function
--------------------------------------------------------------------------------
   16.07       7.652       126  opr_div(...)
    ...
```

## Key Metrics

- **Total Calls**: ~17,000 function calls
- **Unique Functions**: ~150 different functions
- **Log File Size**: ~3-4 MB
- **Runtime**: Varies (designed to be comprehensive)
- **Most Called**: `is_zero()` - critical performance path
- **Slowest**: Division operations - optimization target

## CSV Output Format

The analysis script can generate CSV output with the following columns:

- **function** - Function name/signature
- **calls** - Number of times the function was called
- **total_time_ms** - Total cumulative time in milliseconds
- **avg_time_ms** - Average time per call in milliseconds
- **min_time_ms** - Minimum time for any single call
- **max_time_ms** - Maximum time for any single call

Example CSV usage:
```bash
# Generate CSV and import into Excel/LibreOffice
python3 analyze_benchmark.py benchmark.log --output results.csv

# Generate CSV and process with other tools
python3 analyze_benchmark.py benchmark.log --csv | sort -t, -k3 -n -r | head -20

# Save to file for spreadsheet analysis
python3 analyze_benchmark.py benchmark.log --output analysis.csv
```

The CSV format makes it easy to:
- Import into spreadsheets for further analysis
- Generate charts and graphs
- Compare results across different runs
- Filter and sort by specific metrics

## Tips

1. **Always use Release mode** - Debug builds are much slower
2. **Run multiple times** - Average results for consistency
3. **Close other apps** - Reduce system noise
4. **Check CPU load** - Ensure consistent conditions
5. **Look at hotspots** - Best optimization targets

## Troubleshooting

### "JMATHS_BENCHMARKING is not defined"
```bash
# Solution: Enable benchmarking in CMake
cmake .. -Denable_benchmarking=ON
```

### Empty log file
```bash
# Solution: Check that benchmark was built with benchmarking ON
./build/benchmarks/benchmark_comprehensive
# Should show "Benchmarking: ENABLED"
```

### Analysis script error
```bash
# Solution: Check Python 3 is installed and log file exists
python3 --version
ls -lh benchmark_results.log
```

## Documentation

- **Quick Start**: `benchmarks/README.md`
- **Complete Guide**: `benchmarks/BENCHMARKING_GUIDE.md`
- **System Status**: `BENCHMARKING_STATUS.md`

## Example Session

```bash
$ cd /Users/jasper/CLionProjects/jmaths

$ cmake -B build -DCMAKE_BUILD_TYPE=Release -Denable_benchmarking=ON
-- Benchmarking: ENABLED
-- Configuring done
-- Generating done

$ cmake --build build --target benchmark_comprehensive -j
[100%] Built target benchmark_comprehensive

$ cd build/benchmarks

$ ./benchmark_comprehensive test.log
========================================
  jmaths Comprehensive Benchmark Suite
========================================
Benchmarking: ENABLED
...
Benchmark Complete!
Results saved to: test.log

$ python3 ../../benchmarks/analyze_benchmark.py test.log
Analyzing benchmark log: test.log

================================================================================
JMATHS BENCHMARK ANALYSIS REPORT
================================================================================

Summary:
  Total function calls: 17,175
  Unique functions:     150
...

$ ls -lh test.log
-rw-r--r--  1 user  staff   3.8M Oct 18 10:19 test.log
```

That's it! The benchmarking system is ready to use.
