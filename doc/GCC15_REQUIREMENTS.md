# GCC 15 Toolchain Requirements

## Overview

GCC 15 requires **binutils 2.43 or later** for full functionality. The jmaths CMake build system automatically detects your binutils version and provides guidance if needed.

## Background

GCC 15 generates assembly code with the `.base64` pseudo-operation, introduced in binutils 2.43. Older versions will encounter assembler errors in debug builds and when using sanitizers.

## Verification

Check your binutils version during CMake configuration:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

The build system will report compatibility status and provide guidance if needed.

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

Verify your setup after implementing a solution:

```bash
# Check versions
g++-15 --version
as --version | head -n1

# Test build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-15
cmake --build . -j$(nproc)
ctest
```

## Compatibility Matrix

| Configuration | GCC 15 + binutils 2.42 | GCC 15 + binutils 2.43+ |
|---------------|------------------------|-------------------------|
| Release       | ✅ Works               | ✅ Works                |
| Debug         | ❌ Fails               | ✅ Works                |
| Sanitizers    | ❌ Fails               | ✅ Works                |

## Boost.Test Compatibility

When using GCC 15 with Homebrew's Boost.Test (compiled with Clang), ABI incompatibility may occur. The jmaths test suite includes `test/boost_test_gcc_abi_fix.cpp` which automatically resolves this by providing GCC 15-compiled implementations of required Boost.Test symbols.

## Resources

- [GCC 15 Release Notes](https://gcc.gnu.org/gcc-15/changes.html)
- [GitHub Issues](https://github.com/joligej/jmaths/issues) for support

