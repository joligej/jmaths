# Security Policy

## Supported Versions

The following table indicates which versions of jmaths currently receive security updates:

| Version | Support Status     |
| ------- | ------------------ |
| 2.x     | :white_check_mark: Actively supported |
| 1.x     | :x: No longer supported |

## Reporting a Vulnerability

If you discover a security vulnerability in jmaths, we encourage responsible disclosure. Please report it through one of the following methods:

### Reporting Methods

1. **GitHub Security Advisory** (Preferred)
   - Navigate to the [Security tab](https://github.com/joligej/jmaths/security) of the repository
   - Click "Report a vulnerability"
   - Fill out the private security advisory form

2. **Email**
   - Contact the maintainers directly (see commit history for contact information)
   - Use the subject line: "Security Vulnerability Report - jmaths"

### What to Include in Your Report

To help us address the issue quickly and effectively, please include:

- **Vulnerability Description** — Clear explanation of the security issue
- **Affected Components** — Specific parts of the codebase involved
- **Reproduction Steps** — Detailed steps to reproduce the vulnerability
- **Potential Impact** — Assessment of the security implications
- **Proof of Concept** — Example code demonstrating the issue (if applicable)
- **Suggested Fix** — Proposed solution or mitigation (if available)
- **Disclosure Timeline** — Your preferred timeline for public disclosure

### Response Timeline

We are committed to addressing security vulnerabilities promptly:

- **Initial Response** — Within 48 hours of report submission
- **Status Update** — Within 5 business days with assessment and planned action
- **Critical Vulnerabilities** — Fix provided within 7 days
- **Non-Critical Issues** — Fix provided within 30 days
- **Public Disclosure** — Coordinated with reporter after fix is available

### Confidentiality

We request that you:
- Do not publicly disclose the vulnerability until we have issued a fix
- Provide us reasonable time to address the issue before any public disclosure
- Do not exploit the vulnerability beyond what is necessary to demonstrate it

We commit to:
- Acknowledge your responsible disclosure
- Keep you informed throughout the remediation process
- Credit you in the security advisory (unless you prefer to remain anonymous)

## Security Best Practices

When using jmaths in your applications, consider the following security best practices:

### Input Validation

**Always validate and sanitize user inputs** before passing them to jmaths constructors:

```cpp
// Good: Validate input before use
std::string user_input = get_user_input();
if (is_valid_number_string(user_input)) {
    jmaths::N number(user_input);
    // Process number
} else {
    // Handle invalid input
}

// Avoid: Direct use of unchecked user input
jmaths::N number(get_user_input());  // Potentially unsafe
```

### Resource Limits

**Implement appropriate resource limits** to prevent resource exhaustion:

- **Maximum Number Size** — Set limits on the size of numbers in production environments
- **Operation Timeouts** — Implement timeouts for computationally intensive operations
- **Memory Monitoring** — Monitor memory usage for large-scale arithmetic operations

```cpp
// Example: Limit number size
constexpr size_t MAX_DIGITS = 10000;
if (number.digit_count() > MAX_DIGITS) {
    throw std::runtime_error("Number exceeds maximum allowed size");
}
```

### Dependency Management

**Keep dependencies current and secure**:

- **Compiler Updates** — Use the latest stable compiler version with security patches
- **Boost Library** — Keep Boost updated to receive security fixes
- **System Libraries** — Ensure binutils and other system components are up to date

### Build Configuration

**Use recommended compilation flags** for enhanced security:

The default build configuration enables important security features:
- Stack protection
- Position-independent code
- Format string checks
- Runtime bounds checking (in debug builds)

### Denial of Service Prevention

**Be aware of computational complexity**:

- **Very Large Numbers** — Operations on extremely large numbers can be computationally expensive
- **Complex Operations** — Exponentiation, GCD, and factorization can be time-intensive
- **User-Provided Data** — Limit the size and complexity of user-provided computations

```cpp
// Example: Implement timeout for expensive operations
auto future = std::async(std::launch::async, [&]() {
    return calc::pow(base, exponent);
});

if (future.wait_for(std::chrono::seconds(5)) == std::future_status::timeout) {
    throw std::runtime_error("Operation timed out");
}
```

## Known Security Considerations

### Memory Usage

**Arbitrary-precision numbers can consume significant memory.** Each digit requires storage, and very large numbers may use substantial RAM. In memory-constrained environments or when processing untrusted input, implement appropriate limits.

### Computational Complexity

**Some operations have non-linear complexity:**
- **Multiplication** — O(n²) for schoolbook algorithm, O(n^1.58) for Karatsuba
- **Division** — O(n²) for multi-precision division
- **GCD** — O(n²) worst-case for large numbers
- **Exponentiation** — O(log n) multiplications, but each multiplication is expensive

### String Parsing

**String parsing is validated but should be used carefully:**
- Invalid format strings throw exceptions (safe handling required)
- Very long strings may cause performance issues
- Always sanitize user input before parsing

```cpp
try {
    jmaths::N number(potentially_malicious_string);
} catch (const std::exception& e) {
    // Handle parsing error safely
    log_error("Invalid number format", e.what());
}
```

## Security Advisories

Published security advisories for jmaths are available on the [GitHub Security Advisories page](https://github.com/joligej/jmaths/security/advisories). We recommend monitoring this page for updates.

## Security Scanning

This project uses automated security scanning:
- **CodeQL Analysis** — Continuous static analysis for vulnerability detection
- **Dependency Scanning** — Automated monitoring of dependencies via Dependabot
- **Secret Scanning** — Prevention of accidental credential commits

---

For any security-related questions or concerns, please refer to our reporting guidelines above or contact the maintainers directly.

