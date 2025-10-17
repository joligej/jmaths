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

---

## Minor Issues

### Assertions vs Exceptions

**Severity:** LOW  
**Impact:** Program aborts on invalid input instead of throwing catchable exceptions

Invalid characters in string parsing trigger assertions rather than exceptions, causing program termination in debug builds.

**Workaround:** Validate input strings before passing to constructors.

---

## Reporting Issues

To report new issues:

1. Verify it's a library bug (not expected behavior)
2. Create a minimal reproducible example
3. Document expected vs actual behavior
4. Open an issue on [GitHub](https://github.com/joligej/jmaths/issues)
