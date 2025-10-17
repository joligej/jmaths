# Contributing to jmaths

Thank you for your interest in contributing to jmaths! This document provides comprehensive guidelines and instructions to help you contribute effectively to the project.

## Code of Conduct

By participating in this project, you agree to abide by our [Code of Conduct](CODE_OF_CONDUCT.md). We are committed to maintaining a respectful, inclusive, and welcoming environment for all contributors.

## How Can I Contribute?

### Reporting Bugs

Before creating a bug report, please search existing issues to avoid duplicates. When creating a bug report, include the following information:

- **Clear, Descriptive Title** — Summarize the issue concisely
- **Detailed Steps to Reproduce** — Provide a step-by-step procedure
- **Expected Behavior** — Describe what you expected to happen
- **Actual Behavior** — Describe what actually happened
- **Environment Details** — Operating system, compiler version, jmaths version
- **Minimal Code Sample** — Provide the smallest code example that demonstrates the issue
- **Additional Context** — Any other relevant information (screenshots, logs, etc.)

### Suggesting Features

Feature requests are welcome and encouraged! When suggesting a feature, please provide:

- **Clear Description** — Explain the feature in detail
- **Use Cases and Benefits** — Describe why this feature would be valuable
- **Possible Implementation** — If you have ideas on how to implement it, share them
- **Alternative Solutions** — Consider and mention any alternative approaches

### Pull Requests

1. **Fork the repository** and create your branch from `main` or `develop`
2. **Make your changes** following the coding standards below
3. **Add tests** for any new functionality
4. **Ensure all tests pass**: `cmake --build build && ctest --test-dir build`
5. **Update documentation** if you've changed APIs or added features
6. **Write a clear commit message** describing your changes
7. **Submit a pull request** with a comprehensive description

## Development Setup

### Prerequisites

- **C++23 Compiler** — GCC 15 or later recommended
- **CMake** — Version 3.28.1 or higher
- **Boost Library** — Version 1.70 or higher (required for testing)
- **Git** — For version control and contribution workflow

### Building

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-15
cmake --build . -j8
```

### Running Tests

```bash
cd build
ctest --output-on-failure --verbose
```

## Coding Standards

### C++ Style Guidelines

- **Use Modern C++23 Features** — Leverage latest language capabilities appropriately
- **Follow Existing Code Style** — Maintain consistency with the codebase (see `.clang-format`)
- **Meaningful Names** — Use descriptive names for variables, functions, and classes
- **Focused Functions** — Keep functions concise and single-purpose
- **Const Correctness** — Apply `const` wherever applicable
- **Constexpr Usage** — Use `constexpr` for compile-time evaluable functions
- **Documentation** — Document complex logic and non-obvious implementations

### Code Formatting

We use `clang-format` for consistent formatting. Format your code before committing:

```bash
clang-format -i src/**/*.cpp src/**/*.hpp test/**/*.cpp
```

### Header Files

- Use `#pragma once` for include guards
- Include copyright and license header
- Add comprehensive documentation comments
- Order includes: standard library, third-party, project headers

### Documentation Standards

- **API Documentation** — Use Doxygen-style comments for all public APIs
- **Usage Examples** — Include practical examples in documentation
- **Update References** — Modify API_Reference.md when adding public interfaces
- **Algorithm Documentation** — Update algorithm_documentation.md for new algorithms
- **Inline Comments** — Explain complex or non-obvious code sections

### Testing Requirements

- **Write Tests** — Create Boost.Test unit tests for all new functionality
- **High Coverage** — Aim for comprehensive code coverage
- **Edge Cases** — Test boundary conditions and error scenarios
- **Descriptive Names** — Use clear, descriptive test case names
- **Test Organization** — Group related tests into logical test suites

**Example Test Structure:**
```cpp
BOOST_AUTO_TEST_SUITE(my_feature_tests)

BOOST_AUTO_TEST_CASE(test_basic_functionality) {
    // Arrange
    jmaths::N value = 42_N;
    
    // Act
    auto result = some_operation(value);
    
    // Assert
    BOOST_TEST(result == expected);
}

BOOST_AUTO_TEST_CASE(test_edge_case_zero) {
    jmaths::N zero = 0_N;
    BOOST_TEST(some_operation(zero) == expected_zero_result);
}

BOOST_AUTO_TEST_SUITE_END()
```

### Commit Message Format

Follow the Conventional Commits specification:

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:** `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`

**Examples:**
```
feat(basic_N): add modular exponentiation

Implement efficient modular exponentiation using binary
exponentiation algorithm for basic_N type.

Closes #123
```

```
fix(basic_Q): correct division by zero handling

Add proper exception throwing when attempting to divide
by zero in rational number operations.

Fixes #456
```

## Project Structure

```
jmaths/
├── .github/           # GitHub integration (workflows, templates)
├── doc/               # Documentation
├── samples/           # Example programs
├── src/               # Library source code
│   └── headers/       # Public header files
├── test/              # Test files
├── CMakeLists.txt     # Root CMake configuration
└── README.md          # Project overview
```

## License

By contributing, you agree that your contributions will be licensed under the GNU General Public License v3.0.

## Questions?

If you have questions or need assistance:

- **Open a Discussion** — Use [GitHub Discussions](https://github.com/joligej/jmaths/discussions) for questions
- **Check Documentation** — Review existing documentation and examples
- **Search Issues** — Look through open and closed issues for similar topics

We appreciate your contributions and look forward to collaborating with you!

