# jmaths API Reference

**Version:** 1.0.0  
**License:** GPLv3  
**Author:** Jasper de Smaele

---

## Table of Contents

1. [Introduction](#introduction)
2. [Quick Start](#quick-start)
3. [Core Types](#core-types)
4. [Mathematical Functions](#mathematical-functions)
5. [Utilities](#utilities)
6. [Template Metaprogramming](#template-metaprogramming)
7. [Configuration](#configuration)
8. [Performance Utilities](#performance-utilities)
9. [Advanced Examples](#advanced-examples)

---

## Introduction

**jmaths** is a C++20 library providing arbitrary-precision arithmetic for integers and rational numbers. It supports compile-time evaluation (constexpr), modern C++ features, and integrates seamlessly with the standard library.

### Key Features

- **Arbitrary Precision**: Numbers limited only by available memory
- **Constexpr Support**: Most operations can be evaluated at compile-time
- **Modern C++20**: Uses concepts, ranges, and three-way comparison
- **Standard Library Integration**: Works with `std::format`, `std::unordered_map`, streams
- **Performance**: Optimized algorithms with optional benchmarking

### Installation

Include the main header in your project:

```cpp
#include <jmaths/all.hpp>
```

Or include specific headers for faster compilation:

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
```

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

#### error - Base Exception Class

```cpp
class error : public std::exception {
public:
    error();
    explicit error(std::string_view message);
    const char* what() const noexcept override;
};
```

#### error::division_by_zero

Thrown when attempting to divide by zero.

```cpp
try {
    N a = 10_N;
    N b = 0_N;
    auto [q, r] = a / b;  // Throws division_by_zero
} catch (const jmaths::error::division_by_zero& e) {
    std::cerr << "Cannot divide by zero!" << std::endl;
}

// Manual checking
N divisor = get_divisor();
jmaths::error::division_by_zero::check(divisor);  // Throws if zero
```

#### error::invalid_base

Thrown when an invalid numeric base is specified (must be 2-64).

```cpp
// Valid bases
N binary{"1010", 2};        // Base 2
N octal{"755", 8};          // Base 8
N decimal{"42", 10};        // Base 10
N hex{"DEADBEEF", 16};      // Base 16
N base36{"ZZZ", 36};        // Base 36 (0-9, A-Z)
N base64{"aBc+~", 64};      // Base 64 (0-9, A-Z, a-z, +, ~)
```

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
g++ -DJMATHS_BENCHMARKING -std=c++20 program.cpp -ljmaths
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

```bash
# Compile with jmaths
g++ -std=c++20 program.cpp -ljmaths -o program

# With optimization
g++ -std=c++20 -O3 program.cpp -ljmaths -o program

# With benchmarking
g++ -std=c++20 -DJMATHS_BENCHMARKING program.cpp -ljmaths -o program
```

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

**End of API Reference**
