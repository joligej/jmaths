# Known Issues

**Version:** 2.0.0
**Last Updated:** January 2025

Known bugs, limitations, and workarounds for the jmaths library.

---

## Critical Issues

### INT_MIN Handling in Z Constructor

**Severity:** HIGH
**Impact:** Causes assertion failure and program abort
**Affected:** `basic_Z(std::integral auto num)` constructor

The library cannot construct Z from INT_MIN values (INT8_MIN, INT16_MIN, INT32_MIN, INT64_MIN). The sign extraction logic attempts to negate INT_MIN, but INT_MIN negated equals INT_MIN due to two's complement overflow.

**Example:**

```cpp
Z value(INT64_MIN);  // FAILS with assertion
```

**Workaround:**

```cpp
// Use string constructor
Z int64_min("-9223372036854775808");

// Works for all INT_MIN types
Z int8_min("-128");
Z int16_min("-32768");
Z int32_min("-2147483648");
```

---

## Platform-Specific Issues

### Parametric Tests on macOS

**Severity:** LOW (Test Framework Issue)
**Impact:** BOOST_DATA_TEST_CASE tests fail with memory alignment errors
**Platform:** macOS with GCC 15

Boost.Test's data-driven testing framework causes memory access violations on some macOS configurations. This is not a library bug—all equivalent non-parametric tests pass correctly.

**Workaround:**

```bash
./jmaths_unit_test --run_test='!parametric_tests'
```

### CLion Boost.Test Runner on macOS

**Severity:** LOW (IDE Integration Issue)
**Impact:** CLion's Boost.Test run configuration fails with "memory access violation at address: 0x0"
**Platform:** macOS with CLion, GCC 15, and Homebrew Boost

CLion's built-in Boost.Test runner is incompatible with GCC-compiled binaries linked against Clang-compiled Boost libraries. This causes test failures when using the "jmaths_unit_test" run configuration, even though the tests pass when run via CTest or directly from terminal.

**Root Cause:** ABI mismatch between GCC (libstdc++) and Clang-compiled Boost (libc++). CLion's Boost.Test integration wrapper triggers incorrect symbol resolution.

**Workaround:**

In CLion, use the "All CTest" run configuration instead of the "jmaths_unit_test" Boost.Test configuration:

1. **Run → Edit Configurations**
2. Delete or ignore the "jmaths_unit_test" configuration
3. Use only "All CTest" which works correctly

Alternatively, run tests directly from terminal:
```bash
cd build
ctest --verbose
# or
./test/jmaths_unit_test
```

**Note:** This issue is specific to CLion's Boost.Test runner. All other test execution methods (terminal, CTest, CMake Application configs) work correctly.

---

## Minor Issues

### Assertions vs Exceptions

**Severity:** LOW
**Impact:** Program aborts on invalid input instead of throwing catchable exceptions

Invalid characters in string parsing trigger assertions rather than exceptions, causing program termination in debug builds.

**Workaround:** Validate input strings before passing to constructors.

---

## Planned Future Enhancements

These items are documented in source code TODOs and represent planned improvements rather than bugs.

### Q Type: Subnormal Float Handling

**Location:** `src/headers/basic_Q_impl.hpp`
**Status:** Incomplete implementation
**Description:** Conversion from floating-point types with subnormal values needs enhanced handling for proper representation as rational numbers.

### Random Number Generation: Signed Type Support

**Location:** `src/headers/rand.hpp`
**Status:** Under consideration
**Description:** Random generation currently supports unsigned types. Evaluation needed for signed type support in `rand_generic` function.

### uint Class: Expanded Functionality

**Location:** `src/headers/uint.hpp`
**Status:** Planned expansion
**Description:** The `uint` class currently provides minimal functionality. Future enhancements may include:
- Arithmetic operations
- Configurable element types beyond `unsigned char`
- Extended bit manipulation operations

These enhancements are planned for future releases and do not impact current library functionality.

---

## Reporting Issues

To report new issues:

1. Verify it's a library bug (not expected behavior)
2. Create a minimal reproducible example
3. Document expected vs actual behavior
4. Open an issue on [GitHub](https://github.com/joligej/jmaths/issues)
