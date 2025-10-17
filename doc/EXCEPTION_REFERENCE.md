# Exception Reference

This document provides a complete reference for all exceptions that can be thrown by the jmaths library, including the conditions under which they are thrown and how to handle them.

## Table of Contents

1. [Exception Hierarchy](#exception-hierarchy)
2. [error::division_by_zero](#errordivision_by_zero)
3. [error::invalid_base](#errorinvalid_base)
4. [std::format_error](#stdformat_error)
5. [Exception Safety Guarantees](#exception-safety-guarantees)
6. [Best Practices](#best-practices)

---

## Exception Hierarchy

All jmaths-specific exceptions inherit from the base `jmaths::error` class, which in turn inherits from `std::exception`. This allows catching all jmaths exceptions with a single handler:

```
std::exception
    └── jmaths::error
            ├── jmaths::error::division_by_zero
            └── jmaths::error::invalid_base
```

Additionally, the library may throw `std::format_error` when using `std::format` with invalid format specifiers.

---

## error::division_by_zero

**Header:** `<error.hpp>` or `<all.hpp>`

**Description:** Thrown when attempting to divide by zero or when an operation would result in a zero denominator.

### Inheritance

```cpp
class error::division_by_zero : public error
```

### Constructors

```cpp
division_by_zero();                              // Default message
explicit division_by_zero(std::string_view message);  // Custom message
```

### Default Message

```
"You tried to divide by zero!"
```

### Static Check Method

```cpp
static constexpr void check(const auto & num, 
                           std::string_view message = default_message);
```

Checks if `num` is zero and throws `division_by_zero` if true. Works with any type that has an `is_zero()` method or is comparable to `0`.

### Functions That May Throw

#### Division and Modulo Operations

| Function | Signature | Condition |
|----------|-----------|-----------|
| `operator/` (N) | `std::pair<N, N> operator/(const N& lhs, const N& rhs)` | `rhs.is_zero()` |
| `operator/` (Z) | `std::pair<Z, Z> operator/(const Z& lhs, const Z& rhs)` | `rhs.is_zero()` |
| `operator/` (Q) | `Q operator/(const Q& lhs, const Q& rhs)` | `rhs.is_zero()` |

**Note:** The division operator for `N` and `Z` returns a pair `{quotient, remainder}`. Both the quotient and remainder operations will throw if the divisor is zero.

#### Rational Number Constructors

| Constructor | Condition |
|-------------|-----------|
| `Q(const N& num, const N& denom)` | `denom.is_zero()` |
| `Q(N&& num, N&& denom)` | `denom.is_zero()` |
| `Q(const Z& num, const Z& denom)` | `denom.is_zero()` |
| `Q(Z&& num, Z&& denom)` | `denom.is_zero()` |
| `Q(std::string_view num_str, unsigned base)` | String contains `/0` (zero denominator) |

#### Rational Number Operations

| Method/Operation | Condition |
|------------------|-----------|
| `Q::inverse()` | `this->is_zero()` (cannot invert zero) |
| `operator&(const Q&, const Q&)` | Result has zero denominator: `(lhs.denom_ & rhs.denom_).is_zero()` |
| `operator^(const Q&, const Q&)` | Result has zero denominator: `(lhs.denom_ ^ rhs.denom_).is_zero()` |

#### Modular Arithmetic

| Function | Signature | Condition |
|----------|-----------|-----------|
| `calc::pow_mod` | `N pow_mod(N&& base, N&& exp, const N& mod)` | `mod.is_zero()` |

### Example Usage

```cpp
#include "all.hpp"
using namespace jmaths;

try {
    N a(100);
    N b(0);
    auto result = a / b;  // Throws
} catch (const error::division_by_zero& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}

// Using the check method
N divisor = get_user_input();
error::division_by_zero::check(divisor, "Divisor cannot be zero!");

// Creating rational with zero denominator
try {
    Q fraction("5/0");  // Throws
} catch (const error::division_by_zero& e) {
    std::cerr << "Invalid fraction: " << e.what() << std::endl;
}

// Inverse of zero
try {
    Q zero(0);
    Q result = zero.inverse();  // Throws
} catch (const error::division_by_zero& e) {
    std::cerr << "Cannot invert zero: " << e.what() << std::endl;
}
```

---

## error::invalid_base

**Header:** `<error.hpp>` or `<all.hpp>`

**Description:** Thrown when an invalid numeric base is specified. Valid bases are in the range [2, 64].

### Inheritance

```cpp
class error::invalid_base : public error
```

### Constructors

```cpp
invalid_base();                              // Default message
explicit invalid_base(std::string_view message);  // Custom message
```

### Default Message

```
"You need to enter a base between 2 and 64!"
```

### Constants

```cpp
static constexpr unsigned minimum_base = 2U;
static constexpr unsigned maximum_base = 64U;
```

### Static Check Method

```cpp
static constexpr void check(unsigned base);
```

Checks if `base` is in the valid range [2, 64] and throws `invalid_base` if not.

### Base Encoding Scheme

- **Base 2-10:** Uses digits `0-9`
- **Base 11-36:** Uses `0-9, A-Z` (case-insensitive)
- **Base 37-62:** Uses `0-9, A-Z, a-z`
- **Base 63-64:** Uses `0-9, A-Z, a-z, +, ~`

### Functions That May Throw

#### Constructors

| Constructor | Signature | Condition |
|-------------|-----------|-----------|
| `N(std::string_view, unsigned base)` | String constructor | `base < 2` or `base > 64` |
| `Z(std::string_view, unsigned base)` | String constructor | `base < 2` or `base > 64` |
| `Q(std::string_view, unsigned base)` | String constructor | `base < 2` or `base > 64` |

#### Conversion Methods

| Method | Signature | Condition |
|--------|-----------|-----------|
| `N::to_str(unsigned base)` | Convert to string | `base < 2` or `base > 64` |
| `Z::to_str(unsigned base)` | Convert to string | `base < 2` or `base > 64` |
| `Q::to_str(unsigned base)` | Convert to string | `base < 2` or `base > 64` |

#### Formatting

| Operation | Condition |
|-----------|-----------|
| `std::format("{:base}", number)` | `base < 2` or `base > 64` |

**Note:** `to_hex()` and `to_bin()` methods use fixed bases (16 and 2) and never throw this exception.

### Example Usage

```cpp
#include "all.hpp"
using namespace jmaths;

try {
    N num("FF", 1);  // Invalid: base must be >= 2
} catch (const error::invalid_base& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}

try {
    N num(255);
    std::string str = num.to_str(100);  // Invalid: base must be <= 64
} catch (const error::invalid_base& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}

// Using the check method
unsigned base = get_user_input();
error::invalid_base::check(base);
N num("12345", base);  // Safe, base was validated

// Valid boundary cases
N binary("1010", 2);      // OK: minimum base
N base64("ZzZ", 64);      // OK: maximum base
std::string hex = N(255).to_str(16);  // OK: base 16 is valid
```

---

## std::format_error

**Header:** `<format>` (standard library)

**Description:** In C++20 and later, `std::format` validates format strings at **compile time**. This means that invalid format specifiers cause **compilation errors** rather than runtime exceptions.

### Compile-Time vs Runtime Validation

The jmaths library provides `std::formatter` specializations for `N`, `Z`, and `Q` types. The format specifier syntax is:

```
{:base}
```

Where `base` is a decimal number between 2 and 64.

**Important:** Because format strings are validated at compile time:

- Invalid format specifiers (non-digit characters) cause **compilation errors**, not `std::format_error` exceptions
- Invalid base values (< 2 or > 64) also cause **compilation errors** via `error::invalid_base::check` in a `constexpr` context

This is a feature of C++20's `std::format` - it catches format errors early, at compilation, rather than deferring them to runtime.

### When It's Thrown

**Compile-Time Validation:** In C++20 and later, format strings are validated at compile time, not runtime. This means:

1. Invalid format specifiers cause **compilation errors**
2. The library never throws `std::format_error` at runtime for jmaths types
3. All format errors are caught during compilation

This is a safety feature of modern C++ - you cannot accidentally ship code with invalid format strings.

### Format Specifier Rules (Compile-Time Checked)

The jmaths formatter accepts an optional base specifier:

- **Valid:** `{}`, `{:10}`, `{:16}`, `{:2}`, `{:64}`
- **Invalid (compilation error):** `{:x}`, `{:abc}`, `{:@}`, `{:1x}`
- **Invalid base (compilation error):** `{:0}`, `{:1}`, `{:65}`, `{:999}`

### Example Usage

```cpp
#include "all.hpp"
#include <format>
using namespace jmaths;

// Valid formatting - compiles successfully
N num(255);
std::cout << std::format("{}", num) << std::endl;      // "255" (base 10)
std::cout << std::format("{:16}", num) << std::endl;   // "FF" (base 16)
std::cout << std::format("{:2}", num) << std::endl;    // "11111111" (base 2)

// Invalid format specifier - COMPILATION ERROR
// std::string str = std::format("{:abc}", num);  // Error: non-digit in format spec

// Invalid base value - COMPILATION ERROR
// std::string str = std::format("{:100}", num);  // Error: base out of range

// These errors are caught at compile time, preventing runtime surprises!
```

---

## Exception Safety Guarantees

The jmaths library provides the following exception safety guarantees:

### Strong Guarantee (Rollback Semantics)

Most operations provide the **strong exception guarantee**: if an exception is thrown, the program state remains unchanged (rollback).

- **Constructors:** If a constructor throws, no object is created, and no resources are leaked.
- **Assignment operators:** If assignment throws, the target object remains in its original state.
- **Arithmetic operations:** Operations like `+`, `-`, `*`, `/` don't modify their operands.

### No-Throw Guarantee

The following operations are guaranteed not to throw exceptions:

- **Destructors:** All destructors are `noexcept`.
- **Move constructors and move assignment:** Moving is `noexcept` (assumes allocator move is `noexcept`).
- **`error::what()`:** Declared as `noexcept override`.
- **Query methods:** `is_zero()`, `is_one()`, `is_even()`, `is_odd()`, etc. are `noexcept`.
- **Comparison operators:** All comparison operations are `noexcept`.

### Basic Guarantee

Some operations provide only the **basic guarantee**: if an exception is thrown, the program remains in a valid state, but the exact state is unspecified.

- **I/O operations:** Stream insertion/extraction may leave streams in a partially read/written state.

### Constexpr and Exception Handling

Many jmaths functions are `constexpr`, allowing compile-time evaluation. However:

- Exceptions thrown during compile-time evaluation result in compilation errors.
- Runtime exceptions are thrown normally when called at runtime.

```cpp
// Compile-time error: division by zero detected at compile time
// constexpr N result = N(10) / N(0);

// Runtime exception
N runtime_divisor(0);
auto result = N(10) / runtime_divisor;  // Throws at runtime
```

---

## Best Practices

### 1. Validate Input Early

Use the `check()` methods to validate inputs before performing operations:

```cpp
unsigned base = get_user_base();
error::invalid_base::check(base);  // Validate early

N num(value_str, base);  // Safe, base is valid
```

### 2. Catch Specific Exceptions

Catch specific exception types when you can handle them meaningfully:

```cpp
try {
    Q result = numerator / denominator;
} catch (const error::division_by_zero& e) {
    // Handle division by zero specifically
    return Q(0);  // Or some default value
} catch (const error& e) {
    // Handle other jmaths errors
    std::cerr << "Math error: " << e.what() << std::endl;
} catch (const std::exception& e) {
    // Handle all other exceptions
    std::cerr << "Unexpected error: " << e.what() << std::endl;
}
```

### 3. Prevent Division by Zero

Check for zero before dividing:

```cpp
N divisor = get_divisor();

if (divisor.is_zero()) {
    // Handle zero case without throwing
    return default_value;
}

auto [quotient, remainder] = dividend / divisor;
```

### 4. Validate Base at Input Time

For user-provided bases, validate as soon as possible:

```cpp
std::cout << "Enter base (2-64): ";
unsigned base;
std::cin >> base;

try {
    error::invalid_base::check(base);
} catch (const error::invalid_base&) {
    std::cerr << "Invalid base. Using default base 10." << std::endl;
    base = 10;
}
```

### 5. Use RAII for Exception Safety

When performing multiple operations that might throw, use RAII principles:

```cpp
class Calculator {
    N accumulator_;
    
public:
    void safe_divide(const N& divisor) {
        // Check before modifying state
        error::division_by_zero::check(divisor);
        
        // Now safe to modify
        accumulator_ = accumulator_ / divisor;
    }
};
```

### 6. Document Exception Behavior

When writing functions that use jmaths types, document which exceptions they may throw:

```cpp
/**
 * @brief Compute harmonic mean
 * @throws error::division_by_zero if sum is zero
 */
Q harmonic_mean(const std::vector<Q>& values) {
    Q sum(0);
    for (const auto& val : values) {
        sum += Q(1) / val;  // May throw if val is zero
    }
    return Q(values.size()) / sum;  // May throw if sum is zero
}
```

### 7. Consider `noexcept` for Performance

Functions that don't throw can be marked `noexcept` for better optimization:

```cpp
bool is_valid_result(const N& value) noexcept {
    // Only uses operations that don't throw
    return !value.is_zero() && value.is_even();
}
```

### 8. Test Exception Cases

Always test that your code properly handles exceptions:

```cpp
BOOST_AUTO_TEST_CASE(test_division_by_zero) {
    N zero(0);
    N nonzero(42);
    
    BOOST_CHECK_THROW(nonzero / zero, error::division_by_zero);
}
```

---

## Summary Table

| Exception | Header | Thrown By | Condition |
|-----------|--------|-----------|-----------|
| `error::division_by_zero` | `<error.hpp>` | Division, modulo, Q constructors, `Q::inverse()`, `calc::pow_mod`, bitwise Q ops | Divisor/denominator/modulus is zero |
| `error::invalid_base` | `<error.hpp>` | String constructors, `to_str()`, `std::format` (compile-time) | Base < 2 or base > 64 |
| `std::format_error` | `<format>` | N/A (C++20+ validates at compile time) | Format specifier invalid - **caught at compile time, not runtime** |

---

## See Also

- [API Reference](API_Reference.md) - Complete API documentation
- [Quick Reference](QUICK_REFERENCE.md) - Quick start guide
- [Test Suite](../test/test_exceptions.cpp) - Comprehensive exception tests
