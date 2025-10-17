# Continuous Integration & Deployment

[![CI Status](https://img.shields.io/github/actions/workflow/status/jmaths/jmaths/ci.yml?branch=main&label=CI&logo=github)](https://github.com/jmaths/jmaths/actions/workflows/ci.yml)
[![Security](https://img.shields.io/github/actions/workflow/status/jmaths/jmaths/security.yml?branch=main&label=Security&logo=github)](https://github.com/jmaths/jmaths/actions/workflows/security.yml)
[![Coverage](https://img.shields.io/codecov/c/github/jmaths/jmaths?logo=codecov)](https://codecov.io/gh/jmaths/jmaths)

This document provides a comprehensive overview of the automated workflows that maintain code quality, security, and reliability standards throughout the development lifecycle.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Workflow Catalog](#workflow-catalog)
  - [Continuous Integration](#continuous-integration)
  - [Release Management](#release-management)
  - [Security Scanning](#security-scanning)
  - [Dependency Management](#dependency-management)
  - [Documentation Validation](#documentation-validation)
  - [Code Coverage](#code-coverage)
- [Workflow Coordination](#workflow-coordination)
- [Performance Optimization](#performance-optimization)
- [Security & Permissions](#security--permissions)
- [Monitoring & Maintenance](#monitoring--maintenance)
- [Developer Guidelines](#developer-guidelines)

---

## Overview

The project implements a robust CI/CD pipeline consisting of six specialized GitHub Actions workflows. These workflows automatically validate code quality, detect security vulnerabilities, verify dependencies, maintain documentation standards, measure test coverage, and automate release distribution. The system operates continuously through automated triggers on code changes and scheduled interval checks.

---

## Workflow Catalog

### Continuous Integration

> **Workflow:** `.github/workflows/ci.yml`  
> **Triggers:** Push to `main`/`develop` • Pull requests • Weekly schedule (Sunday 00:00 UTC) • Manual dispatch

The cornerstone of the development pipeline, this workflow performs comprehensive validation across multiple dimensions to ensure code reliability and maintainability.

#### Build & Test Matrix

The workflow executes parallel builds across different configurations to validate cross-platform compatibility and build stability:

| Component | Configuration |
|-----------|---------------|
| **Operating System** | Ubuntu Latest |
| **Build Types** | Release, Debug |
| **Compiler** | GCC 14 with C++20 support |
| **Dependencies** | Boost (all components), Binutils 2.43+ |
| **Build System** | CMake 3.28.1+ |

**Key Operations:**
- Multi-configuration matrix builds (Release/Debug) with intelligent caching
- Complete test suite execution via CTest with JUnit XML output
- Automated package generation (`.tar.gz`) for release artifacts
- Test result archival for historical tracking and analysis

#### Code Quality Assurance

Enforces consistent code style and formatting standards:

- **Format Validation:** clang-format conformance checking
- **Whitespace Policy:** Trailing whitespace detection and prevention
- **Character Encoding:** Tab character prohibition (spaces-only policy)

#### Static Analysis

Deep source code inspection using industry-standard tools:

- **Tool:** cppcheck with all checks enabled
- **Scope:** Security vulnerabilities, undefined behavior, code smells
- **Output:** Detailed reports with inline suppression support
- **Integration:** Automated report generation and artifact preservation

#### Runtime Validation (Sanitizers)

Parallel sanitizer execution for comprehensive runtime error detection:

| Sanitizer | Purpose | Detection Scope |
|-----------|---------|-----------------|
| **AddressSanitizer** | Memory safety | Heap/stack overflows, use-after-free, double-free |
| **UndefinedBehaviorSanitizer** | Language conformance | Integer overflow, null dereference, alignment issues |
| **LeakSanitizer** | Resource management | Memory leaks, resource leaks |

Each sanitizer runs in isolated jobs with specialized instrumentation flags and runtime options optimized for maximum detection accuracy.

### Release Management

> **Workflow:** `.github/workflows/release.yml`  
> **Triggers:** Version tags (`v*.*.*`) • Manual dispatch

Fully automated release pipeline that transforms version tags into distributable releases with minimal manual intervention.

**Release Process:**

1. **Build Validation:** Full Release build with GCC 14 and optimizations enabled
2. **Quality Gate:** Complete test suite execution (release fails if tests fail)
3. **Package Creation:** CPack-generated distribution archives
4. **Release Publication:** Automated GitHub release with generated changelog
5. **Asset Distribution:** Compiled packages attached for immediate download

**Semantic Versioning:**
- Tags follow `v{MAJOR}.{MINOR}.{PATCH}` convention
- Automatic release notes generation from commit history
- Public releases (non-draft, non-prerelease) by default

---

### Security Scanning

> **Workflow:** `.github/workflows/security.yml`  
> **Triggers:** Push to `main`/`develop` • Pull requests • Weekly schedule (Tuesday 00:00 UTC) • Manual dispatch

Multi-layered security analysis providing continuous vulnerability monitoring and dependency security assessment.

#### CodeQL Analysis

Advanced semantic code analysis powered by GitHub's security infrastructure:

- **Language Support:** C++ with full semantic understanding
- **Query Suite:** Security-and-quality patterns (CWE coverage)
- **Detection Capabilities:**
  - SQL injection vulnerabilities
  - Command injection risks
  - Memory safety violations
  - Buffer overflows and underflows
  - Resource leaks and race conditions
- **Integration:** Results published to Security tab with SARIF format

#### Dependency Review

Automated supply chain security for pull requests:

- **Vulnerability Database:** GitHub Advisory Database
- **License Compliance:** Automatic license conflict detection
- **Risk Assessment:** Dependency age, maintenance status, and security posture
- **Blocking Capability:** Can prevent merges with critical vulnerabilities

---

### Dependency Management

> **Workflow:** `.github/workflows/dependency-check.yml`  
> **Triggers:** Push to `main`/`develop` • Pull requests • Weekly schedule (Monday 00:00 UTC) • Manual dispatch

Proactive dependency health monitoring ensuring build environment stability and compatibility.

**Validation Checks:**

| Component | Verification | Requirement |
|-----------|--------------|-------------|
| **Boost Libraries** | Version compatibility | All components available |
| **GCC Compiler** | Version and feature support | GCC 14 with C++20 |
| **Binutils** | Toolchain compatibility | Version 2.43+ |
| **CMake** | Build system version | 3.28.1 or higher |

**Validation Process:**
1. Package availability verification
2. Version compatibility testing
3. Minimal build smoke test
4. Health status reporting

---

### Documentation Validation

> **Workflow:** `.github/workflows/documentation.yml`  
> **Triggers:** Push to `main` • Pull requests to `main` • Manual dispatch

Comprehensive documentation quality assurance maintaining professional standards and completeness.

#### Structure Validation

**Required Documentation:**
- `README.md` – Project overview and quick start
- `CHANGELOG.md` – Version history and release notes
- `CONTRIBUTING.md` – Contribution guidelines
- `CODE_OF_CONDUCT.md` – Community standards
- `SECURITY.md` – Security policy and reporting
- `doc/API_Reference.md` – API documentation
- `doc/algorithm_documentation.md` – Algorithm specifications

**Quality Checks:**
- Markdown structure validation (heading hierarchy)
- Public API header documentation verification
- Cross-reference consistency

#### Content Validation

**Code Example Verification:**
- C++ code block extraction from documentation
- Syntax and structure validation
- Completeness checks (return statements, includes)
- Best practices conformance

**Accuracy Maintenance:**
- Version number consistency across documents
- Outdated information detection
- Link validity (where applicable)

#### Spell Checking

- **Tool:** GNU Aspell with English dictionary
- **Scope:** All markdown documentation files
- **Mode:** Advisory (non-blocking) with report generation

---

### Code Coverage

> **Workflow:** `.github/workflows/coverage.yml`  
> **Triggers:** Push to `main`/`develop` • Pull requests • Manual dispatch

Quantitative test effectiveness measurement with automated reporting and threshold enforcement.

#### Coverage Instrumentation

**Build Configuration:**
- Debug build with optimization disabled (`-O0`)
- GCC coverage flags: `--coverage -fprofile-arcs -ftest-coverage`
- Full test suite execution with data collection

#### Report Generation

**Multiple Format Support:**

| Format | Tool | Purpose |
|--------|------|---------|
| **XML** | gcovr | Machine-readable, Codecov integration |
| **HTML** | gcovr | Interactive browsing with line highlighting |
| **LCOV** | lcov | Alternative format, historical compatibility |

**Exclusions:**
- System headers (`/usr/*`)
- Test code (`*/test/*`)
- Build artifacts (`*/build/*`)

#### Integration & Reporting

- **Codecov Integration:** Automatic upload with trend tracking
- **PR Comments:** Coverage delta reporting on pull requests
- **Threshold Enforcement:** Warning at <70% coverage
- **Artifact Preservation:** HTML reports available for download

**Coverage Metrics:**
- Line coverage percentage
- Branch coverage analysis
- Function coverage tracking
- File-level granularity

---

## Workflow Coordination

The six workflows operate as an integrated system, with each component serving a specific role in the quality assurance pipeline.

### Execution Model

```
┌─────────────────────────────────────────────────────────────────┐
│  Developer Push / Pull Request                                  │
└────────────┬────────────────────────────────────────────────────┘
             │
             ├──────────► CI Workflow (Build, Test, Quality)
             │
             ├──────────► Security Scan (CodeQL, Dependencies)
             │
             ├──────────► Coverage Analysis (Test Effectiveness)
             │
             └──────────► Documentation Check (Content Quality)
                         
┌─────────────────────────────────────────────────────────────────┐
│  Scheduled Maintenance (Weekly)                                 │
└────────────┬────────────────────────────────────────────────────┘
             │
             ├──────────► Full CI Suite (Sunday 00:00 UTC)
             ├──────────► Dependency Health (Monday 00:00 UTC)
             └──────────► Security Scan (Tuesday 00:00 UTC)

┌─────────────────────────────────────────────────────────────────┐
│  Release Tag (v*.*.*)                                           │
└────────────┬────────────────────────────────────────────────────┘
             │
             └──────────► Release Workflow (Build, Test, Publish)
```

### Workflow Relationships

| Workflow | Role | Dependencies | Blocking |
|----------|------|--------------|----------|
| **CI** | Foundation | None | Yes (required status check) |
| **Security** | Vulnerability detection | None | Configurable |
| **Coverage** | Quality metrics | CI builds | No (informational) |
| **Documentation** | Content validation | None | Recommended |
| **Dependencies** | Environment health | None | No (proactive) |
| **Release** | Distribution | Tag creation | Yes (must pass tests) |

**Parallel Execution:** Independent workflows run concurrently to minimize feedback latency. Security scans, coverage analysis, and documentation checks operate in parallel with the primary CI workflow.

**Failure Isolation:** Individual job failures don't cascade (`fail-fast: false`). This allows all validation dimensions to complete, providing comprehensive feedback even when one aspect fails.

---

## Performance Optimization

Strategic optimizations minimize execution time and resource consumption while maintaining thorough validation coverage.

### Build Caching Strategy

**CMake Build Cache:**
- **Cache Key:** `{OS}-cmake-{BuildType}-{CMakeLists.txt Hash}`
- **Cached Artifacts:** Compiled objects, generated build files, dependency resolution
- **Hit Rate Impact:** 60-80% reduction in clean build time
- **Invalidation:** Automatic on CMakeLists.txt changes

```yaml
# Example cache configuration
cache:
  key: ubuntu-cmake-Release-abc123def
  paths:
    - build/
    - ~/.cache/
```

### Parallel Execution

**Job-Level Parallelism:**
- Matrix builds execute simultaneously (Release + Debug)
- Sanitizer jobs run independently (Address, Undefined, Leak)
- Quality checks operate in parallel with builds

**Build-Level Parallelism:**
- Multi-core compilation: `cmake --build build -j$(nproc)`
- Typical speedup: 3-4x on GitHub Actions runners

### Optimization Metrics

| Workflow | Typical Duration | Peak Duration | Cache Benefit |
|----------|------------------|---------------|---------------|
| CI (cached) | 8-12 minutes | 15 minutes | ~65% reduction |
| CI (uncached) | 20-25 minutes | 30 minutes | Baseline |
| Security | 10-15 minutes | 20 minutes | ~40% reduction |
| Coverage | 12-18 minutes | 22 minutes | ~55% reduction |
| Release | 15-20 minutes | 25 minutes | N/A (clean build) |

---

## Security & Permissions

All workflows adhere to the principle of least privilege, requesting only the minimum permissions required for their specific operations.

### Permission Model

| Workflow | Contents | Security Events | Pull Requests | Purpose |
|----------|----------|-----------------|---------------|---------|
| **CI** | `read` | - | `write` | Test result comments |
| **Security** | `read` | `write` | - | CodeQL result upload |
| **Coverage** | `read` | - | `write` | Coverage comments |
| **Documentation** | `read` | - | - | Read-only validation |
| **Dependencies** | `read` | - | - | Read-only checks |
| **Release** | `write` | - | - | Release creation |

### Secret Management

**Required Secrets:**
- `CODECOV_TOKEN` – Coverage reporting (optional for public repos)
- `GITHUB_TOKEN` – Automatic (provided by GitHub Actions)

**Secret Usage:**
- Secrets never logged or exposed in outputs
- Used only within trusted GitHub Actions from verified publishers
- No secrets transmitted to external services except Codecov (opt-in)

### Supply Chain Security

**Action Version Pinning:**
- All actions pinned to major versions (e.g., `@v4`)
- Regular Dependabot updates for action dependencies
- Verified publisher requirements for external actions

---

## Monitoring & Maintenance

### Visibility & Observability

**Logging Standards:**
- Verbose output enabled for all critical operations
- Structured logging with clear step delineation
- Error context preservation with full stack traces

**Artifact Retention:**
- Test results (JUnit XML): 90 days
- Coverage reports (HTML): 30 days
- Static analysis reports: 30 days
- Build packages: 90 days

### Health Monitoring

**Success Rate Tracking:**
- Workflow run history available via Actions tab
- Status badges in README for at-a-glance status
- Email notifications on workflow failures (configurable)

**Scheduled Validation:**
- Weekly full CI execution (Sunday) catches environmental drift
- Weekly dependency checks (Monday) identify supply chain issues
- Weekly security scans (Tuesday) ensure continuous vulnerability monitoring

### Maintenance Procedures

**Regular Tasks:**
- Review failed workflow runs weekly
- Update action versions monthly (Dependabot assisted)
- Validate cache effectiveness quarterly
- Audit permissions and secrets semi-annually

**Troubleshooting:**
- Manual dispatch available for all workflows
- Verbose logging enabled by default
- Artifact downloads for offline analysis
- Reusable workflow patterns for consistency

---

## Developer Guidelines

### Pre-Commit Requirements

Before pushing code or opening pull requests, ensure local validation:

```bash
# Build and test locally
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
cd build && ctest --output-on-failure

# Check code formatting
clang-format --dry-run --Werror src/**/*.cpp src/**/*.hpp

# Run static analysis
cppcheck --enable=all --suppress=missingIncludeSystem src/
```

### Pull Request Workflow

**Required Status Checks:**
1. ✅ CI workflow must pass (all jobs)
2. ✅ Security scan must complete (vulnerabilities reviewed)
3. ✅ Documentation validation recommended
4. ℹ️ Coverage information provided (not blocking)

**Review Criteria:**
- All automated checks pass
- Code coverage maintained or improved
- No new security vulnerabilities introduced
- Documentation updated for API changes
- CHANGELOG.md updated for user-facing changes

### Commit Message Standards

For automatic release note generation:

```
type(scope): subject

body

footer
```

**Types:** `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`  
**Example:** `feat(matrix): add sparse matrix support`

### Coverage Expectations

**Minimum Thresholds:**
- Overall project coverage: 70%
- New code coverage: 80%
- Critical paths coverage: 95%

**Coverage Reports:**
- Automatically generated on every PR
- Detailed HTML reports available as artifacts
- Trend tracking via Codecov integration

### Release Process

**Creating Releases:**

```bash
# Ensure main branch is clean and tested
git checkout main
git pull origin main

# Create and push version tag
git tag -a v1.2.3 -m "Release v1.2.3: Description"
git push origin v1.2.3

# Workflow automatically creates release
```

**Version Numbering:**
- **Major (X.0.0):** Breaking API changes
- **Minor (0.X.0):** New features, backward compatible
- **Patch (0.0.X):** Bug fixes, backward compatible

---

## Additional Resources

- **Contributing Guide:** [CONTRIBUTING.md](CONTRIBUTING.md)
- **Security Policy:** [SECURITY.md](SECURITY.md)
- **Code of Conduct:** [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md)
- **API Documentation:** [doc/API_Reference.md](doc/API_Reference.md)
- **Algorithm Documentation:** [doc/algorithm_documentation.md](doc/algorithm_documentation.md)

---

## Support

For workflow-related issues:

1. Check the [GitHub Actions logs](../../actions) for detailed error messages
2. Review this documentation for expected behavior
3. Search [existing issues](../../issues) for similar problems
4. Open a new issue with workflow run links and error details

---

<div align="center">

**[Back to Top](#continuous-integration--deployment)**

*Last Updated: 2025-10-15*  
*Maintained by the jmaths development team*

</div>
