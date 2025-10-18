#!/usr/bin/env python3
"""
Analyze jmaths benchmark results
Generates statistics on function call frequency and performance
Supports both human-readable and CSV output formats
"""

import sys
import csv
from collections import defaultdict
from pathlib import Path


def analyze_benchmark_log(log_file):
    """Parse and analyze benchmark log file"""

    call_counts = defaultdict(int)
    time_data = defaultdict(list)

    with open(log_file, 'r') as f:
        for line in f:
            parts = line.strip().split('\t')
            if len(parts) < 2:
                continue

            log_type = parts[0]

            if log_type == 'log:call':
                func_name = parts[1]
                call_counts[func_name] += 1

            elif log_type == 'log:time':
                if len(parts) >= 5:
                    time_ms = float(parts[1])
                    func_name = parts[4]
                    time_data[func_name].append(time_ms)

    return call_counts, time_data


def print_report(call_counts, time_data):
    """Print formatted analysis report"""

    print("=" * 80)
    print("JMATHS BENCHMARK ANALYSIS REPORT")
    print("=" * 80)

    # Summary statistics
    total_calls = sum(call_counts.values())
    total_functions = len(call_counts)
    print(f"\nSummary:")
    print(f"  Total function calls: {total_calls:,}")
    print(f"  Unique functions:     {total_functions:,}")

    # Most frequently called functions
    print(f"\n{'-' * 80}")
    print("TOP 20 MOST FREQUENTLY CALLED FUNCTIONS")
    print(f"{'-' * 80}")
    print(f"{'Calls':>8}  Function")
    print(f"{'-' * 80}")

    sorted_calls = sorted(call_counts.items(), key=lambda x: x[1], reverse=True)
    for func, count in sorted_calls[:20]:
        print(f"{count:>8}  {func}")

    # Slowest functions (by total time)
    print(f"\n{'-' * 80}")
    print("TOP 20 SLOWEST FUNCTIONS (by total time)")
    print(f"{'-' * 80}")
    print(f"{'Total ms':>10}  {'Avg ms':>10}  {'Calls':>8}  Function")
    print(f"{'-' * 80}")

    total_times = []
    for func, times in time_data.items():
        total_ms = sum(times)
        avg_ms = total_ms / len(times)
        count = len(times)
        total_times.append((total_ms, avg_ms, count, func))

    total_times.sort(reverse=True)
    for total_ms, avg_ms, count, func in total_times[:20]:
        print(f"{total_ms:>10.3f}  {avg_ms:>10.6f}  {count:>8}  {func}")

    # Slowest functions (by average time)
    print(f"\n{'-' * 80}")
    print("TOP 20 SLOWEST FUNCTIONS (by average time)")
    print(f"{'-' * 80}")
    print(f"{'Avg ms':>10}  {'Total ms':>10}  {'Calls':>8}  Function")
    print(f"{'-' * 80}")

    avg_times = []
    for func, times in time_data.items():
        total_ms = sum(times)
        avg_ms = total_ms / len(times)
        count = len(times)
        avg_times.append((avg_ms, total_ms, count, func))

    avg_times.sort(reverse=True)
    for avg_ms, total_ms, count, func in avg_times[:20]:
        print(f"{avg_ms:>10.6f}  {total_ms:>10.3f}  {count:>8}  {func}")

    # Performance hotspots (functions with significant total time AND many calls)
    print(f"\n{'-' * 80}")
    print("PERFORMANCE HOTSPOTS (high frequency + high time)")
    print(f"{'-' * 80}")
    print(f"{'Score':>8}  {'Total ms':>10}  {'Calls':>8}  Function")
    print(f"{'-' * 80}")

    hotspots = []
    for func, times in time_data.items():
        total_ms = sum(times)
        count = len(times)
        # Score = total_time * log(calls) to find functions that are both slow and frequent
        import math
        if count > 1:
            score = total_ms * math.log10(count)
            hotspots.append((score, total_ms, count, func))

    hotspots.sort(reverse=True)
    for score, total_ms, count, func in hotspots[:15]:
        print(f"{score:>8.2f}  {total_ms:>10.3f}  {count:>8}  {func}")

    print(f"\n{'=' * 80}")


def print_csv_report(call_counts, time_data, output_file=None):
    """Print CSV formatted analysis report"""

    # Prepare data: combine call counts and timing data
    function_stats = []

    for func in call_counts.keys():
        calls = call_counts[func]

        if func in time_data and time_data[func]:
            times = time_data[func]
            total_time_ms = sum(times)
            avg_time_ms = total_time_ms / len(times)
            min_time_ms = min(times)
            max_time_ms = max(times)
        else:
            total_time_ms = 0.0
            avg_time_ms = 0.0
            min_time_ms = 0.0
            max_time_ms = 0.0

        function_stats.append({
            'function': func,
            'calls': calls,
            'total_time_ms': total_time_ms,
            'avg_time_ms': avg_time_ms,
            'min_time_ms': min_time_ms,
            'max_time_ms': max_time_ms
        })

    # Sort by total time (descending)
    function_stats.sort(key=lambda x: x['total_time_ms'], reverse=True)

    # Write CSV
    fieldnames = ['function', 'calls', 'total_time_ms', 'avg_time_ms', 'min_time_ms', 'max_time_ms']

    if output_file:
        with open(output_file, 'w', newline='') as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(function_stats)
        print(f"CSV report written to: {output_file}")
    else:
        writer = csv.DictWriter(sys.stdout, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(function_stats)


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 analyze_benchmark.py <benchmark_log_file> [--csv] [--output <csv_file>]")
        print()
        print("Options:")
        print("  --csv             Output in CSV format instead of human-readable")
        print("  --output FILE     Write CSV output to FILE (implies --csv)")
        print()
        print("Examples:")
        print("  python3 analyze_benchmark.py benchmark.log")
        print("  python3 analyze_benchmark.py benchmark.log --csv")
        print("  python3 analyze_benchmark.py benchmark.log --output results.csv")
        sys.exit(1)

    log_file = sys.argv[1]
    csv_mode = '--csv' in sys.argv
    output_file = None

    # Check for --output option
    if '--output' in sys.argv:
        csv_mode = True
        try:
            output_idx = sys.argv.index('--output')
            if output_idx + 1 < len(sys.argv):
                output_file = sys.argv[output_idx + 1]
            else:
                print("Error: --output requires a filename")
                sys.exit(1)
        except ValueError:
            pass

    if not Path(log_file).exists():
        print(f"Error: File not found: {log_file}")
        sys.exit(1)

    if not csv_mode:
        print(f"Analyzing benchmark log: {log_file}\n")

    call_counts, time_data = analyze_benchmark_log(log_file)

    if csv_mode:
        print_csv_report(call_counts, time_data, output_file)
    else:
        print_report(call_counts, time_data)


if __name__ == "__main__":
    main()
