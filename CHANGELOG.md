# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Additional mathematical functions (factorial, combinatorics)
- Performance optimizations for large number operations

## [2.0.0] - 2025-01-17

### Added

#### Testing Infrastructure
- Comprehensive Boost.Test unit test suite with 550+ test cases
- Parametric testing using data-driven framework
- Edge case and boundary condition testing
- Performance benchmarking for major operations

#### Continuous Integration
- CI workflow with Release and Debug builds
- Code coverage reporting with Codecov
- Static analysis with cppcheck
- Security scanning with CodeQL
- Sanitizer testing (Address, Undefined Behavior, Leak)

#### Documentation
- Comprehensive API documentation with examples
- Exception handling reference
- Algorithm complexity analysis
- GCC 15 toolchain compatibility guide
- Known issues and workarounds documentation
- Quick reference guide for common patterns
- CI/CD workflow documentation
- Professional contribution guidelines
- Security policy and best practices

### Changed
- Reorganized test structure for maintainability
- Enhanced API documentation with detailed examples
- Improved build system with better error messages

### Fixed
- Include guard issues in header files
- Documentation comment syntax errors
- Build compatibility across platforms

## [1.0.0] - 2024-12-01

### Added
- Initial release with arbitrary-precision arithmetic
- Support for unsigned integers (N), signed integers (Z), and rational numbers (Q)
- Arithmetic operations (addition, subtraction, multiplication, division)
- Mathematical functions (GCD, square root, exponentiation)
- Bitwise operations for integer types
- String conversion and parsing
- User-defined literals (`_N`, `_Z`, `_Q`)
- Constexpr support for compile-time evaluation
- Standard library integration (streams, hashing)

---

For detailed release information, see [GitHub Releases](https://github.com/joligej/jmaths/releases).

