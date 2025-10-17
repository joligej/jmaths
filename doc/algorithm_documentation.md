# Algorithm Documentation

**jmaths Library — Arbitrary-Precision Arithmetic Algorithms**

This document provides comprehensive technical documentation of the algorithms implemented in the jmaths library. All algorithms are thoroughly documented within the source code comments in `src/headers/`, and this document serves as a high-level overview and reference.

## Table of Contents

1. [Overview](#overview)
2. [Arbitrary-Precision Unsigned Integers (N)](#unsigned-integers-n)
3. [Arbitrary-Precision Signed Integers (Z)](#signed-integers-z)
4. [Arbitrary-Precision Rational Numbers (Q)](#rational-numbers-q)
5. [Mathematical Functions](#mathematical-functions)
6. [Utility Algorithms](#utility-algorithms)
7. [Performance Characteristics](#performance-characteristics)
8. [Implementation Notes](#implementation-notes)

---

## Overview

The jmaths library implements arbitrary-precision arithmetic using a digit-based representation system. Each number is stored as a dynamically-sized vector of fixed-size integers (digits), allowing numbers to grow beyond the limits of native integer types, constrained only by available memory.

### Core Design Principles

- **Little-Endian Storage** — Least significant digit stored first, enabling efficient carry propagation in arithmetic operations
- **Zero Normalization** — Leading zeros are always removed to maintain canonical form and optimize comparisons
- **Type Safety** — Distinct types for unsigned (N), signed (Z), and rational (Q) numbers prevent type confusion
- **Constexpr Support** — Most operations are designed to work at compile-time when possible
- **Performance** — Algorithms selected and optimized for real-world performance characteristics

---

## Unsigned Integers (N)

The `basic_N` class template implements arbitrary-precision unsigned integers. All operations maintain the invariant that no leading zeros exist in the digit representation.

### Basic Arithmetic Operations

#### Addition (basic_N_detail_impl.hpp)
**Algorithm**: Multi-precision addition with carry propagation  
**Complexity**: O(n) where n is the number of digits  
**Method**: Classic schoolbook addition

The addition algorithm processes digits from least to most significant, propagating carries:

**Three-Phase Process:**
1. **Phase 1**: Add corresponding digits from both operands with carry detection
   - For each position i: `sum[i] = lhs[i] + rhs[i] + carry`
   - Carry prediction: `carry = (lhs[i] >= radix - rhs[i] - carry)`
   
2. **Phase 2**: Propagate carry through remaining digits of longer operand
   - Continue until a digit is not at maximum value
   - If carry propagates past all digits, append 1
   
3. **Phase 3**: Copy remaining digits when no carry exists

**Number Storage**: Little-endian order (least significant digit first) enables efficient left-to-right carry propagation.

#### Subtraction (basic_N_detail_impl.hpp)
**Algorithm**: Multi-precision subtraction with borrow propagation  
**Complexity**: O(n)  
**Precondition**: lhs ≥ rhs (caller must verify)  
**Method**: Schoolbook subtraction with borrow

The subtraction algorithm handles borrowing from higher-order digits:

**Two-Phase Process:**
1. **Phase 1**: Subtract corresponding digits with borrow handling
   - If `lhs[i] < rhs[i]`, borrow from higher digits
   - Borrowing: Find next non-zero digit, decrement it, set intervening zeros to `max_digit`
   - Perform subtraction: `diff[i] = lhs[i] - rhs[i]`
   
2. **Phase 2**: Copy remaining digits from lhs

**Post-processing**: Remove leading zeros to maintain canonical form.

#### Multiplication (basic_N_detail_impl.hpp)
**Two Algorithm Options** (selected at compile-time):

##### 1. Schoolbook Multiplication (Default: JMATHS_KARATSUBA=0)
**Complexity**: O(n²)  
**Method**: Long multiplication (elementary school algorithm)

Multiplies each digit of the first operand by the entire second operand, shifting appropriately:

**Implementation Details:**
- Uses wider integer type (`base_int_big_type`) to prevent overflow during digit multiplication
- For each digit `lhs[i]`:
  - Multiply by entire rhs: `lhs[i] × rhs[j]` for all j
  - Add result shifted left by i positions (multiply by radix^i)
  - Accumulate carry to next position
- Sum all partial products

**Example** (base 10): `123 × 45`
```
    123
  ×  45
  -----
    615  (123 × 5)
  4920   (123 × 4, shifted)
  -----
  5535
```

##### 2. Karatsuba Multiplication (Work in Progress: JMATHS_KARATSUBA=1)
**Complexity**: O(n^1.585) ≈ O(n^log₂3)  
**Method**: Divide-and-conquer approach

Reduces multiplication from 4 sub-multiplications to 3:
- Split numbers: `lhs = lhs_high × B^m + lhs_low`
- Compute three products instead of four
- More efficient for very large numbers (typically n > 1000 digits)

**Status**: Currently under development

#### Division (basic_N_detail_impl.hpp)
**Algorithm**: Binary long division (restoring division)  
**Complexity**: O(n × m) where n is dividend bits, m is divisor bits  
**Method**: Bit-by-bit division algorithm

Processes bits from most significant to least significant:

**Algorithm Steps:**
1. Initialize quotient `q = 0`, remainder `r = 0`
2. For each bit i in dividend (from MSB to LSB):
   - Left shift remainder: `r = r << 1`
   - Set LSB of r to dividend bit i
   - If `r ≥ divisor`:
     - Subtract: `r = r - divisor`
     - Set quotient bit i to 1
3. Return `(quotient, remainder)` pair

**Invariant**: At each step, `dividend = quotient × divisor + remainder`

### Bitwise Operations

#### AND, OR, XOR (basic_N_detail_impl.hpp)
**Complexity**: O(n) where n is number of digits  
**Method**: Digit-by-digit bitwise operations

**Operation Characteristics:**
- **AND**: Result size = min(lhs, rhs) — ANDing with implicit zeros yields zero
- **OR**: Result size = max(lhs, rhs) — ORing with implicit zeros preserves bits
- **XOR**: Result size = max(lhs, rhs) — XORing with implicit zeros preserves bits

**Post-processing**: AND and XOR remove leading zeros; OR never needs this.

### Comparison (basic_N_detail_impl.hpp)
**Algorithm**: Three-way comparison (spaceship operator <=>)  
**Complexity**: O(1) best case, O(n) worst case

**Two-Step Strategy:**
1. **Quick size comparison**: More digits = larger number (since no leading zeros)
2. **Digit-by-digit comparison**: If same size, compare from MSB to LSB

This is optimal: most comparisons terminate in O(1) by checking sizes.

### Type Conversion (basic_N_impl.hpp)

#### Integer to BigNum (handle_int_)
**Algorithm**: Convert native integer to arbitrary-precision  
**Method**: Split integer into base_int_type chunks, store in little-endian order

Supports integers up to 128 bits and beyond on supporting platforms.

#### BigNum to Integer (fits_into_)
**Algorithm**: Convert arbitrary-precision to native integer  
**Method**: Extract bytes, assemble into target type  
**Returns**: `std::optional` — `std::nullopt` if value doesn't fit in target type

---

## Signed Integers (Z)

The `basic_Z` class template implements arbitrary-precision signed integers by storing a sign separately from the magnitude (unsigned part).

### Arithmetic Operations (basic_Z_detail_impl.hpp)

All signed operations decompose into unsigned operations plus sign management.

#### Addition
**Algorithm**: Signed integer addition with magnitude comparison

**Four Cases Based on Signs:**
1. **(+a) + (+b)** = +(|a| + |b|) — Add magnitudes, result positive
2. **(+a) + (-b)** = ±(|a| - |b|) — Subtract magnitudes, sign of larger magnitude
3. **(-a) + (+b)** = ±(|b| - |a|) — Subtract magnitudes, sign of larger magnitude  
4. **(-a) + (-b)** = -(|a| + |b|) — Add magnitudes, result negative

**Implementation**: Uses underlying unsigned N operations, manages sign separately.

#### Subtraction
**Algorithm**: Signed integer subtraction (addition with negation)  
**Insight**: `a - b = a + (-b)`

Similar case analysis as addition, but with second operand's sign flipped.

#### Multiplication
**Algorithm**: Signed integer multiplication  
**Complexity**: Same as unsigned multiplication + O(1) sign handling

**Sign Rule**: Result negative if signs differ, positive if same  
**Implementation**: XOR of sign bits: `result_sign = lhs.sign XOR rhs.sign`

#### Division
**Algorithm**: Signed integer division (truncated division)  
**Complexity**: Same as unsigned division + O(1) sign handling

**C++ Standard Behavior** (rounding toward zero):
- **Quotient sign**: Negative if signs differ, positive if same
- **Remainder sign**: Always matches dividend (lhs)

**Examples:**
```
 7 ÷  3 =  2 remainder  1
-7 ÷  3 = -2 remainder -1
 7 ÷ -3 = -2 remainder  1
-7 ÷ -3 =  2 remainder -1
```

---

## Rational Numbers (Q)

The `basic_Q` class template implements arbitrary-precision rational numbers (fractions) with numerator, denominator, and sign stored separately.

**Invariant**: Denominators are always positive; sign stored separately.

### Arithmetic Operations (basic_Q_detail_impl.hpp)

#### Addition
**Algorithm**: Rational number addition  
**Formula**: `a/b + c/d = (a×d + b×c) / (b×d)`

**Process:**
1. Cross-multiply to get common denominator: `new_denom = b × d`
2. Compute numerator: `new_num = a×d + b×c`
3. Handle sign combinations (4 cases, similar to Z addition)
4. Reduce to lowest terms by dividing by GCD(numerator, denominator)

**Optimization**: Could use LCM for smaller intermediate values, but this is simpler.

#### Subtraction
**Algorithm**: Similar to addition, but subtract cross products  
**Formula**: `a/b - c/d = (a×d - b×c) / (b×d)`

#### Multiplication
**Algorithm**: Rational number multiplication  
**Formula**: `(a/b) × (c/d) = (a×c) / (b×d)`

**Advantages**: Simpler than addition — no cross-multiplication needed!  
**Sign Rule**: XOR of signs (same as integer multiplication)

#### Division
**Algorithm**: Invert and multiply  
**Formula**: `(a/b) ÷ (c/d) = (a/b) × (d/c) = (a×d) / (b×c)`

**Process**: Multiply by reciprocal of second fraction  
**Precondition**: Divisor numerator ≠ 0 (checked before operation)

#### Comparison
**Algorithm**: Cross-multiplication comparison  
**Method**: To compare `a/b` with `c/d`, compare `a×d` with `b×c`

**Advantages**: Avoids actual division, uses only integer arithmetic  
**Sign Handling**: For negative numbers, reverse comparison direction

---

## Mathematical Functions

### GCD (calc_impl.hpp)
**Algorithm**: Binary GCD (Stein's algorithm)  
**Complexity**: O(n log n) where n is number of bits  
**Advantage**: Faster than Euclidean algorithm on binary computers

Uses bit shifts and subtraction instead of expensive division:

**Five-Step Process:**
1. **Handle base cases**: `gcd(0, n) = n`
2. **Extract common power of 2**: Count trailing zeros in both numbers
3. **Make both odd**: Right shift to remove all factors of 2
4. **Main loop**: While numbers differ:
   - Ensure first ≤ second (swap if needed)
   - Subtract: `second = second - first` (result is even)
   - Make odd again: right shift to remove factors of 2
5. **Restore common factor**: Left shift result by common power of 2

**Properties Used:**
- `gcd(2a, 2b) = 2 × gcd(a, b)`
- `gcd(2a, b) = gcd(a, b)` when b is odd
- `gcd(a, b) = gcd(|a - b|, min(a, b))` when both odd

### Square Root (calc_impl.hpp)
**Algorithm**: Integer square root using binary search  
**Complexity**: O(log n × n²) — log n iterations, each doing O(n²) multiplication  
**Goal**: Find largest integer x where x² ≤ num

**Binary Search Process:**
1. Set search range: `[low=1, high=num/2]`
2. While `low ≤ high`:
   - Compute `mid = (low + high) / 2`
   - If `mid² ≤ num < (mid+1)²`: return mid
   - If `mid² < num`: search upper half
   - If `mid² > num`: search lower half
3. Return `(sqrt, remainder)` where `num = sqrt² + remainder`

**Optimization**: Could use Newton-Raphson for faster convergence on very large numbers.

### Exponentiation (calc_impl.hpp)

#### Regular Power
**Algorithm**: Exponentiation by squaring (binary exponentiation)  
**Complexity**: O(log e × n²) where e is exponent, n is number size  
**Method**: Based on binary representation of exponent

**Key Insight**: Use binary representation of exponent to reduce operations.

**Example**: `3^13` where `13 = 1101₂`
```
13 = 8 + 4 + 1
3^13 = 3^8 × 3^4 × 3^1
```

**Algorithm Steps:**
1. Initialize result = 1, base = number, exp = exponent
2. While exp > 0:
   - If exp is odd: multiply result by base
   - Square base: `base = base × base`
   - Halve exponent: `exp = exp / 2`

**Benefit**: Computes huge powers efficiently (e.g., `2^1000000` in ~1000000 multiplications, not 10^6 - 1)

#### Modular Power
**Algorithm**: Modular exponentiation by squaring  
**Complexity**: O(log e × n²) with bounded intermediate values  
**Application**: Critical for cryptography (RSA, Diffie-Hellman, etc.)

**Key Property**: `(a × b) mod m = ((a mod m) × (b mod m)) mod m`

**Advantage**: Apply modulo after each multiplication to keep numbers bounded  
**Result**: Intermediate values never exceed `mod²`, even for huge exponents

**Example**: Computing `3^1000 mod 7` requires only small intermediate values.

---

## Utility Algorithms

### Random Number Generation (rand_impl.hpp)

#### Unsigned Random (basic_N)
**Algorithm**: Random arbitrary-precision unsigned integer generation  
**Goal**: Generate number with at most N bits uniformly distributed

**Strategy:**
1. Calculate full digits needed: `full_digits = N / bits_per_digit`
2. Generate random value for each full digit using base random generator
3. Calculate remaining bits: `remaining = N % bits_per_digit`
4. If remaining > 0: generate partial digit with only those bits random
5. Remove leading zeros (may occur if MSB of last digit is 0)

**Distribution**: Uniform over `[0, 2^N - 1]`

**Reseeding**: Periodically reseeds generator to maintain randomness quality (configurable)

#### Signed Random (basic_Z)
**Algorithm**: Generate magnitude, then randomly choose sign

**Process:**
1. Generate unsigned magnitude using N generator
2. Choose sign randomly with 50% probability each
3. Special case: zero is always positive

### Hashing (hash_impl.hpp)

Good hash functions are essential for using arbitrary-precision types in hash-based containers (unordered_map, unordered_set).

#### Hash for basic_N
**Algorithm**: Byte-sequence hashing  
**Method**: Treat digit array as byte sequence, use `std::hash<std::string_view>`

**Properties:**
- **Consistency**: Same value always produces same hash
- **Distribution**: Well-distributed across hash space
- **Efficiency**: O(n) time, no copies made

**Optimization**: Empty (zero) numbers get cached hash value.

#### Hash for basic_Z
**Algorithm**: Combine magnitude hash with sign  
**Formula**: `hash(Z) = hash(magnitude) XOR (sign << offset)`

**Offset Calculation**: `first_digit % (bits_per_size_t)` — adds entropy

**Guarantee**: Positive and negative of same value have different hashes.

#### Hash for basic_Q
**Algorithm**: Combine numerator, denominator, and sign hashes  
**Formula**: `hash(Q) = (hash(num) XOR hash(denom)) XOR (sign << offset)`

**Offset**: Based on XOR of first digits of numerator and denominator

**Invariant**: Fractions in reduced form have consistent hashes.

---

## Performance Characteristics

### Time Complexities Summary

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| **Arithmetic** | | |
| Addition/Subtraction | O(n) | n = number of digits; optimal |
| Multiplication (schoolbook) | O(n²) | Default implementation |
| Multiplication (Karatsuba) | O(n^1.585) | Work in progress; better for large n |
| Division | O(n × m) | Bit-by-bit method; n = dividend bits, m = divisor bits |
| **Bitwise** | | |
| AND, OR, XOR | O(n) | Digit-by-digit operations |
| Bit Shifts | O(n) | May require reallocation |
| **Comparison** | | |
| Equality (==) | O(1) to O(n) | O(1) if different sizes |
| Three-way (<=>) | O(1) to O(n) | O(1) if different sizes |
| **Mathematical** | | |
| GCD | O(n log n) | Binary GCD; optimal |
| Square Root | O(log n × n²) | Binary search; each iteration multiplies |
| Power | O(log e × n²) | e = exponent; binary exponentiation |
| Modular Power | O(log e × n²) | Bounded intermediate values |
| **Utility** | | |
| Hash | O(n) | Linear in number of digits |
| Random Generation | O(n) | Linear in number of digits |
| String Conversion | O(n log n) | Depends on base |

### Space Complexities

| Operation | Space Usage | Notes |
|-----------|-------------|-------|
| Most operations | O(n) | Temporary storage for result |
| In-place operators (+=, etc.) | O(1) to O(n) | May avoid allocation if space available |
| Division | O(n) | Stores quotient and remainder |
| String conversion | O(n log n) | Depends on output base |

**Memory Management:**
- Pre-allocation used to minimize reallocations
- Move semantics employed to avoid copies
- Small string optimization not applicable (arbitrary size)

---

## Implementation Notes

### Design Decisions

1. **Number Representation**: Little-endian digit storage (LSB first)
   - **Rationale**: Enables left-to-right carry propagation in addition
   - **Trade-off**: MSB operations slightly less efficient

2. **Normalization**: Leading zeros always removed for canonical form
   - **Rationale**: Simplifies comparison, ensures unique representation
   - **Invariant**: Maintained by all operations

3. **Optimization**: Uses wider integer types to prevent overflow during operations
   - **Example**: `uint64_t × uint64_t` uses `__uint128_t` intermediate
   - **Benefit**: Avoids expensive carry handling in inner loops

4. **Constexpr Support**: Most functions are constexpr-compatible
   - **Benefit**: Compile-time computation when possible
   - **Limitation**: Some operations (I/O, random) cannot be constexpr

5. **Error Handling**: Division by zero checked before operations
   - **Method**: `error::division_by_zero::check()` throws exception
   - **Philosophy**: Fail fast with clear error messages

### Compilation Options

- **JMATHS_KARATSUBA**: Enable Karatsuba multiplication (0=off, 1=on)
  - **Default**: 0 (schoolbook multiplication)
  - **Note**: Karatsuba currently work in progress

- **JMATHS_PERIODICALLY_RESEED_RAND**: Reseed random generator periodically
  - **Default**: Enabled
  - **Period**: Every 2 months of runtime

### File Organization

**Core Type Headers** (in `src/headers/`):
- `basic_N.hpp`: Unsigned integer class definition with comprehensive inline documentation
- `basic_Z.hpp`: Signed integer class definition with sign-magnitude representation
- `basic_Q.hpp`: Rational number class definition with automatic reduction
- `constants_and_types.hpp.in`: Fundamental type definitions and library constants

**Implementation Files** (in `src/headers/`):
- `basic_N_detail_impl.hpp`: Core unsigned integer operations (addition, subtraction, multiplication, division, bitwise)
- `basic_N_impl.hpp`: Type conversion, comparison operators, and helper functions for N
- `basic_Z_detail_impl.hpp`: Signed integer operations with four-case sign handling
- `basic_Z_impl.hpp`: Construction, increment/decrement, and compound operators for Z
- `basic_Q_detail_impl.hpp`: Rational number operations with cross-multiplication
- `basic_Q_impl.hpp`: Floating-point conversion, fraction reduction, and compound operators for Q
- `calc_impl.hpp`: Mathematical functions (GCD, sqrt, power)
- `rand_impl.hpp`: Random number generation
- `hash_impl.hpp`: Hash function implementations for unordered containers
- `uint_impl.hpp`: Fixed-size unsigned integer utilities
- `sign_type_impl.hpp`: Sign extraction and management
- `error_impl.hpp`: Exception handling
- `literals_impl.hpp`: User-defined literals (_N, _Z, _Q)
- `formatter.hpp`: std::format support for all types

**Supporting Headers** (in `src/headers/`):
- `TMP.hpp`: Template metaprogramming utilities with detailed documentation
- `scoped_timer.hpp`: RAII-based profiling timer for benchmarking
- `calc.hpp`: Mathematical function declarations
- `declarations.hpp`: Forward declarations to break circular dependencies
- `def.hh`: Macro definitions (JMATHS_FUNCTION_TO_LOG, JMATHS_REPEAT)
- `rand.hpp`: Random generator wrapper with periodic reseeding
- `hash.hpp`: std::hash specializations
- `literals.hpp`: User-defined literal declarations
- `error.hpp`: Exception class hierarchy
- `sign_type.hpp`: Sign management base class
- `uint.hpp`: Fixed-size unsigned integer template
- `all.hpp`: Convenience header that includes everything

**Documentation Coverage**:
- ✅ All 30 header files have comprehensive algorithm documentation
- ✅ Every major function has complexity analysis and algorithm explanation
- ✅ Edge cases and special conditions are documented
- ✅ Examples provided for complex operations
- ✅ Design rationale explained for key decisions

---

**Document Status**: Complete  
**Last Updated**: October 12, 2025  
**Corresponds to**: jmaths library with full algorithm documentation in all source files
