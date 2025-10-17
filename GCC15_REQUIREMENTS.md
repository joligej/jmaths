# GCC 15 Toolchain Requirements

## Overview

GCC 15 requires **binutils 2.43 or later** for full functionality when building jmaths. This document explains the requirement, provides detection methods, and offers solutions if your system has an older version.

## Summary

The jmaths CMake build system automatically detects your binutils version and provides appropriate guidance during configuration. If you have an incompatible version, this guide will help you resolve it.

## Background

GCC 15.2.0 generates assembly code containing the `.base64` pseudo-operation, which was introduced in binutils 2.43. Systems with older versions of binutils (2.42 or earlier) will encounter assembler errors during compilation, particularly in debug builds and when using sanitizers.

### Technical Details

The `.base64` directive allows GCC to embed base64-encoded data directly in assembly output, which is used for certain optimizations and debug information. Without binutils 2.43+, the assembler cannot process these directives, resulting in build failures.

## Automatic Detection

The jmaths build system automatically detects your binutils version during configuration:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

You will see output indicating your binutils version and compatibility status. If an incompatible version is detected, the build system will provide guidance on resolution.

## Resolution Options

### Option 1: Update PATH (Recommended)

If you have binutils 2.43 or later installed but not in your PATH, add it:

```bash
# Add to ~/.bashrc, ~/.zshrc, or ~/.profile
export PATH="/path/to/newer/binutils/bin:$PATH"

# Reload your shell configuration
source ~/.bashrc  # or ~/.zshrc, ~/.profile
```

Verify the change:
```bash
as --version | head -n1
```

### Option 2: Install or Upgrade binutils

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install --upgrade binutils
```

Check your version:
```bash
as --version | head -n1
# Should show 2.43 or later
```

#### Building from Source

If your distribution doesn't provide binutils 2.43+, build from source:

```bash
# Download latest binutils
wget https://ftp.gnu.org/gnu/binutils/binutils-2.45.tar.gz
tar xzf binutils-2.45.tar.gz
cd binutils-2.45

# Configure and build
./configure --prefix=/usr/local
make -j$(nproc)

# Install
sudo make install

# Verify installation
/usr/local/bin/as --version
```

### Option 3: Use GCC 14

GCC 14 does not require binutils 2.43 and works with older versions:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-14
```

### Option 4: Use Clang

Clang does not have this binutils requirement:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++
```

Note: Ensure your Clang version supports C++23.

### Option 5: Use Release Builds Only

If you only need release builds and cannot upgrade binutils, use:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

Release builds work with any binutils version when using GCC 15.

## Verification

After implementing any solution, verify your setup:

```bash
# Check GCC version
g++-15 --version
# Should show 15.x.x

# Check binutils version
as --version | head -n1
# Should show 2.43 or later

# Test the build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-15
cmake --build . -j$(nproc)
cd build && ctest
```

## Compatibility Matrix

| Build Configuration | GCC 15 + binutils 2.42 | GCC 15 + binutils 2.43+ | GCC 14 | Clang 17+ |
|---------------------|------------------------|-------------------------|---------|-----------|
| Release             | ✅ Works               | ✅ Works                | ✅ Works | ✅ Works   |
| Debug               | ❌ Fails               | ✅ Works                | ✅ Works | ✅ Works   |
| Sanitizers          | ❌ Fails               | ✅ Works                | ✅ Works | ✅ Works   |

## CI/CD Integration

The GitHub Actions CI pipeline uses `ubuntu-latest` runners, which include binutils 2.43 or later. Therefore, all build configurations (Release, Debug, Sanitizers) work correctly in the CI environment without additional configuration.

## References

- [GCC 15 Release Notes](https://gcc.gnu.org/gcc-15/changes.html)
- [binutils 2.43 Release Notes](https://sourceware.org/binutils/)
- [GNU binutils Documentation](https://sourceware.org/binutils/docs/)

## Additional Resources

For further assistance:
- Open an issue on [GitHub Issues](https://github.com/joligej/jmaths/issues)
- Consult the [Contributing Guidelines](CONTRIBUTING.md)
- Check [GitHub Discussions](https://github.com/joligej/jmaths/discussions) for community support

---

**Last Updated:** January 2025  
**Affected GCC Versions:** 15.0+  
**Required binutils Version:** 2.43+

