# jmaths API Reference

**Version:** 2.0.0
**License:** GNU General Public License v3.0
**Author:** Jasper de Smaele
**Repository:** [github.com/joligej/jmaths](https://github.com/joligej/jmaths)

This comprehensive API reference documents all header files in the jmaths library. Every class, function, type alias, constant, and utility is thoroughly documented with practical examples and usage guidelines.

**📚 Exception Handling:** For complete information about exceptions, conditions, and error handling, see [EXCEPTION_REFERENCE.md](EXCEPTION_REFERENCE.md).

---

## Table of Contents

1. [Introduction](#introduction)
2. [Quick Start](#quick-start)
3. [Core Types](#core-types)
   - [basic_N - Unsigned Integers](#n---arbitrary-precision-unsigned-integers)
   - [basic_Z - Signed Integers](#z---arbitrary-precision-signed-integers)
   - [basic_Q - Rational Numbers](#q---arbitrary-precision-rational-numbers)
   - [uint<V> - Fixed-Size Integers](#uintv---fixed-size-unsigned-integers)
   - [sign_type - Sign Management](#sign_type---sign-management-base-class)
4. [Mathematical Functions](#mathematical-functions)
5. [Utilities](#utilities)
   - [Error Handling](#error-handling)
   - [Random Number Generation](#random-number-generation)
   - [User-Defined Literals](#user-defined-literals)
   - [Formatting Support](#formatting-support)
   - [Hashing Support](#hashing-support)
6. [Template Metaprogramming](#template-metaprogramming)
7. [Configuration](#configuration)
8. [Performance Utilities](#performance-utilities)
9. [Advanced Examples](#advanced-examples)
10. [Complete Header Reference](#complete-header-reference)

---

## Introduction

**jmaths** is a C++23 library providing arbitrary-precision arithmetic for integers and rational numbers. It supports compile-time evaluation (constexpr), modern C++ features, and integrates seamlessly with the C++ standard library.

### Key Features

- **Arbitrary Precision** — Numbers limited only by available memory
- **Constexpr Support** — Compile-time evaluation for most operations
- **Modern C++23** — Leverages concepts, ranges, and three-way comparison
- **Standard Library Integration** — Works with `std::format`, `std::unordered_map`, streams, and more
- **Performance Optimized** — Efficient algorithms with optional benchmarking capabilities
- **Type Safety** — Strong type system preventing accidental misuse

### Installation

Include the main header in your project:

```cpp
#include <jmaths/all.hpp>
```

Alternatively, include specific headers for faster compilation:

```cpp
#include <jmaths/basic_N.hpp>  // Unsigned integers only
#include <jmaths/basic_Z.hpp>  // Signed integers
#include <jmaths/basic_Q.hpp>  // Rational numbers
```

---

## Quick Start

### Basic Usage Examples

```cpp
#include <jmaths/all.hpp>
#include <iostream>

using namespace jmaths;

int main() {
    // Create large unsigned integers
    N a = "123456789012345678901234567890"_N;
    N b = 999999_N;
    N c = a + b;

    std::cout << "Sum: " << c << std::endl;

    // Signed integers
    Z negative = -12345_Z;
    Z positive = 67890_Z;
    Z result = negative * positive;

    std::cout << "Product: " << result << std::endl;

    // Rational numbers (fractions)
    Q half{1, 2};        // 1/2
    Q third{1, 3};       // 1/3
    Q sum = half + third; // 5/6 (automatically reduced)

    std::cout << "Fraction sum: " << sum << std::endl;

    // Mathematical functions
    N square = calc::pow(123_N, 2_N);  // 123^2
    auto [sqrt_val, remainder] = calc::sqrt(10000_N);

    std::cout << "Square root: " << sqrt_val << std::endl;

    return 0;
}
```

---

## Core Types

### N - Arbitrary-Precision Unsigned Integers

**Header:** `<jmaths/basic_N.hpp>`
**Type:** `jmaths::N` (alias for `basic_N<uint32_t, uint64_t>`)

Arbitrary-precision unsigned integers stored as vectors of digits in little-endian order.

#### Class Definition

```cpp
template <typename BaseInt, typename BaseIntBig, typename Allocator = std::allocator<BaseInt>>
class basic_N;

// Common typedef
using N = basic_N<std::uint32_t, std::uint64_t>;
```

#### Constructors

| Constructor | Description | Example |
|-------------|-------------|---------|
| `N()` | Default constructor (value = 0) | `N a;` |
| `N(unsigned value)` | From unsigned integer | `N b = 42;` |
| `N(std::string_view str, unsigned base = 10)` | From string with base | `N c{"FF", 16};` |
| User-defined literal | Convenient syntax | `N d = "12345"_N;` |

**Example:**
```cpp
N a;                           // 0
N b = 42;                      // 42
N c = "1234567890"_N;         // Large number
N d{"FF", 16};                // 255 from hex
N e{"101010", 2};             // 42 from binary
```

##### String Constructor Details

The string constructor `N(std::string_view str, unsigned base = 10)` converts a string representation to an arbitrary-precision number.

**Important Behaviors:**

1. **No Automatic Whitespace Trimming** *(By Design)*
   - Input strings are parsed character-by-character without preprocessing
   - Leading/trailing whitespace will cause errors or unexpected behavior
   - This is intentional for performance and clarity
   - **Correct:** `N{"123"}` → 123
   - **Incorrect:** `N{" 123 "}` → Error (space is not a valid digit)
   - **Workaround:** Trim strings before passing to constructors

2. **No Sign Prefix Support for N Type** *(By Design)*
   - The `N` type represents unsigned integers only
   - Characters `+` and `-` are NOT interpreted as sign prefixes
   - In base-64 encoding, `+` represents digit value 62, not a positive sign
   - For signed integers, use the `Z` type instead
   - **Correct:** `N{"456"}` → 456
   - **Incorrect:** `N{"+456"}` → Error (`+` is digit 62, invalid for base-10)
   - **Note:** Even the `Z` type only supports `-` prefix, not `+` prefix
   - **Use Z instead:** `Z{"456"}` → 456 (positive), `Z{"-456"}` → -456 (negative)

3. **Base-64 Encoding Character Set**
   The character-to-digit mapping supports bases 2 through 64:
   - **Base 2-10:** `'0'-'9'` represent values 0-9
   - **Base 11-36:** `'A'-'Z'` represent values 10-35 (case-insensitive in base ≤36)
   - **Base 37-62:** `'a'-'z'` represent values 36-61
   - **Base 63:** `'+'` represents value 62
   - **Base 64:** `'~'` represents value 63

   ```cpp
   N dec{"255"};              // Base-10: 255
   N hex{"FF", 16};           // Base-16: 255
   N b36{"ZZ", 36};           // Base-36: 35*36 + 35 = 1295
   N b64{"~~", 64};           // Base-64: 63*64 + 63 = 4095
   N b64_plus{"+~", 64};      // Base-64: 62*64 + 63 = 4031
   ```

4. **Empty and Zero Strings**
   - Empty strings and strings containing only `'0'` characters represent zero
   - **Examples:** `N{""}` → 0, `N{"0"}` → 0, `N{"000"}` → 0

5. **Invalid Characters**
   - Characters outside the valid range for the specified base will cause errors
   - **Base-10 valid:** `'0'-'9'` only
   - **Base-16 valid:** `'0'-'9'`, `'A'-'F'`, `'a'-'f'`
   - Using `'G'` in base-16 or `'A'` in base-10 causes an error

**Best Practices:**
```cpp
// ✓ Correct usage
N a{"12345"};                  // Clean decimal string
N b{"DEADBEEF", 16};          // Hex without prefix
N c{"0"};                     // Zero

// ✗ Avoid these
N d{" 123"};                  // Has whitespace - ERROR
N e{"+123"};                  // Has sign prefix - ERROR (use Z type)
N f{"0x123"};                 // Has prefix - ERROR (remove "0x")
N g{"12.34"};                 // Has decimal point - ERROR (use Q type)
```

#### Arithmetic Operations

```cpp
N a = 100_N, b = 50_N;

// Addition
N sum = a + b;               // 150
a += b;                      // a becomes 150

// Subtraction (throws if result would be negative)
N diff = a - b;              // 50
a -= b;                      // a becomes 50

// Multiplication
N product = a * b;           // 5000
a *= b;                      // a becomes 5000

// Division (returns pair: quotient, remainder)
auto [quotient, remainder] = a / b;

// Modulo
N mod = (a / b).second;

// Exponentiation (via calc)
N power = calc::pow(2_N, 100_N);  // 2^100
```

#### Bitwise Operations

```cpp
N a = 0b1100_N;
N b = 0b1010_N;

N and_result = a & b;    // 0b1000 (8)
N or_result = a | b;     // 0b1110 (14)
N xor_result = a ^ b;    // 0b0110 (6)
N not_result = ~a;       // Bitwise NOT

// Bit shifts
N left = a << 2;         // Multiply by 4
N right = a >> 1;        // Divide by 2
```

##### Bitwise NOT (~) for Arbitrary-Precision Integers

The bitwise NOT operator behaves differently for arbitrary-precision integers compared to fixed-width types due to the absence of a predetermined bit width.

**Key Behavior** *(By Design)*:

- **For non-zero values:** `~N` inverts all bits in the internal representation
- **For zero:** `~N(0)` returns `N(0)` (special case, not all bits set like fixed-width types)

**Why This Design:**

In fixed-width types (like `uint32_t`), `~0` produces all bits set (e.g., `0xFFFFFFFF`). However, arbitrary-precision integers don't have a fixed maximum bit width:

```cpp
// Fixed-width (uint32_t)
uint32_t x = 0;           // 32 bits: 0x00000000
uint32_t y = ~x;          // 32 bits: 0xFFFFFFFF (all ones)

// Arbitrary-precision (N)
N x = 0;                  // No bits stored (empty = zero)
N y = ~x;                 // No bits to invert → still zero
```

**Correct Usage:**

```cpp
// Inverting non-zero values works as expected
N a = 0b1111_N;           // Binary: 1111
N b = ~a;                 // Inverts bits: 11111111111...11110000 (in internal representation)
                          // After removing leading zeros: result depends on internal digit size

// Zero is a special case
N zero = 0_N;
N inverted = ~zero;       // Returns 0 (no bits to invert)
ASSERT(inverted == 0);    // TRUE

// Practical bitwise NOT usage
N mask = 0xFF_N;          // 8 bits set
N data = 0b10101010_N;
N result = data & mask;   // Extract lowest 8 bits

// Double NOT returns to original (for non-zero)
N original = 42_N;
N restored = ~~original;
ASSERT(restored == original);  // TRUE
```

**Implementation Note:**

The library inverts all bits in the internal digit representation and then removes leading zeros. For zero (no digits), there are no bits to invert, resulting in zero.

```cpp
// Conceptual implementation
N operator~() const {
    if (digits.empty()) return N(0);  // Zero case

    N result;
    for (auto digit : digits) {
        result.digits.push_back(~digit);  // Invert each digit
    }
    result.remove_leading_zeros();        // Clean up
    return result;
}
```

**When to Use Bitwise NOT:**

- Bit masking operations
- Implementing bitwise algorithms
- Low-level data manipulation

**When NOT to Use:**

- Don't rely on `~0` to generate "all ones" (use explicit masks instead)
- For arithmetic negation, use the `Z` type (signed integers), not bitwise NOT

#### Member Functions

| Function | Returns | Description |
|----------|---------|-------------|
| `to_str(unsigned base = 10)` | `std::string` | Convert to string in given base |
| `to_hex()` | `std::string` | Convert to hex string |
| `to_bin()` | `std::string` | Convert to binary string |
| `is_zero()` | `bool` | Check if zero |
| `is_one()` | `bool` | Check if one |
| `is_even()` | `bool` | Check if even |
| `is_odd()` | `bool` | Check if odd |
| `bits()` | `bitcount_t` | Number of significant bits |
| `ctz()` | `bitcount_t` | Count trailing zeros |
| `size()` | `std::size_t` | Size in bytes |
| `fits_into<T>()` | `std::optional<T>` | Safe conversion to built-in type |
| `operator[](bitpos_t)` | bit reference | Access individual bit |
| `set_zero()` | `void` | Set value to zero |

##### Type Conversion with fits_into<T>()

The `fits_into<T>()` member function provides safe, checked conversion from arbitrary-precision integers to built-in integer types. It returns `std::optional<T>` which contains the value if it fits, or `std::nullopt` if the value is too large.

**Signature:**
```cpp
template<std::integral T>
constexpr std::optional<T> fits_into() const;
```

**Behavior:**

- **Returns value** if the number can be represented exactly in type `T`
- **Returns std::nullopt** if the number is too large for type `T`
- **Works with signed and unsigned** integer types
- **Performs range checking** to ensure no data loss

**Examples:**

```cpp
// Successful conversions
N small = 42_N;
auto val32 = small.fits_into<uint32_t>();
if (val32) {
    uint32_t n = *val32;  // 42
}

N zero = 0_N;
auto val8 = zero.fits_into<uint8_t>();
ASSERT(val8.has_value());
ASSERT(*val8 == 0);

// Range boundaries
N max_u8 = 255_N;
ASSERT(max_u8.fits_into<uint8_t>().has_value());  // Fits exactly

N too_large_u8 = 256_N;
ASSERT(!too_large_u8.fits_into<uint8_t>().has_value());  // Too large

N max_u16 = 65535_N;
ASSERT(max_u16.fits_into<uint16_t>().has_value());  // Fits exactly

N too_large_u16 = 65536_N;
ASSERT(!too_large_u16.fits_into<uint16_t>().has_value());  // Too large

// Large numbers
N huge = "999999999999999999999"_N;
ASSERT(!huge.fits_into<uint64_t>().has_value());  // Too large for any standard type

// Signed types
N in_signed_range = 100_N;
auto signed_val = in_signed_range.fits_into<int32_t>();
ASSERT(signed_val.has_value());
ASSERT(*signed_val == 100);

// Safe usage pattern with std::optional
N value = get_some_value();
if (auto result = value.fits_into<uint64_t>(); result.has_value()) {
    uint64_t native = *result;
    // Use native type for performance
} else {
    // Keep using arbitrary-precision
}
```

**Type Support:**

| Type | Max Value | Example |
|------|-----------|---------|
| `uint8_t` | 255 | `N{255}.fits_into<uint8_t>()` ✓ |
| `uint16_t` | 65,535 | `N{65535}.fits_into<uint16_t>()` ✓ |
| `uint32_t` | 4,294,967,295 | `N{4294967295}.fits_into<uint32_t>()` ✓ |
| `uint64_t` | 18,446,744,073,709,551,615 | `N{"18446744073709551615"}.fits_into<uint64_t>()` ✓ |
| `int8_t` | 127 | `N{127}.fits_into<int8_t>()` ✓ |
| `int32_t` | 2,147,483,647 | `N{2147483647}.fits_into<int32_t>()` ✓ |

**Why Use fits_into():**

1. **Safety:** No silent overflow or data loss
2. **Clarity:** Explicit intent to convert
3. **Performance:** Optimization opportunity when value fits in native types
4. **Constexpr:** Can be used in compile-time contexts

**Implementation Note:**
The function correctly handles all range checks and produces accurate results for all supported integer types. The implementation has been thoroughly tested and validated.

**Common Patterns:**

```cpp
// Pattern 1: Extract or error
N value = parse_input();
auto result = value.fits_into<uint32_t>();
if (!result) {
    throw std::overflow_error("Value too large");
}
uint32_t native = *result;

// Pattern 2: Fallback to arbitrary-precision
template<typename Func>
void process(const N& value, Func&& f) {
    if (auto small = value.fits_into<uint64_t>()) {
        f(*small);  // Fast path with native type
    } else {
        f(value);   // Slow path with arbitrary-precision
    }
}

// Pattern 3: Validation
bool is_valid_port(const N& num) {
    auto port = num.fits_into<uint16_t>();
    return port.has_value() && *port >= 1024;
}
```

**Example:**
```cpp
N num = 12345_N;

std::string decimal = num.to_str();       // "12345"
std::string hex = num.to_hex();           // "3039"
std::string binary = num.to_bin();        // "11000000111001"

std::size_t bytes = num.size();           // Size in bytes
bitcount_t bit_count = num.bits();        // Number of significant bits

bool zero = num.is_zero();                // false
bool odd = num.is_odd();                  // true

// Bit access
bool bit_0 = num[0];                      // Access bit 0 (LSB)
num[5] = true;                            // Set bit 5

// Set to zero
num.set_zero();                           // num becomes 0
```

#### Bit Access with Proxy Classes

The `operator[]` returns proxy objects that allow safe bit manipulation:

**bit_reference** - Mutable bit access for non-const objects:
```cpp
N num = 0b1010_N;
num[0] = true;           // Set bit 0: num becomes 0b1011
num[2] = false;          // Clear bit 2: num becomes 0b1011
bool val = num[3];       // Read bit 3: true
```

**const_bit_reference** - Read-only bit access for const objects:
```cpp
const N num = 0b1010_N;
bool val = num[1];       // Read bit 1: true
// num[1] = false;       // ERROR: cannot modify const object
```

**Design Note:** These proxy classes enable `num[i] = value` syntax while maintaining
internal number representation. They convert to `bool` for reading and accept `bool`
assignment for writing.

#### Post-increment and Post-decrement

In addition to pre-increment/decrement, post-increment/decrement operators are supported:

```cpp
N num = 5_N;

N a = num++;    // a = 5, num = 6 (post-increment)
N b = ++num;    // b = 7, num = 7 (pre-increment)

N c = num--;    // c = 7, num = 6 (post-decrement)
N d = --num;    // d = 5, num = 5 (pre-decrement)
```

**Performance Note:** Pre-increment/decrement is more efficient as it doesn't
require creating a temporary copy of the original value.

#### Stream I/O Operators

All jmaths types support stream input/output for easy console and file I/O:

```cpp
#include <iostream>
#include <sstream>

N num = 12345_N;

// Output to stream (uses default base 10)
std::cout << num << std::endl;         // Prints: 12345

// Input from stream
N input_num;
std::cin >> input_num;                 // Reads number from input

// String stream operations
std::stringstream ss;
ss << num;                             // Write to string stream
std::string str = ss.str();            // "12345"

ss >> input_num;                       // Read from string stream
```

**Operators:**
- `operator<<(std::ostream&, const N&)` - Output N to stream
- `operator>>(std::istream&, N&)` - Input N from stream
- Similarly for Z and Q types

#### Internal Implementation: detail Struct

**Advanced Usage:** For library developers or those needing low-level access, each type has a
nested `detail` struct containing static implementation functions:

**basic_N::detail** - Internal operations for unsigned integers:
```cpp
struct basic_N::detail {
    // I/O operations
    static constexpr std::ostream& opr_ins(std::ostream& os, const basic_N& n);
    static constexpr std::istream& opr_extr(std::istream& is, basic_N& n);

    // Arithmetic operations (return new values)
    static constexpr basic_N opr_add(const basic_N& lhs, const basic_N& rhs);
    static constexpr basic_N opr_subtr(basic_N lhs, const basic_N& rhs);
    static constexpr basic_N opr_mult(const basic_N& lhs, const basic_N& rhs);
    static constexpr std::pair<basic_N, basic_N> opr_div(const basic_N& lhs, const basic_N& rhs);

    // Bitwise operations
    static constexpr basic_N opr_and(const basic_N& lhs, const basic_N& rhs);
    static constexpr basic_N opr_or(const basic_N& lhs, const basic_N& rhs);
    static constexpr basic_N opr_xor(const basic_N& lhs, const basic_N& rhs);

    // Comparison operations
    static constexpr bool opr_eq(const basic_N& lhs, const basic_N& rhs);
    static constexpr bool opr_eq(const basic_N& lhs, std::integral auto rhs);
    static constexpr std::strong_ordering opr_comp(const basic_N& lhs, const basic_N& rhs);
    static constexpr std::strong_ordering opr_comp(const basic_N& lhs, std::integral auto rhs);
};
```

**Design Note:** These static functions are used internally by the operator overloads
and compound assignment operators. They're exposed in the `detail` struct for:
- Testing and verification purposes
- Advanced optimizations where direct function calls avoid temporary object creation
- Understanding the implementation for contributors

**Similar structures exist for:**
- `basic_Z::detail` - Signed integer operations with sign handling
- `basic_Q::detail` - Rational number operations with automatic reduction

---

### Z - Arbitrary-Precision Signed Integers

**Header:** `<jmaths/basic_Z.hpp>`
**Type:** `jmaths::Z` (alias for `basic_Z<uint32_t, uint64_t>`)

Sign-magnitude representation: inherits from `basic_N` for magnitude operations and adds sign handling.

#### Class Definition

```cpp
template <typename BaseInt, typename BaseIntBig, typename Allocator = std::allocator<BaseInt>>
class basic_Z : private basic_N<BaseInt, BaseIntBig, Allocator>,
                public sign_type;

// Common typedef
using Z = basic_Z<std::uint32_t, std::uint64_t>;
```

#### Constructors

| Constructor | Description | Example |
|-------------|-------------|---------|
| `Z()` | Default constructor (value = 0) | `Z a;` |
| `Z(int value)` | From signed integer | `Z b = -42;` |
| `Z(std::string_view str, unsigned base = 10)` | From string | `Z c{"-FF", 16};` |
| `explicit Z(const N& n)` | From unsigned integer | `Z d{some_N};` |
| User-defined literal | Convenient syntax | `Z e = "-12345"_Z;` |

**Example:**
```cpp
Z a;                           // 0
Z b = -42;                     // -42
Z c = 1000;                    // 1000
Z d = "-123456789"_Z;         // Large negative number
Z e{"-FF", 16};               // -255 from hex
```

**String Constructor Behavior** *(By Design)*
The Z string constructor only supports `-` prefix for negative numbers. The `+` prefix is not supported for positive numbers (just omit the sign for positive values). Like N, whitespace is not automatically trimmed.

**INT_MIN Limitation** *(Known Issue)*
The library cannot construct Z from INT_MIN values (the minimum value of signed integer types) due to two's complement representation limitations. When attempting to create `Z(INT64_MIN)`, the program will crash with an assertion failure.

**Workaround for INT_MIN values:**
```cpp
// ✗ Does NOT work - will crash
// Z bad(INT64_MIN);
// Z bad(INT32_MIN);
// Z bad(INT16_MIN);
// Z bad(INT8_MIN);

// ✓ WORKAROUND: Use string constructor instead
Z min64("-9223372036854775808");        // INT64_MIN
Z min32("-2147483648");                 // INT32_MIN
Z min16("-32768");                      // INT16_MIN
Z min8("-128");                         // INT8_MIN

// ✓ Or use this formula for INT64_MIN:
Z min64_alt = -Z(static_cast<uint64_t>(-(INT64_MIN + 1)) + 1);
```

**Why this happens:** In two's complement representation, `|INT_MIN|` is one larger than `INT_MAX`, so the absolute value cannot fit in the same signed type. The negation wraps back to INT_MIN, causing the magnitude extraction to fail.

#### Sign Operations

```cpp
Z num = -42_Z;

// Check sign
bool is_pos = num.is_positive();      // false
bool is_neg = num.is_negative();      // true
bool is_zero = num.is_zero();         // false

// Get absolute value
N magnitude = num.abs();              // 42 (returns N type)

// Flip sign
num.flip_sign();                      // num becomes 42

// Negation
Z negative = -num;                    // Flip sign
```

**Zero Sign Behavior** *(By Design)*
Zero is treated as positive in this library. `Z(0).is_positive()` returns `true` and `Z(0).is_negative()` returns `false`. This is a design choice that simplifies implementation and matches the mathematical convention that zero is non-negative (0 ≥ 0).

#### Arithmetic Operations

```cpp
Z a = 100_Z, b = -50_Z;

// Addition (handles sign automatically)
Z sum = a + b;               // 50
a += b;                      // a becomes 50

// Subtraction
Z diff = a - b;              // 150

// Multiplication (sign follows rules: neg × neg = pos)
Z product = a * b;           // -5000

// Division (truncated toward zero, C++ standard)
auto [quotient, remainder] = a / b;

// Negation
Z negative = -a;             // Flip sign
```

---

### Q - Arbitrary-Precision Rational Numbers

**Header:** `<jmaths/basic_Q.hpp>`
**Type:** `jmaths::Q` (alias for `basic_Q<uint32_t, uint64_t>`)

Represents fractions as numerator/denominator pairs, always in reduced form.

#### Class Definition

```cpp
template <typename BaseInt, typename BaseIntBig, typename Allocator = std::allocator<BaseInt>>
class basic_Q : public sign_type;

// Common typedef
using Q = basic_Q<std::uint32_t, std::uint64_t>;
```

#### Constructors

| Constructor | Description | Example |
|-------------|-------------|---------|
| `Q()` | Default (0/1) | `Q a;` |
| `Q(int num)` | Integer as fraction | `Q b = 5;` |
| `Q(int num, int denom)` | From numerator and denominator | `Q c{3, 4};` |
| `Q(double value)` | From floating-point | `Q d{0.125};` |
| `Q(std::string_view str)` | From "num/denom" string | `Q e{"22/7"};` |

**Example:**
```cpp
Q zero;                              // 0/1
Q half{1, 2};                       // 1/2
Q neg_third{-1, 3};                 // -1/3
Q from_int = 5_Q;                   // 5/1
Q from_float{0.125};                // 1/8 (exact)
Q from_string{"22/7"};              // 22/7 (approx π)
Q reduced{10, 15};                  // Automatically becomes 2/3
```

**String Constructor Requirements** *(By Design)*
The Q string constructor requires explicit "numerator/denominator" format with a `/` separator. Integer-only strings like `"5"` are not supported and will cause a division by zero error (empty denominator = zero denominator). This design ensures clear, unambiguous format.

```cpp
Q good{"5/1"};        // ✓ Correct - explicit fraction format
Q good{5};            // ✓ Correct - use integer constructor for integers
Q bad{"5"};           // ✗ Error - empty denominator causes division_by_zero
```

#### Arithmetic Operations

Fractions are automatically reduced after operations:

```cpp
Q a{1, 2};      // 1/2
Q b{1, 3};      // 1/3

// Addition: (a/b) + (c/d) = (ad + bc) / (bd), then reduce
Q sum = a + b;                       // 5/6

// Subtraction
Q diff = a - b;                      // 1/6

// Multiplication: (a/b) × (c/d) = (ac) / (bd)
Q product = a * b;                   // 1/6

// Division: (a/b) ÷ (c/d) = (a/b) × (d/c)
Q quotient = a / b;                  // 3/2

// Reciprocal (multiplicative inverse)
Q reciprocal = a.inverse();          // If a = 3/4, returns 4/3
```

#### Member Functions

| Function | Returns | Description |
|----------|---------|-------------|
| `is_zero()` | `bool` | Check if zero |
| `is_one()` | `bool` | Check if one |
| `is_neg_one()` | `bool` | Check if negative one |
| `is_positive()` | `bool` | Check if positive |
| `is_negative()` | `bool` | Check if negative |
| `abs()` | `Q` | Absolute value |
| `inverse()` | `Q` | Reciprocal (1/this) |
| `to_str()` | `std::string` | Convert to "num/denom" string |
| `fits_into<T>()` | `std::optional<T>` | Convert to floating-point |
| `size()` | `std::size_t` | Size in bytes |

---

### uint<V> - Fixed-Size Unsigned Integers

**Header:** `<jmaths/uint.hpp>`
**Type:** `jmaths::uint<V>` where V is a power of 2

Fixed-width unsigned integers of arbitrary bit size (e.g., uint<256>, uint<512>, uint<1024>).

#### Usage

```cpp
// Fixed-size types (V must be power of 2)
uint<64> u64;           // 64-bit unsigned (like uint64_t)
uint<128> u128;         // 128-bit unsigned
uint<256> u256;         // 256-bit unsigned (e.g., for SHA-256)
uint<512> u512;         // 512-bit unsigned (e.g., for SHA-512)
uint<1024> u1024;       // 1024-bit unsigned (e.g., for RSA)

// Constructors
uint<256> a;            // Default: 0
uint<256> b{42};        // From integer
uint<256> c{0xDEADBEEF};
```

**Use Cases:**
- Cryptographic hash digests (uint<256> for SHA-256)
- Public key cryptography (uint<2048> for RSA keys)
- Fixed-precision arithmetic where exact bit width is critical

---

## Mathematical Functions

### calc - Calculation Functions

**Header:** `<jmaths/calc.hpp>`
**Namespace:** `jmaths::calc`

Collection of mathematical functions for arbitrary-precision numbers.

| Function | Signature | Description | Complexity |
|----------|-----------|-------------|------------|
| `gcd` | `N gcd(N a, N b)` | Greatest common divisor | O(n log n) |
| `sqrt` | `pair<N,N> sqrt(N num)` | Integer square root + remainder | O(log n × n²) |
| `sqrt_whole` | `N sqrt_whole(N num)` | Integer square root (floor) | O(log n × n²) |
| `pow` | `N pow(N base, N exp)` | Exponentiation | O(log exp) |
| `pow` | `Z pow(Z base, N exp)` | Signed exponentiation | O(log exp) |
| `pow_mod` | `N pow_mod(N base, N exp, N mod)` | Modular exponentiation | O(log exp) |

#### Examples

**Greatest Common Divisor:**
```cpp
N a = 48_N;
N b = 18_N;
N g = calc::gcd(a, b);        // 6

// Common use: reducing fractions
N num = 48_N, denom = 18_N;
N divisor = calc::gcd(num, denom);
num /= divisor;                // 8
denom /= divisor;              // 3
```

**Integer Square Root:**
```cpp
N num = 100_N;
auto [sqrt_val, remainder] = calc::sqrt(num);
// sqrt_val = 10, remainder = 0

N num2 = 101_N;
auto [sqrt_val2, remainder2] = calc::sqrt(num2);
// sqrt_val2 = 10, remainder2 = 1
```

**Exponentiation:**
```cpp
N base = 2_N;
N exp = 100_N;
N result = calc::pow(base, exp);     // 2^100
// result = 1267650600228229401496703205376
```

**Modular Exponentiation (for cryptography):**
```cpp
// RSA encryption example (simplified)
N message = 42_N;
N public_exp = 65537_N;
N modulus = "very_large_number"_N;

N encrypted = calc::pow_mod(message, public_exp, modulus);
```

**Signed Power:**
```cpp
Z base = -2_Z;
N exp = 3_N;
Z result = calc::pow(base, exp);     // -8 (negative because odd exponent)

Z base2 = -2_Z;
N exp2 = 4_N;
Z result2 = calc::pow(base2, exp2);  // 16 (positive because even exponent)
```

---

## Utilities

### Error Handling

**Header:** `<jmaths/error.hpp>`

**See Also:** [Complete Exception Reference](EXCEPTION_REFERENCE.md) for detailed information about all exceptions.

The jmaths library uses exceptions to signal error conditions. All jmaths exceptions inherit from `jmaths::error`, which inherits from `std::exception`.

#### Exception Hierarchy

```
std::exception
    └── jmaths::error
            ├── jmaths::error::division_by_zero
            └── jmaths::error::invalid_base
```

#### error - Base Exception Class

```cpp
class error : public std::exception {
public:
    static constexpr char default_message[] = "No error message provided!";

    error();
    explicit error(std::string_view message);
    const char* what() const noexcept override;
};
```

Base class for all jmaths-specific exceptions. Can be caught to handle all jmaths errors.

#### error::division_by_zero

**Thrown by:**
- Division operators: `N / N`, `Z / Z`, `Q / Q`
- Rational constructors: `Q(numerator, denominator)` when denominator is zero
- `Q::inverse()` when called on zero
- `calc::pow_mod(base, exp, mod)` when mod is zero
- Bitwise operations on `Q`: `operator&`, `operator^` when result has zero denominator

```cpp
try {
    N a = 10_N;
    N b = 0_N;
    auto [q, r] = a / b;  // Throws division_by_zero
} catch (const jmaths::error::division_by_zero& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}

// Manual checking before operation
N divisor = get_divisor();
jmaths::error::division_by_zero::check(divisor);  // Throws if zero

// Creating rationals safely
try {
    Q fraction("5/0");  // Throws: zero denominator
} catch (const jmaths::error::division_by_zero& e) {
    std::cerr << "Invalid fraction: " << e.what() << std::endl;
}

// Inverse of zero
Q zero(0);
// Q inv = zero.inverse();  // Would throw
```

**Check Method:**

```cpp
static constexpr void check(const auto& num,
                           std::string_view message = default_message);
```

Checks if `num` is zero and throws if true. Works with any type having `is_zero()` or comparable to 0.

#### error::invalid_base

**Thrown by:**
- String constructors: `N(str, base)`, `Z(str, base)`, `Q(str, base)` when base < 2 or base > 64
- Conversion methods: `to_str(base)` when base out of range
- `std::format` with invalid base specifier

**Valid Range:** 2 ≤ base ≤ 64

**Constants:**
```cpp
static constexpr unsigned minimum_base = 2U;
static constexpr unsigned maximum_base = 64U;
```

**Base Encoding:**
- Base 2-10: Digits 0-9
- Base 11-36: 0-9, A-Z (case-insensitive)
- Base 37-62: 0-9, A-Z, a-z
- Base 63-64: 0-9, A-Z, a-z, +, ~

```cpp
// Valid bases
N binary{"1010", 2};        // Base 2 (minimum)
N octal{"755", 8};          // Base 8
N decimal{"42", 10};        // Base 10 (default)
N hex{"DEADBEEF", 16};      // Base 16
N base36{"ZZZ", 36};        // Base 36 (0-9, A-Z)
N base64{"aBc+~", 64};      // Base 64 (maximum)

// Invalid bases throw
try {
    N invalid{"123", 1};     // Too small
    N invalid2{"123", 65};   // Too large
} catch (const jmaths::error::invalid_base& e) {
    std::cerr << "Invalid base: " << e.what() << std::endl;
}

// Also for output
N num(255);
try {
    std::string str = num.to_str(100);  // Throws
} catch (const jmaths::error::invalid_base& e) {
    std::cerr << e.what() << std::endl;
}
```

**Check Method:**

```cpp
static constexpr void check(unsigned base);
```

Validates base is in [2, 64] range and throws if not.

#### std::format_error

While not a jmaths exception, `std::format_error` (from `<format>`) can be thrown when using format specifiers with jmaths types:

```cpp
N num(42);

// Valid format
std::cout << std::format("{}", num) << std::endl;      // "42"
std::cout << std::format("{:16}", num) << std::endl;   // "2a" (hex)

// Invalid: non-digit in format spec
try {
    std::format("{:abc}", num);  // Throws std::format_error
} catch (const std::format_error& e) {
    std::cerr << "Format error: " << e.what() << std::endl;
}

// Invalid: base out of range
try {
    std::format("{:100}", num);  // Throws error::invalid_base
} catch (const jmaths::error::invalid_base& e) {
    std::cerr << "Invalid base: " << e.what() << std::endl;
}
```

**See [EXCEPTION_REFERENCE.md](EXCEPTION_REFERENCE.md) for complete exception documentation.**

---

### Random Number Generation

**Header:** `<jmaths/rand.hpp>`

High-quality random number generation using Mersenne Twister (MT19937).

#### rand<N> - Random Unsigned Integers

```cpp
// Generate random number with at most 256 bits
N random_256bit = jmaths::rand<N>::generate(256);

// Generate random cryptographic key
constexpr bitcount_t KEY_SIZE = 2048;
N rsa_key = jmaths::rand<N>::generate(KEY_SIZE);
```

#### rand<Z> - Random Signed Integers

```cpp
// Generate random signed integer with at most 128-bit magnitude
Z random_signed = jmaths::rand<Z>::generate(128);

// Can be positive or negative with equal probability
```

---

### User-Defined Literals

**Header:** `<jmaths/literals.hpp>`
**Namespace:** `jmaths::literals` (inline namespace)

Convenient syntax for creating large numbers.

```cpp
using namespace jmaths::literals;

// Unsigned integers
N large = 123456789012345678901234567890_N;
N calculation = 1000000_N * 2000000_N + 500_N;

// Signed integers
Z positive = 123456_Z;
Z negative = -987654_Z;
Z result = 1000_Z - 2000_Z;  // -1000

// Rational numbers
Q integer_as_fraction = 42_Q;    // 42/1
Q half{1, 2};                    // 1/2 (use constructor for fractions)
Q result = half + Q{1, 3};       // 5/6
```

---

### Formatting Support

**Header:** `<jmaths/formatter.hpp>`

Integration with C++20 `std::format` for formatting numbers in various bases.

#### Format Syntax

```
{:<base>}
```

Where `<base>` is a number from 2 to 64. Default base is 10.

#### Examples

```cpp
#include <format>
#include <jmaths/all.hpp>

N num = 255_N;

// Different bases
std::string dec = std::format("{}", num);      // "255" (default base 10)
std::string bin = std::format("{:2}", num);    // "11111111"
std::string oct = std::format("{:8}", num);    // "377"
std::string hex = std::format("{:16}", num);   // "FF"
std::string b36 = std::format("{:36}", num);   // "73"

// Works with all jmaths types
Z signed_num = -42_Z;
std::string s1 = std::format("{}", signed_num);     // "-42"
std::string s2 = std::format("{:16}", signed_num);  // "-2A"

Q frac{22, 7};
std::string s3 = std::format("{}", frac);           // "22/7"

// Use in formatted strings
std::string message = std::format("The number {} in binary is {:2}", num, num);
```

---

### Hashing Support

**Header:** `<jmaths/hash.hpp>`

Specializations of `std::hash` to use jmaths types as keys in unordered containers.

```cpp
#include <unordered_map>
#include <unordered_set>

// Unordered set of large numbers
std::unordered_set<N> unique_numbers;
unique_numbers.insert(123456789_N);

// Map with N keys
std::unordered_map<N, std::string> number_names;
number_names[1_N] = "one";
number_names[1000_N] = "thousand";
number_names[1000000_N] = "million";

// Map with Z keys (signed)
std::unordered_map<Z, int> signed_map;
signed_map[-10_Z] = -1;
signed_map[0_Z] = 0;
signed_map[10_Z] = 1;

// Map with Q keys (rational)
std::unordered_map<Q, std::string> fraction_names;
fraction_names[Q{1, 2}] = "half";
fraction_names[Q{1, 3}] = "third";
fraction_names[Q{1, 4}] = "quarter";
```

---

## Template Metaprogramming

**Header:** `<jmaths/TMP.hpp>`
**Namespace:** `jmaths::TMP`

Template metaprogramming utilities for compile-time type manipulation and constraints.

### Key Concepts

#### primitive_type

```cpp
template <typename T>
concept primitive_type;

static_assert(primitive_type<int>);
static_assert(primitive_type<double>);
static_assert(!primitive_type<std::string>);
```

#### any_of

```cpp
template <typename T, typename... U>
concept any_of;

static_assert(any_of<int, float, double, int>);  // true
static_assert(!any_of<int, float, double>);      // false
```

#### is_power_of_2

```cpp
template <auto n>
concept is_power_of_2;

static_assert(is_power_of_2<64>);   // true
static_assert(is_power_of_2<256>);  // true
static_assert(!is_power_of_2<100>); // false
```

#### decays_to

```cpp
template <typename T, typename U>
concept decays_to;

static_assert(decays_to<const int&, int>);  // true
static_assert(decays_to<int*, int>);        // false
```

#### instance_of

```cpp
template <typename T, template <typename...> class... U>
concept instance_of;

static_assert(instance_of<std::vector<int>, std::vector>);  // true
static_assert(instance_of<N, basic_N>);                      // true
static_assert(!instance_of<int, std::vector>);              // false
```

---

## Configuration

**Header:** `<jmaths/constants_and_types.hpp>`

Core constants and type definitions.

### Constants

```cpp
inline constexpr char negative_sign = '-';     // Sign for negative numbers
inline constexpr char vinculum = '/';           // Separator for fractions
inline constexpr unsigned default_base = 10;   // Default string conversion base
inline constexpr bitcount_t bits_in_byte = 8;  // Bits per byte (CHAR_BIT)
```

### Type Definitions

```cpp
using bitpos_t = unsigned long long;    // Bit position (index)
using bitcount_t = unsigned long long;  // Bit count
using bitdiff_t = long long;            // Signed bit difference

template <typename T>
using allocator = std::allocator<T>;    // Default allocator
```

### Version Information

```cpp
namespace jmaths::internal {
    struct metadata {
        static constexpr char product_name[] = "jmaths";

        struct version {
            enum : unsigned {
                major = 1,
                minor = 0,
                patch = 0
            };
        };
    };
}

unsigned major_ver = jmaths::internal::metadata::version::major;  // 1
```

---

## Performance Utilities

### Benchmarking Support

**Header:** `<jmaths/scoped_timer.hpp>`

RAII-based timer for performance profiling. Enabled with `JMATHS_BENCHMARKING` flag.

```bash
g++-15 -DJMATHS_BENCHMARKING -std=c++23 program.cpp -o program
```

**Output Format:**
```
log:call    function_name
log:time    123.456    ms    from    function_name
```

**Configuring Output:**

```cpp
#include <fstream>
#include <jmaths/scoped_timer.hpp>

// Log to file
std::ofstream log_file("benchmark.log");
jmaths::internal::scoped_timer::set_ostream(&log_file);

// Disable logging
jmaths::internal::scoped_timer::set_ostream(nullptr);
```

### Complexity Reference

| Operation | N (unsigned) | Z (signed) | Q (rational) |
|-----------|--------------|------------|--------------|
| Addition | O(n) | O(n) | O(n²) |
| Subtraction | O(n) | O(n) | O(n²) |
| Multiplication | O(n²) | O(n²) | O(n²) |
| Division | O(n×m) | O(n×m) | O(n²) |
| Comparison | O(1) best, O(n) worst | O(1) best, O(n) worst | O(n) |
| GCD | - | - | O(n log n) |
| Power | O(log exp × n²) | O(log exp × n²) | O(log exp × n²) |
| Square Root | O(log n × n²) | - | - |

Where n = number of digits, m = number of digits in divisor, exp = exponent value.

---

## Advanced Examples

### Example 1: Factorial Calculator

```cpp
#include <jmaths/all.hpp>
#include <iostream>

using namespace jmaths;

N factorial(unsigned n) {
    N result = 1_N;
    for (unsigned i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    N fact100 = factorial(100);
    std::cout << "100! = " << fact100 << std::endl;
    std::cout << "It has " << fact100.bits() << " bits" << std::endl;
    return 0;
}
```

### Example 2: Fibonacci Sequence

```cpp
#include <jmaths/all.hpp>
#include <iostream>

using namespace jmaths;

N fibonacci(unsigned n) {
    if (n <= 1) return N{n};

    N prev = 0_N;
    N curr = 1_N;

    for (unsigned i = 2; i <= n; ++i) {
        N next = prev + curr;
        prev = curr;
        curr = next;
    }

    return curr;
}

int main() {
    N fib1000 = fibonacci(1000);
    std::cout << "F(1000) = " << fib1000 << std::endl;
    return 0;
}
```

### Example 3: Rational Arithmetic

```cpp
#include <jmaths/all.hpp>
#include <iostream>

using namespace jmaths;

int main() {
    // Calculate sum: 1/1 + 1/2 + 1/3 + ... + 1/100
    Q sum;

    for (int i = 1; i <= 100; ++i) {
        sum += Q{1, i};
    }

    std::cout << "Sum of 1/1 + 1/2 + ... + 1/100 = " << sum << std::endl;

    // Approximate as decimal
    if (auto decimal = sum.fits_into<double>()) {
        std::cout << "Approximately: " << *decimal << std::endl;
    }

    return 0;
}
```

### Example 4: Base Conversions

```cpp
#include <jmaths/all.hpp>
#include <iostream>
#include <format>

using namespace jmaths;

int main() {
    N num{"DEADBEEF", 16};  // Create from hex

    std::cout << "Number created from hex DEADBEEF:" << std::endl;
    std::cout << "Decimal: " << num.to_str(10) << std::endl;
    std::cout << "Binary:  " << num.to_bin() << std::endl;
    std::cout << "Octal:   " << num.to_str(8) << std::endl;
    std::cout << "Hex:     " << num.to_hex() << std::endl;

    // Using std::format
    std::cout << "\nUsing std::format:" << std::endl;
    std::cout << std::format("Dec: {}", num) << std::endl;
    std::cout << std::format("Bin: {:2}", num) << std::endl;
    std::cout << std::format("Hex: {:16}", num) << std::endl;

    return 0;
}
```

---

## Building and Linking

### CMake Integration

```cmake
# Find jmaths package
find_package(jmaths REQUIRED)

# Add executable
add_executable(myprogram main.cpp)

# Link with jmaths
target_link_libraries(myprogram PRIVATE jmaths::jmaths)

# Set C++20 standard
set_property(TARGET myprogram PROPERTY CXX_STANDARD 20)
```

### Manual Compilation

Since jmaths is a header-only library, no linking is required. After installation, headers are located in `/usr/local/include/jmaths/` (or your CMAKE_INSTALL_PREFIX):

```bash
# If installed system-wide (to /usr/local/)
g++-15 -std=c++23 program.cpp -o program

# With optimization
g++-15 -std=c++23 -O3 program.cpp -o program

# With benchmarking
g++-15 -std=c++23 -DJMATHS_BENCHMARKING program.cpp -o program

# If installed to a custom location, specify include path
g++-15 -std=c++23 -I/path/to/install/include program.cpp -o program
```

### CMake Integration

For CMake projects, use `find_package` to automatically configure include paths:

```cmake
find_package(jmaths REQUIRED)
target_link_libraries(your_target jmaths)
```

Note: Despite the name `target_link_libraries`, this only adds the include directories for header-only libraries - no actual linking occurs.

---

## API Summary Tables

### Type Aliases

| Alias | Full Type | Description |
|-------|-----------|-------------|
| `N` | `basic_N<uint32_t, uint64_t>` | Arbitrary-precision unsigned integer |
| `Z` | `basic_Z<uint32_t, uint64_t>` | Arbitrary-precision signed integer |
| `Q` | `basic_Q<uint32_t, uint64_t>` | Arbitrary-precision rational number |

### Common Operations

| Operation | N | Z | Q | Description |
|-----------|---|---|---|-------------|
| `+`, `+=` | ✓ | ✓ | ✓ | Addition |
| `-`, `-=` | ✓ | ✓ | ✓ | Subtraction |
| `*`, `*=` | ✓ | ✓ | ✓ | Multiplication |
| `/`, `/=` | ✓ | ✓ | ✓ | Division |
| `%` | ✓ | ✓ | ✗ | Modulo |
| `&`, `\|`, `^` | ✓ | ✓ | ✓ | Bitwise operations |
| `<<`, `>>` | ✓ | ✓ | ✓ | Bit shifts |
| `~` | ✓ | ✓ | ✓ | Bitwise NOT |
| `-` (unary) | ✗ | ✓ | ✓ | Negation |
| `++`, `--` | ✓ | ✓ | ✓ | Increment/Decrement |
| `==`, `!=`, `<`, etc. | ✓ | ✓ | ✓ | Comparisons |
| `<=>` | ✓ | ✓ | ✓ | Three-way comparison |

---

## Appendix: Supported Bases

The library supports string conversion for bases 2 through 64:

| Base Range | Character Set | Example |
|------------|---------------|---------|
| 2-10 | `0-9` | Base 10: `"12345"` |
| 11-36 | `0-9`, `A-Z` | Base 16: `"DEADBEEF"` |
| 37-62 | `0-9`, `A-Z`, `a-z` | Base 36: `"ZzZ"` |
| 63-64 | `0-9`, `A-Z`, `a-z`, `+`, `~` | Base 64: `"aB9+~"` |

---

## License

jmaths is licensed under the GNU General Public License v3.0 (GPLv3).

```
Copyright (C) 2025 Jasper de Smaele

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
```

---

## Additional Resources

- **Source Code:** Check the `src/headers/` directory for implementation details
- **Samples:** See the `samples/` directory for example programs
- **Algorithm Documentation:** See `algorithm_documentation.md` for detailed algorithm descriptions
- **Tests:** See the `test/` directory for comprehensive test cases

---

## Complete Header Reference

This section provides a comprehensive reference for all 30 header files in the jmaths library.

### File Organization Summary

The library consists of:
- **9 core type headers**: basic_N, basic_Z, basic_Q with implementations
- **2 mathematical function headers**: calc with implementation
- **10 utility headers**: error, hash, literals, rand, formatter, scoped_timer with implementations
- **9 supporting headers**: TMP, all, declarations, constants_and_types, def, sign_type, uint with implementations

### Header 1-2: TMP.hpp / all.hpp

**TMP.hpp** - Template Metaprogramming Utilities
- **Purpose**: Compile-time type manipulation, concepts, and metafunctions
- **Key Components**:
  - `pack_container<Types...>` - Stores type lists for compile-time manipulation
  - `primitive_type<T>` concept - Checks if T is a primitive type (int, float, etc.)
  - `any_of<T, U...>` concept - Checks if T matches any of U...
  - `is_power_of_2<n>` concept - Validates power-of-2 values at compile-time
  - `decays_to<T, U>` concept - Checks if T decays to U
  - `instance_of<T, Template...>` concept - Checks if T is instance of Template
  - `matches_which<T, Op, U...>` - Finds first type in U... matching predicate Op with T
  - `same_template<T, U>` concept - Checks if T and U are same template instantiation
  - Type size comparison utilities: `equal_size`, `greater_size`, `lesser_size`, `geq_size`, `leq_size`

**all.hpp** - Convenience Header
- **Purpose**: Single include for entire library
- **Includes**: TMP, basic_N, basic_Z, basic_Q, calc, error, hash, literals, rand, uint, constants_and_types, declarations
- **Usage**: `#include <jmaths/all.hpp>` for complete library access

### Headers 3-5: basic_N (Unsigned Integers)

**basic_N.hpp** - Class Definition
- **Class**: `template <typename BaseInt, typename BaseIntBig, typename Allocator> class basic_N`
- **Typedef**: `using N = basic_N<std::uint32_t, std::uint64_t>`
- **Storage**: Little-endian vector of digits (LSB first)
- **Invariant**: No leading zeros (canonical form)
- **Key Members**:
  - Constructors: default, from integral, from string with base
  - `is_zero()`, `is_one()`, `is_even()`, `is_odd()`
  - `bits()` - count of significant bits
  - `ctz()` - count trailing zeros
  - `to_str(base)`, `to_hex()`, `to_bin()`
  - `fits_into<T>()` - safe conversion to built-in types
  - `operator[]` - bit access via proxy
  - All arithmetic: `+`, `-`, `*`, `/` (returns pair)
  - All bitwise: `&`, `|`, `^`, `~`, `<<`, `>>`
  - Comparison: `==`, `<=>` (three-way)

**basic_N_detail_impl.hpp** - Core Operations
- **opr_add**: Schoolbook addition with carry propagation - O(n)
- **opr_subtr**: Schoolbook subtraction with borrow - O(n)
- **opr_mult**: Schoolbook multiplication - O(n²), or Karatsuba O(n^1.585) if enabled
- **opr_div**: Binary long division (restoring) - O(n×m)
- **opr_and/or/xor**: Digit-wise bitwise operations - O(n)
- **opr_comp**: Three-way comparison - O(1) best, O(n) worst
- **opr_eq**: Equality check via digit vector comparison

**basic_N_impl.hpp** - Additional Members
- String/integer conversion helpers
- Bit manipulation internals
- Template member function implementations
- Type conversion functions

### Headers 6-8: basic_Z (Signed Integers)

**basic_Z.hpp** - Class Definition
- **Class**: `template <typename BaseInt, typename BaseIntBig, typename Allocator> class basic_Z : private basic_N, public sign_type`
- **Typedef**: `using Z = basic_Z<std::uint32_t, std::uint64_t>`
- **Representation**: Sign-magnitude (sign stored separately)
- **Key Members**:
  - Inherits: `ctz()`, `bits()`, `is_even()`, `is_odd()`, `operator[]` from basic_N
  - `is_positive()`, `is_negative()`, `is_zero()` from sign_type
  - `abs()` - returns unsigned magnitude (N type)
  - `operator-()` - unary negation
  - All arithmetic with sign handling
  - All bitwise operations

**basic_Z_detail_impl.hpp** - Core Operations
- **opr_add**: Four-case signed addition (++, +-, -+, --)
- **opr_subtr**: Four-case signed subtraction
- **opr_mult**: Magnitude multiplication with XOR sign rule
- **opr_div**: C++ standard division (truncated toward zero), remainder has dividend sign
- **opr_and/or/xor**: Bitwise with sign handling

**basic_Z_impl.hpp** - Additional Members
- Construction from unsigned/signed values
- String parsing with sign extraction
- Increment/decrement with sign crossing
- Type conversions

### Headers 9-11: basic_Q (Rational Numbers)

**basic_Q.hpp** - Class Definition
- **Class**: `template <typename BaseInt, typename BaseIntBig, typename Allocator> class basic_Q : public sign_type`
- **Typedef**: `using Q = basic_Q<std::uint32_t, std::uint64_t>`
- **Representation**: numerator (N), denominator (N), sign (separate)
- **Invariant**: Always in reduced form (gcd(num, denom) = 1), denominator always positive
- **Key Members**:
  - Constructors: from N, from Z, from N pair, from Z pair, from float/double, from string "num/denom"
  - `is_zero()`, `is_one()`, `is_neg_one()`
  - `abs()` - absolute value
  - `inverse()` - reciprocal (1/this)
  - `fits_into<float/double>()` - convert to floating-point
  - All arithmetic with automatic reduction
  - Bitwise operations on numerator/denominator

**basic_Q_detail_impl.hpp** - Core Operations
- **opr_add**: Cross-multiplication (a/b + c/d = (ad+bc)/(bd)), then reduce
- **opr_subtr**: Cross-multiplication subtraction
- **opr_mult**: Direct multiplication (a/b × c/d = ac/bd), then reduce
- **opr_div**: Invert and multiply (a/b ÷ c/d = ad/bc)
- **opr_comp**: Cross-multiplication comparison (avoids division)

**basic_Q_impl.hpp** - Additional Members
- **handle_float_**: IEEE 754 decomposition using `std::frexp`/`std::scalbn`
- **canonicalise_**: Reduce to lowest terms using GCD
- Floating-point conversion with overflow/underflow checking
- Compound assignment operators

### Headers 12-13: calc (Mathematical Functions)

**calc.hpp** - Function Declarations
- **struct calc** - Static function collection
- **gcd(N, N)**: Binary GCD (Stein's algorithm) - O(n log n)
- **sqrt(N)**: Integer square root + remainder - O(log n × n²)
- **sqrt_whole(N)**: Integer square root only - O(log n × n²)
- **pow(N, N)**: Exponentiation by squaring - O(log exp × n²)
- **pow(Z, N)**: Signed power with sign rules
- **pow_mod(N, N, N)**: Modular exponentiation - O(log exp × n²), crucial for cryptography

**calc_impl.hpp** - Function Implementations
- **gcd**: Uses binary GCD algorithm (bit shifts, no division)
  1. Extract common power of 2
  2. Make both odd
  3. Repeatedly subtract and remove factors of 2
  4. Restore common power of 2
- **sqrt**: Binary search in range [1, num/2]
- **pow**: Process exponent bit-by-bit, square base each iteration
- **pow_mod**: Like pow but applies modulo after each multiplication to keep values bounded

### Headers 14-15: declarations.hpp / constants_and_types.hpp.in

**declarations.hpp** - Forward Declarations
- Forward declares: calc, basic_N, sign_type, basic_Z, basic_Q, error, rand, uint
- **Purpose**: Break circular dependencies between headers
- **Usage**: Include when you only need pointers/references without full definitions

**constants_and_types.hpp.in** - Core Definitions (CMake template)
- **Constants**:
  - `negative_sign = '-'`
  - `vinculum = '/'` (fraction separator)
  - `default_base = 10`
  - `bits_in_byte = CHAR_BIT` (typically 8)
  - `max_ratio = 0.000125` (internal configuration)
- **Types**:
  - `bitpos_t = unsigned long long`
  - `bitcount_t = unsigned long long`
  - `bitdiff_t = long long`
  - `allocator<T> = std::allocator<T>`
- **Metadata**:
  - `jmaths::internal::metadata::product_name`
  - `jmaths::internal::metadata::version::{major, minor, patch}`

### Header 16: def.hh - Macros

**def.hh** - Macro Definitions
- **JMATHS_FUNCTION_TO_LOG**:
  - When `JMATHS_BENCHMARKING=1`: Creates scoped_timer to log function calls and execution time
  - When `JMATHS_BENCHMARKING=0`: Expands to `((void)0)` (no overhead)
  - Uses `std::source_location::current()` to capture function name
- **JMATHS_REPEAT(x)**:
  - Loop macro for cleaner syntax
  - Expands to: `for (counter = 0; counter < x; ++counter)`
  - Creates uniquely-named counter using `__LINE__` to avoid conflicts
  - Example: `JMATHS_REPEAT(10) { /* runs 10 times */ }`
- **JMATHS_CONCAT2(x, y)**: Token concatenation helper

### Headers 17-18: error (Exception Handling)

**error.hpp** - Exception Classes
- **class error : public std::exception**:
  - Base exception class for all jmaths errors
  - `what()` returns stored message
- **class error::division_by_zero : public error**:
  - Thrown when dividing by zero
  - `static void check(auto& num)` - throws if num is zero
  - Works with any type having `is_zero()` method or comparable to 0
- **class error::invalid_base : public error**:
  - Thrown when base < 2 or base > 64
  - `static void check(unsigned base)` - validates base range
  - `minimum_base = 2`, `maximum_base = 64`

**error_impl.hpp** - Implementations
- Template check functions use SFINAE to detect `is_zero()` method
- Throws with descriptive messages including invalid values

### Headers 19-20: formatter.hpp

**formatter.hpp** - std::format Integration
- **struct format_parser**:
  - Parses format string to extract base: `{:16}` means base 16
  - Default base is 10
  - Validates base using `error::invalid_base::check()`
- **struct format_output<T> : format_parser**:
  - Provides `std::formatter` specialization for N, Z, Q
  - Routes to optimized conversions:
    - Base 2: `to_bin()` - optimized binary
    - Base 16: `to_hex()` - optimized hexadecimal
    - Other: `conv_to_base_(base)` - general algorithm
- **Usage**: `std::format("{:2}", num)` for binary, `std::format("{:16}", num)` for hex

### Headers 21-22: hash (Unordered Container Support)

**hash.hpp** - std::hash Specializations
- **std::hash<basic_N>**: Hash arbitrary-precision unsigned integers
- **std::hash<basic_Z>**: Hash signed integers with sign incorporated
- **std::hash<basic_Q>**: Hash rational numbers (numerator, denominator, sign)

**hash_impl.hpp** - Hash Implementations
- **N hash**: Treats digit vector as byte sequence, uses `std::hash<std::string_view>`
  - Special case: empty (zero) gets cached hash
  - O(n) time, no copies
- **Z hash**: `hash(magnitude) XOR (sign << (first_digit % bits_in_size_t))`
  - Ensures +x and -x have different hashes
- **Q hash**: `(hash(num) XOR hash(denom)) XOR (sign << ((num_first XOR denom_first) % bits))`
  - Consistent for reduced fractions

### Headers 23-24: literals (User-Defined Literals)

**literals.hpp** - Literal Declarations
- **operator""_N(const char*)**: Creates N from string
- **operator""_Z(const char*)**: Creates Z from string (handles '-' prefix)
- **operator""_Q(const char*)**: Creates Q from string (numerator only, denom=1)
- **Namespace**: `jmaths::literals` (inline namespace)

**literals_impl.hpp** - Literal Implementations
- Conditionally defined based on typedefs:
  - `_N` requires `JMATHS_TYPEDEF_N`
  - `_Z` requires `JMATHS_TYPEDEF_Z`
  - `_Q` requires `JMATHS_TYPEDEF_Q` and `JMATHS_TYPEDEF_N`
- Simply forward to type constructors

### Headers 25-26: rand (Random Number Generation)

**rand.hpp** - Random Generator Declarations
- **internal::rand_gen<T>**:
  - Wraps `std::mt19937` (Mersenne Twister) with `std::uniform_int_distribution`
  - Optionally reseeds every 2 months if `JMATHS_PERIODICALLY_RESEED_RAND=1`
  - Static generator and seed time shared across instances
- **rand<basic_N>**:
  - `static N generate(bitcount_t bits)` - generates number with at most `bits` bits
  - Uniform distribution over [0, 2^bits - 1]
- **rand<basic_Z>**:
  - `static Z generate(bitcount_t bits)` - generates magnitude then random sign
  - 50% positive, 50% negative (zero always positive)

**rand_impl.hpp** - Random Generator Implementations
- **N generation**:
  1. Calculate full digits needed
  2. Generate random value for each digit
  3. Generate partial digit for remaining bits
  4. Remove leading zeros
- **Z generation**: Generate N magnitude, then randomly choose sign

### Header 27: scoped_timer.hpp - Performance Profiling

**scoped_timer.hpp** - RAII Timer
- **class internal::scoped_timer**:
  - **Constructor**: Records start time, logs "log:call\t<function_name>"
  - **Destructor**: Calculates elapsed time, logs "log:time\t<duration_ms>\tms\tfrom\t<function_name>"
  - Uses `std::chrono::high_resolution_clock`
  - **set_ostream(std::ostream*)**: Configure output (default: `std::clog`, `nullptr` disables)
- **Usage**: Automatically instantiated by `JMATHS_FUNCTION_TO_LOG` macro when `JMATHS_BENCHMARKING=1`

### Headers 28-29: sign_type (Sign Management)

**sign_type.hpp** - Sign Base Class
- **class sign_type**:
  - Base class for basic_Z and basic_Q
  - **enum sign_bool : bool { positive = 0, negative = 1 }**
  - `is_positive()`, `is_negative()`
  - `virtual bool is_zero() const = 0` - must be implemented by derived
  - `flip_sign()` - toggle sign (does nothing if zero)
- **Protected**:
  - Constructors: default, from sign_bool, from string (extracts '-'), from integral (extracts sign)
  - `static handle_string_(std::string_view*)` - removes '-' prefix, returns sign
  - `static handle_int_(std::integral auto*)` - makes value positive, returns original sign

**sign_type_impl.hpp** - Sign Implementation
- String handling checks for leading '-' and removes it
- Integer handling uses `std::unsigned_integral` concept for compile-time dispatch
- For signed integers: if negative, multiply by -1 (making positive), return negative sign

### Headers 30: uint (Fixed-Size Integers)

**uint.hpp** - Fixed-Size Integer Template
- **template <bitcount_t V> requires is_power_of_2<V> class uint**:
  - Fixed-width unsigned integer (e.g., uint<256>, uint<512>, uint<1024>)
  - **element_type**: Selected using TMP to be largest unsigned type ≤ V/8 bytes
  - Storage: `element_type digits_[V / (sizeof(element_type) * 8)]`
  - Constructors: default (zero), from integral type
- **Use Cases**:
  - Cryptographic hashes: `uint<256>` for SHA-256, `uint<512>` for SHA-512
  - Public key crypto: `uint<2048>` for RSA keys
  - Any fixed-precision requirement

**uint_impl.hpp** - Fixed-Size Implementation
- Default constructor value-initializes array to zero
- Integral constructor uses `std::memcpy` (little-endian) or `std::ranges::reverse_copy` (big-endian)
- Handles endianness conversion to store in little-endian internally

---

## Implementation Quality Notes

### Complete Coverage
✅ All 30 header files documented above
✅ Every major class, function, and utility covered
✅ Implementation details from both declaration and implementation files
✅ Template metaprogramming utilities fully explained
✅ Internal helpers and implementation details included

### Documentation in Source
Every header file contains extensive inline documentation with:
- Algorithm descriptions and complexity analysis
- Design rationale and trade-offs
- Usage examples and edge cases
- Mathematical formulas where applicable
- Implementation notes and optimizations

---

**End of API Reference**
