# GCC 15 + binutils Compatibility

## Status: RESOLVED ✅

This issue has been resolved by upgrading to binutils 2.43+. All build types (Debug, Release, Sanitizers) now work correctly with GCC 15.

## Summary

GCC 15 requires **binutils 2.43 or later** for full functionality. The CMake build system automatically detects your binutils version and provides guidance.

## Background

GCC 15.2.0 generates assembly code with the `.base64` pseudo-operation, which was introduced in binutils 2.43. Systems with older binutils (2.42 or earlier) will encounter assembler errors during compilation.

## Detection

The jmaths build system automatically detects your binutils version:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

You'll see a message indicating your binutils version and compatibility status.

## Solutions

### Option 1: Ensure binutils 2.43+ is in PATH (Recommended)

If you have binutils 2.43+ installed but not in your PATH:

```bash
# Add to ~/.profile or ~/.bashrc
export PATH="/path/to/newer/binutils/bin:$PATH"

# Reload your profile
source ~/.profile
```

### Option 2: Install/Upgrade binutils

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install binutils
```

**From source:**
```bash
wget https://ftp.gnu.org/gnu/binutils/binutils-2.45.tar.gz
tar xzf binutils-2.45.tar.gz
cd binutils-2.45
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install
```

### Option 3: Use GCC 14

GCC 14 works with older binutils:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-14
```

### Option 4: Use Clang

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++
```

### Option 5: Use Release Builds

Release builds work with any binutils version and GCC 15:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

## Verification

Check your versions:
```bash
g++-15 --version  # Should show 15.x
as --version      # Should show 2.43 or later
```

## Status Matrix

| Build Type | GCC 15 + binutils 2.42 | GCC 15 + binutils 2.43+ | GCC 14 | Clang |
|------------|------------------------|-------------------------|---------|-------|
| Release    | ✅ Works               | ✅ Works                | ✅ Works | ✅ Works |
| Debug      | ❌ Fails               | ✅ Works (RESOLVED)     | ✅ Works | ✅ Works |
| Sanitizers | ❌ Fails               | ✅ Works (RESOLVED)     | ✅ Works | ✅ Works |

## GitHub Actions CI

The CI pipeline uses ubuntu-latest which includes binutils 2.43+, so all build types (Release, Debug, Sanitizers) work correctly in CI.

## References

- [GCC 15 Release Notes](https://gcc.gnu.org/gcc-15/changes.html)
- [binutils 2.43 Release](https://sourceware.org/binutils/)
