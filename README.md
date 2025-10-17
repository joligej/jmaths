# jmaths

[![CI](https://img.shields.io/github/actions/workflow/status/joligej/jmaths/ci.yml?branch=main&label=CI&logo=github)](https://github.com/joligej/jmaths/actions/workflows/ci.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.28+-064F8C?logo=cmake)](https://cmake.org/)

A professional, high-performance C++23 library for arbitrary-precision arithmetic operations with comprehensive support for unlimited-size integers, rational numbers, and advanced mathematical computations.

## Features

- **Arbitrary-Precision Integers** (`N`, `Z`) — Unlimited-size integers with complete arithmetic support
- **Rational Numbers** (`Q`) — Arbitrary-precision fractions with automatic reduction to lowest terms
- **Modern C++23** — Leverages latest language features for clean, efficient, and type-safe code
- **Comprehensive Testing** — 550+ unit tests ensuring reliability and correctness
- **Constexpr Support** — Compile-time evaluation for most operations
- **User-Defined Literals** — Natural syntax using `_N`, `_Z`, and `_Q` literals
- **Rich Mathematical API** — GCD, square root, exponentiation, bitwise operations, and more
- **Production-Ready** — Thoroughly tested with continuous integration and quality assurance

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

- **Compiler** — GCC 15 or later with C++23 support
- **CMake** — 3.28.1 or later
- **Boost** — 1.70 or later (testing framework only)
- **binutils** — 2.43 or later (required for GCC 15)
- **Operating System** — Linux, macOS, or Windows (with appropriate toolchain)

> **Note**: When using GCC 15, ensure binutils 2.43 or later is installed. Older versions may cause assembler errors in debug builds. The CMake configuration automatically detects your binutils version and provides appropriate guidance. See [GCC15_REQUIREMENTS.md](doc/GCC15_REQUIREMENTS.md) for detailed information.

## Documentation

- **[API Reference](doc/API_Reference.md)** — Complete API documentation
- **[Quick Reference](doc/QUICK_REFERENCE.md)** — Common patterns and gotchas
- **[Algorithm Documentation](doc/algorithm_documentation.md)** — Algorithm explanations and complexity analysis
- **[Known Issues](doc/KNOWN_ISSUES.md)** — Known bugs and workarounds
- **[Exception Reference](doc/EXCEPTION_REFERENCE.md)** — Exception handling guide
- **[GCC 15 Requirements](doc/GCC15_REQUIREMENTS.md)** — Toolchain compatibility
- **[CI/CD Workflows](doc/WORKFLOWS.md)** — Automated testing pipeline
- **[Contributing](CONTRIBUTING.md)** — Contribution guidelines
- **[Security](SECURITY.md)** — Security policy and best practices

## Testing

The library includes a comprehensive test suite with over 550 test cases covering all major functionality:

```bash
cd build
ctest --output-on-failure
```

### Test Coverage

- **Arithmetic Operations** — All operations for N, Z, and Q types
- **Edge Cases** — Boundary conditions, zero handling, and special values
- **Type Conversions** — String parsing, numeric conversions, and range checking
- **Bitwise Operations** — Complete coverage of bitwise operations for integers
- **Mathematical Functions** — GCD, square root, exponentiation, and modular arithmetic
- **Error Handling** — Exception throwing and safety guarantees

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

- **Base Integer Types** — Customize underlying digit representation
- **Custom Allocators** — Provide specialized memory allocators
- **Karatsuba Multiplication** — Enable advanced multiplication algorithm (experimental)

## Performance

jmaths is engineered for high performance through several optimization strategies:

- **Zero-Cost Abstractions** — Template-based design with no runtime overhead
- **Efficient Algorithms** — Binary GCD, exponentiation by squaring, Karatsuba multiplication
- **Compile-Time Optimization** — Extensive use of `constexpr` and `[[nodiscard]]` attributes
- **Memory Efficiency** — Custom allocator support and minimal overhead per operation

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

- **Issues** — [GitHub Issues](https://github.com/joligej/jmaths/issues) for bug reports and feature requests
- **Discussions** — [GitHub Discussions](https://github.com/joligej/jmaths/discussions) for questions
- **Documentation** — [Complete documentation index](DOCUMENTATION_INDEX.md)

## Acknowledgments

- Implemented in **C++23** with modern language features
- Build system powered by **CMake**
- Testing framework provided by **Boost.Test**
- Continuous integration via **GitHub Actions**

---

**jmaths** — Professional arbitrary-precision arithmetic for C++23
