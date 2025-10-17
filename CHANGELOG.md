# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Additional mathematical functions (factorial, combinatorics)
- Extended documentation with more examples
- Performance optimizations for large number operations

## [2.0.0] - 2025-01-XX

### Added

#### Testing Infrastructure
- Comprehensive Boost.Test unit test suite with over 215 test cases
- Parametric testing using Boost.Test data-driven framework
- Edge case testing for boundary conditions and error handling scenarios
- Performance benchmarking tests for all major operations
- Test coverage tracking and reporting

#### Continuous Integration & Deployment
- **Main CI Workflow** with Release and Debug builds
- **Code Coverage Reporting** with Codecov integration
- **Static Analysis** with cppcheck for code quality assurance
- **Security Scanning** with GitHub CodeQL
- **Dependency Checking** for build environment validation
- **Documentation Validation** for completeness and accuracy
- Sanitizer testing (AddressSanitizer, UndefinedBehaviorSanitizer, LeakSanitizer)

#### GitHub Integration
- CODEOWNERS file for automatic code review assignments
- Dependabot configuration for automated dependency updates
- Leak Sanitizer suppressions file for known safe patterns
- Professional issue templates (bug report, feature request, question)
- Pull request template with comprehensive checklist
- Automated release workflow for distribution

#### Documentation
- Comprehensive CONTRIBUTING.md with detailed contribution guidelines
- SECURITY.md with security policy and vulnerability reporting process
- Enhanced README.md with badges, better structure, and clearer examples
- doc/WORKFLOWS.md documenting the CI/CD pipeline
- GCC15_REQUIREMENTS.md for toolchain compatibility information

### Changed
- Reorganized test structure into modular, maintainable files
- Updated CMakeLists.txt to meet professional standards
- Enhanced API documentation in header files with detailed examples
- Improved build system with better error messages and validation

### Fixed
- Missing `#pragma once` directive in formatter.hpp
- Missing include guards causing potential redefinition errors
- Documentation comment syntax errors in sign_type_impl.hpp
- Build system compatibility issues across different platforms

### Technical Improvements
- Enforced compiler flag `-Werror` for warnings-as-errors policy
- Added comprehensive code formatting checks with clang-format
- Implemented build caching for faster CI execution
- Enhanced CMake configuration with better dependency management

## [1.0.0] - 2024-XX-XX

### Added
- Initial release with arbitrary-precision arithmetic functionality
- Support for unsigned integers (N), signed integers (Z), and rational numbers (Q)
- Basic arithmetic operations (addition, subtraction, multiplication, division)
- Mathematical functions (GCD, square root, exponentiation)
- Bitwise operations for integer types
- String conversion and parsing
- User-defined literals for convenient syntax
- Constexpr support for compile-time evaluation
- Standard library integration (streams, hashing, formatting)

---

For more details on each release, see the [GitHub Releases](https://github.com/joligej/jmaths/releases) page.

