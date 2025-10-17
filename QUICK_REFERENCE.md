# jmaths Library Behavior Quick Reference

**For Developers and Users**  
**Updated:** 2025-10-17

This quick reference clarifies commonly misunderstood behaviors in the jmaths library.

---

## String Constructors

### ❌ Common Mistakes

```cpp
// WRONG: Expecting whitespace trimming
N a(" 123 ");  // ERROR: spaces are not valid digits

// WRONG: Using + as positive sign prefix for N
N b("+456");   // ERROR: + is digit 62 (base-64), not a sign

// WRONG: Using 0x prefix
N c("0xFF");   // ERROR: remove prefix, use N{"FF", 16}

// WRONG: Expecting exception on empty string
try {
    N d("");   // This is valid! Creates N(0)
} catch(...) {
    // This won't execute
}
```

### ✅ Correct Usage

```cpp
// Clean strings only
N a("123");           // ✓ Works

// Use Z type for signed integers
Z b("-456");          // ✓ Z supports - prefix
Z c("789");           // ✓ Positive without prefix

// Specify base without prefix
N d("FF", 16);        // ✓ Hexadecimal
N e("1010", 2);       // ✓ Binary

// Empty = zero
N f("");              // ✓ Returns N(0)
N g("0");             // ✓ Also N(0)
N h("000");           // ✓ Also N(0)
```

---

## Base-64 Encoding

The library supports bases 2-64 with this character mapping:

| Base Range | Characters | Example |
|------------|-----------|---------|
| 2-10 | `0-9` | `N{"42"}` = 42 |
| 11-36 | `0-9`, `A-Z` | `N{"FF", 16}` = 255 |
| 37-62 | `0-9`, `A-Z`, `a-z` | `N{"Zz", 36}` |
| 63 | Above + `+` | `N{"+", 63}` = 62 |
| 64 | Above + `~` | `N{"~", 64}` = 63 |

### Important

- **In base-10:** Only `0-9` are valid
- **Characters `+` and `~`** are digits in base-63/64, NOT sign indicators
- **Character `-`** is only valid in Z type, not N type

```cpp
// Base-64 example
N base64_num("aB+~", 64);  // Valid base-64 number

// WRONG in base-10
N wrong("+123");  // + is not valid in base-10

// CORRECT in base-10
N correct("123"); // Just digits
```

---

## Bitwise NOT (~) Operator

### Key Difference: Arbitrary-Precision vs Fixed-Width

```cpp
// Fixed-width (e.g., uint32_t)
uint32_t x = 0;
uint32_t y = ~x;  // 0xFFFFFFFF (all bits set)

// Arbitrary-precision (jmaths N)
N x = 0;
N y = ~x;         // Returns N(0) - see below why
```

### Why ~N(0) == N(0)?

Arbitrary-precision integers don't have a fixed bit width:
- **Fixed-width:** "all bits set" is well-defined (e.g., 32 bits of 1s)
- **Arbitrary-precision:** No maximum bit width, so no "all bits set" state
- **Implementation:** Inverts bits in internal representation, removes leading zeros
- **For zero:** No bits to invert → still zero after cleanup

### ✅ Correct Usage

```cpp
// Works as expected with non-zero
N a = 42;
N b = ~a;           // ✓ Inverts bits
N c = ~~a;          // ✓ Returns original: c == a

// Special case for zero
N zero = 0;
N inverted = ~zero; // ✓ Returns N(0) (documented behavior)

// Practical usage
N mask = 0xFF;
N data = 0b10101010;
N result = data & mask;  // Extract bits
```

### ❌ Don't Use For

```cpp
// WRONG: Trying to get "all ones"
N wrong = ~N(0);    // This is still 0, not all ones!

// CORRECT: Use explicit mask
N mask = (N(1) << 32) - N(1);  // 32 bits of 1s
```

---

## Type Conversion with fits_into<T>()

### ✅ Safe Conversion

```cpp
N large = "12345"_N;

// Safe conversion with check
if (auto val = large.fits_into<uint32_t>()) {
    uint32_t native = *val;  // Use native type
} else {
    // Keep using arbitrary-precision
}

// Range checking
N byte_value = 200;
ASSERT(byte_value.fits_into<uint8_t>().has_value());  // true

N too_large = 256;
ASSERT(!too_large.fits_into<uint8_t>().has_value()); // false
```

### Type Limits

```cpp
// Maximum values that fit
N(255).fits_into<uint8_t>()     // ✓ has_value()
N(256).fits_into<uint8_t>()     // ✗ nullopt

N(65535).fits_into<uint16_t>()  // ✓ has_value()
N(65536).fits_into<uint16_t>()  // ✗ nullopt

// Zero always fits
N(0).fits_into<uint8_t>()       // ✓ has_value() == 0
```

---

## Sign Handling (Z Type)

### Default Constructor

```cpp
Z zero;                  // Creates zero
ASSERT(zero.is_zero());  // ✓ true
ASSERT(zero.is_positive()); // ✓ true (library marks zero as positive)
ASSERT(!zero.is_negative()); // ✓ true
```

### String Constructor

```cpp
// Only '-' prefix is supported
Z neg("-123");      // ✓ Negative
Z pos("123");       // ✓ Positive (no prefix needed)

// NO support for '+' prefix
Z wrong("+123");    // ✗ ERROR: + is not recognized
```

---

## Rational Numbers (Q Type)

### String Format

```cpp
// Fraction format: "numerator/denominator"
Q half("1/2");           // ✓ 1/2
Q third("1/3");          // ✓ 1/3

// Integer format: "number"
Q five("5");             // ✓ 5/1

// NO whitespace
Q wrong(" 1 / 2 ");      // ✗ ERROR: spaces not trimmed

// Automatic reduction
Q reduced("4/6");        // ✓ Becomes 2/3
```

### Constructor Ambiguity

```cpp
// WRONG: Ambiguous when using integer literals
Q wrong(1, 2);           // Compilation error (multiple candidates)

// CORRECT: Use explicit types
Q correct(N(1), N(2));   // ✓ Unambiguous
Q also_correct("1/2");   // ✓ String constructor
```

---

## Best Practices Summary

1. **String Input:** Always provide clean strings without whitespace or prefixes
2. **Bitwise NOT:** Remember `~0` special case; use explicit masks instead
3. **Type Conversion:** Always check `has_value()` before dereferencing
4. **Sign Prefixes:** Use Z type for signed numbers; only `-` is supported
5. **Q Construction:** Use string form or explicit N types to avoid ambiguity
6. **Base Encoding:** Be aware that `+` and `~` are digits, not operators

---

## When in Doubt

1. Check `doc/API_Reference.md` for detailed documentation
2. Review `LIBRARY_BEHAVIOR_ANALYSIS.md` for design rationale
3. Look at passing tests for usage examples
4. Remember: The library prioritizes performance and correctness over convenience

---

## Need Help?

- **API Reference:** `doc/API_Reference.md`
- **Behavior Analysis:** `LIBRARY_BEHAVIOR_ANALYSIS.md`
- **Test Examples:** `test/test_*.cpp`
- **Contributing:** `CONTRIBUTING.md`
