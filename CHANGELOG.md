# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.0.0] - 2025-01-XX

### Added
- Comprehensive Boost.Test unit test suite with 215+ test cases
- Parametric testing using Boost.Test data-driven framework
- Edge case testing for boundary conditions and error handling
- Performance benchmarking tests for all major operations
- GitHub Actions CI/CD with multiple workflows:
  - Main CI with Release and Debug builds
  - Code coverage reporting with Codecov integration
  - Static analysis with cppcheck
  - Security scanning with CodeQL
  - Dependency checking
  - Documentation validation
- Sanitizer testing (AddressSanitizer, UndefinedBehaviorSanitizer, LeakSanitizer)
- CODEOWNERS file for automatic code review assignments
- Dependabot configuration for automated dependency updates
- Leak Sanitizer suppressions file
- Professional GitHub integration:
  - CI/CD workflows for building and testing
  - Code coverage reporting
  - Automated release workflow
  - Issue templates (bug report, feature request, question)
  - Pull request template
- Documentation:
  - CONTRIBUTING.md with detailed contribution guidelines
  - SECURITY.md with security policy
  - Enhanced README.md with badges and better structure
- CMake improvements:
  - Forced g++-15 compiler usage
  - Better test organization
  - Improved error handling

### Changed
- Reorganized test structure into modular files
- Updated CMakeLists.txt for professional standards
- Enhanced API documentation in headers

### Fixed
- Missing `#pragma once` in formatter.hpp
- Missing include guards causing redefinition errors
- Documentation comment syntax errors in sign_type_impl.hpp

## [1.0.0] - Previous Release

Initial release with basic arbitrary-precision arithmetic functionality.
