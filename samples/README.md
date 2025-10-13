# jmaths Samples

This directory contains example programs demonstrating the jmaths library.

## Building Samples

### Manual Compilation

You can manually compile samples after installing jmaths:

```bash
g++-15 -std=c++23 -O3 my_little_sample.cpp -ljmaths -o my_little_sample
./my_little_sample
```

Or from the repository root without installation:

```bash
g++-15 -std=c++23 -O3 -I src/headers -I build/src \
  -DJMATHS_KARATSUBA=0 -DJMATHS_PERIODICALLY_RESEED_RAND=0 -DJMATHS_BENCHMARKING=0 \
  samples/my_little_sample.cpp -o my_little_sample
./my_little_sample
```

## Available Samples

- **my_little_sample.cpp** - Basic usage demonstration
- **fig_terminal_comparison_example.cpp** - Comparison operations
- **fig_terminal_q_fits_into_example.cpp** - Type conversion examples
- **fig_terminal_z_random_example.cpp** - Random number generation

## Re-enabling Automated Builds

To re-enable samples in the CMake build system:

1. Uncomment `add_subdirectory(samples)` in the root `CMakeLists.txt`
2. Rebuild the project

```bash
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

The sample executables will be available in `build/samples/`.
