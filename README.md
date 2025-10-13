# jmaths

[![CI](https://img.shields.io/github/actions/workflow/status/joligej/jmaths/ci.yml?branch=main&label=CI&logo=github)](https://github.com/joligej/jmaths/actions/workflows/ci.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.28+-064F8C?logo=cmake)](https://cmake.org/)

A professional, high-performance C++ library for arbitrary-precision arithmetic with support for integers, rationals, and advanced mathematical operations.

## Features

- **Arbitrary-Precision Integers** (`N`, `Z`): Unlimited size integers with full arithmetic support
- **Rational Numbers** (`Q`): Arbitrary-precision fractions automatically reduced to lowest terms
- **Modern C++23**: Leverages latest language features for clean, efficient code
- **Comprehensive Testing**: 215+ unit tests with Boost.Test framework
- **Constexpr Support**: Most operations work at compile-time
- **User-Defined Literals**: Natural syntax with `_N`, `_Z`, `_Q` literals
- **Rich API**: GCD, square root, power, bitwise operations, and more

## Quick Start

### Installation

#### Using the build script (recommended)

```bash
./build_install_posix.sh
```

#### Manual installation

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-15
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Basic Usage

```cpp
#include <jmaths/all.hpp>
using namespace jmaths;
using namespace jmaths::literals;

int main() {
    // Arbitrary-precision unsigned integers
    N large = "123456789012345678901234567890"_N;
    N result = large * large;
    
    // Signed integers
    Z negative = -42_Z;
    Z positive = 100_Z;
    Z sum = negative + positive;  // 58
    
    // Rational numbers (fractions)
    Q half("1/2");
    Q third("1/3");
    Q sum_frac = half + third;  // 5/6
    
    // Mathematical functions
    N base(2);
    N exponent(100);
    N power = calc::pow(base, exponent);  // 2^100
    
    N a(48), b(18);
    N gcd = calc::gcd(a, b);  // 6
    
    return 0;
}
```

### Compilation

```bash
g++-15 -std=c++23 -O3 your_program.cpp -ljmaths -o your_program
```

## Requirements

- **Compiler**: g++-15 or later with C++23 support
- **CMake**: 3.28.1 or later
- **Boost**: 1.70 or later (for testing only)
- **binutils**: 2.43 or later (required for GCC 15)
- **Operating System**: Linux, macOS, or Windows (with appropriate toolchain)

> **Note**: When using g++-15, ensure binutils 2.43 or later is installed. Older versions will cause assembler errors in debug builds. The CMake configuration automatically detects your binutils version and provides guidance. See [GCC15_BINUTILS_ISSUE.md](GCC15_BINUTILS_ISSUE.md) for details.

## Documentation

- **[API Reference](doc/API_Reference.md)**: Complete API documentation
- **[Algorithm Documentation](doc/algorithm_documentation.md)**: Detailed algorithm explanations
- **[Contributing Guidelines](CONTRIBUTING.md)**: How to contribute to the project
- **[Code of Conduct](CODE_OF_CONDUCT.md)**: Community guidelines
- **[Security Policy](SECURITY.md)**: Security considerations and reporting

## Testing

The library includes a comprehensive test suite with 215+ test cases:

```bash
cd build
ctest --output-on-failure --verbose
```

Test coverage includes:
- **Basic Operations**: Arithmetic, comparison, assignment for N, Z, Q types
- **Edge Cases**: Boundary conditions, zero handling, overflow prevention
- **Parametric Tests**: Data-driven testing with multiple input sets
- **Performance Tests**: Benchmarking of all major operations
- **Type Conversions**: String parsing, numeric conversions, type safety
- **Bitwise Operations**: AND, OR, XOR, shifts for arbitrary-precision integers
- **Mathematical Functions**: GCD, square root, power, modular arithmetic
- **Random Number Generation**: Cryptographically secure random numbers
- **Error Handling**: Division by zero, invalid inputs, exception safety

## Project Structure

```
jmaths/
├── .github/              # GitHub integration (workflows, templates)
├── doc/                  # Documentation
├── src/
│   └── headers/          # Public header files
├── test/                 # Boost.Test unit tests
├── CMakeLists.txt        # Root CMake configuration
├── CONTRIBUTING.md       # Contribution guidelines
├── CODE_OF_CONDUCT.md    # Community guidelines
├── SECURITY.md           # Security policy
└── README.md             # This file
```

## Configuration Options

You can customize the library by editing `src/headers/constants_and_types.hpp.in` before building:

- **Base integer types**: Customize underlying digit types
- **Allocators**: Provide custom memory allocators
- **Karatsuba multiplication**: Toggle advanced multiplication algorithm (experimental)

## Performance

jmaths is designed for performance with:
- **Zero-cost abstractions**: Template-based design with no runtime overhead
- **Efficient algorithms**: Binary GCD, exponentiation by squaring, Karatsuba multiplication
- **Compile-time optimization**: Extensive use of `constexpr` and `[[nodiscard]]`
- **Memory efficiency**: Custom allocator support and minimal overhead

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Support

- **Issues**: [GitHub Issues](https://github.com/joligej/jmaths/issues)
- **Discussions**: [GitHub Discussions](https://github.com/joligej/jmaths/discussions)
- **Email**: See maintainer information in commit history

## Acknowledgments

- Built with modern C++23 and CMake
- Uses Boost.Test for unit testing
- Inspired by various arbitrary-precision arithmetic libraries

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for version history and release notes.

---

**Note**: This library requires a modern C++23 compiler. Ensure your toolchain is up to date before building.

