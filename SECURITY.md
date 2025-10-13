# Security Policy

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 2.x     | :white_check_mark: |
| 1.x     | :x:                |

## Reporting a Vulnerability

If you discover a security vulnerability in jmaths, please report it by emailing the maintainers or by creating a private security advisory on GitHub.

### What to include

- Description of the vulnerability
- Steps to reproduce
- Potential impact
- Suggested fix (if any)

### Response time

We aim to respond to security reports within 48 hours and provide a fix within 7 days for critical vulnerabilities.

## Security Best Practices

When using jmaths:

1. **Input Validation**: Always validate string inputs before passing them to jmaths constructors
2. **Resource Limits**: Set appropriate limits on number sizes in production environments
3. **Dependencies**: Keep Boost and compiler versions up to date
4. **Build Flags**: Use the recommended compilation flags for security (enabled by default)

## Known Security Considerations

- **Memory Usage**: Arbitrary-precision numbers can consume significant memory. Implement appropriate resource limits in production.
- **Denial of Service**: Extremely large numbers or computationally intensive operations could be used for DoS. Consider timeouts for user-provided computations.
- **String Parsing**: Invalid format strings are validated, but always sanitize user input.
