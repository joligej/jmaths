# jmaths Sample Programs

This directory contains example programs demonstrating various features and capabilities of the jmaths library.

## Available Samples

### Basic Examples

- **my_little_sample.cpp** — Introduction to basic library usage and fundamental operations
- **fig_terminal_comparison_example.cpp** — Demonstrates comparison operations across different number types
- **fig_terminal_q_fits_into_example.cpp** — Examples of type conversions and numeric type interactions
- **fig_terminal_z_random_example.cpp** — Random number generation with arbitrary-precision integers

## Building Samples

### Method 1: Manual Compilation (After Installation)

After installing jmaths system-wide, compile samples directly:

```bash
g++-15 -std=c++23 -O3 my_little_sample.cpp -ljmaths -o my_little_sample
./my_little_sample
```

### Method 2: Compilation Without Installation

Compile from the repository root without installing:

```bash
g++-15 -std=c++23 -O3 \
  -I src/headers \
  -I build/src \
  -DJMATHS_KARATSUBA=0 \
  -DJMATHS_PERIODICALLY_RESEED_RAND=0 \
  -DJMATHS_BENCHMARKING=0 \
  samples/my_little_sample.cpp \
  -o my_little_sample

./my_little_sample
```

### Method 3: CMake Build System (Optional)

To enable automated sample compilation through CMake:

1. Edit the root `CMakeLists.txt` file
2. Uncomment the line: `add_subdirectory(samples)`
3. Rebuild the project:

```bash
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

Sample executables will be generated in `build/samples/`.

## Sample Descriptions

### my_little_sample.cpp

Introduction to the jmaths library, covering:

- Basic number creation and initialization
- Arithmetic operations
- String conversion and output
- User-defined literals

### fig_terminal_comparison_example.cpp

Comprehensive demonstration of comparison operations:

- Equality and inequality testing
- Relational comparisons (<, >, <=, >=)
- Three-way comparison operator (<=>)
- Cross-type comparisons

### fig_terminal_q_fits_into_example.cpp

Type conversion examples including:

- Converting between N, Z, and Q types
- Checking if rational numbers represent integers
- Extracting integer parts from fractions
- Safe numeric conversions

### fig_terminal_z_random_example.cpp

Random number generation features:

- Generating random arbitrary-precision integers
- Setting ranges for random values
- Cryptographically secure random numbers
- Seeding and reproducibility

## Learning Path

For newcomers to jmaths, we recommend exploring samples in this order:

1. **my_little_sample.cpp** — Start here for basic concepts
2. **fig_terminal_comparison_example.cpp** — Learn about comparisons
3. **fig_terminal_q_fits_into_example.cpp** — Understand type conversions
4. **fig_terminal_z_random_example.cpp** — Explore random number generation

## Additional Resources

- **API Reference** — See [API_Reference.md](API_Reference.md) for complete API documentation
- **Algorithm Documentation** — See [algorithm_documentation.md](algorithm_documentation.md) for implementation details
- **Main README** — See [README.md](../README.md) for general library information

## Compilation Notes

- **C++23 Required** — All samples require a C++23-compliant compiler
- **Optimization** — Use `-O3` for best performance in production
- **Debug Builds** — Use `-g -O0` for debugging with full symbol information

## Troubleshooting

If you encounter compilation errors:

1. Verify compiler version: `g++-15 --version`
2. Ensure binutils 2.43+ is installed: `as --version`
3. Check that jmaths is properly installed (for Method 1)
4. Verify include paths are correct (for Method 2)

For additional help, see [CONTRIBUTING.md](../CONTRIBUTING.md) or the [GCC 15 Requirements Guide](GCC15_REQUIREMENTS.md).

