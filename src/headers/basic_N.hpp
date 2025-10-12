// The jmaths library for C++
// Copyright (C) 2025  Jasper de Smaele

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <istream>
#include <limits>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "TMP.hpp"
#include "constants_and_types.hpp"
#include "declarations.hpp"
#include "formatter.hpp"

/**
 * @file basic_N.hpp
 * @brief Arbitrary-precision unsigned integer type (basic_N class template)
 *
 * This file defines the basic_N class template, which implements arbitrary-precision
 * unsigned integers. Numbers are stored as vectors of fixed-size digits in little-endian
 * order (least significant digit first).
 *
 * KEY FEATURES:
 * - Dynamic size: Numbers grow as needed, limited only by available memory
 * - Little-endian storage: Efficient carry propagation during addition
 * - Zero normalization: Leading zeros always removed for canonical representation
 * - Constexpr support: Most operations work at compile-time
 * - Operator overloading: Natural syntax for arithmetic operations
 *
 * DESIGN INVARIANTS:
 * - No leading zeros in digit representation (except for zero itself)
 * - Empty digits_ vector represents zero
 *
 * TEMPLATE PARAMETERS:
 * - BaseInt: Base digit type (e.g., uint32_t)
 * - BaseIntBig: Wider type for intermediate calculations (e.g., uint64_t)
 * - Allocator: Memory allocator for digit storage
 */

// declarations of N and associated functions and types
namespace jmaths {

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::ostream & operator<<(std::ostream & os, const basic_N_type & n);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::istream & operator>>(std::istream & is, basic_N_type & n);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator+(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator-(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator*(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::pair<basic_N_type, basic_N_type> operator/(const basic_N_type & lhs,
                                                          const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator&(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator|(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr basic_N_type operator^(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr bool operator==(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr bool operator==(const basic_N_type & lhs, std::integral auto rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr bool operator==(std::integral auto lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::strong_ordering operator<=>(const basic_N_type & lhs, const basic_N_type & rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::strong_ordering operator<=>(const basic_N_type & lhs, std::integral auto rhs);

template <TMP::instance_of<basic_N> basic_N_type>
constexpr std::strong_ordering operator<=>(std::integral auto lhs, const basic_N_type & rhs);

/**
 * @class basic_N
 * @brief Arbitrary-precision unsigned integer
 *
 * Implements unsigned integers of arbitrary size using a vector of base_int_type digits.
 * Each number is stored in little-endian order (least significant digit first) to optimize
 * carry propagation during arithmetic operations.
 *
 * STORAGE FORMAT:
 * - digits_[0] = least significant digit
 * - digits_[n-1] = most significant digit
 * - radix = 2^(bits in base_int_type)
 *
 * EXAMPLE: For base_int_type = uint32_t (radix = 2^32)
 * The number 2^64 + 5 would be stored as: digits_ = {5, 0, 1}
 *
 * COMPLEXITY SUMMARY:
 * - Addition/Subtraction: O(n)
 * - Multiplication: O(n²) (schoolbook) or O(n^1.585) (Karatsuba, WIP)
 * - Division: O(n×m) bit-by-bit
 * - Comparison: O(1) best case, O(n) worst case
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator = allocator<BaseInt>>
class basic_N {
    using basic_Z_type = basic_Z<BaseInt, BaseIntBig, Allocator>;
    using basic_Q_type = basic_Q<BaseInt, BaseIntBig, Allocator>;

    using base_int_type = BaseInt;
    using base_int_big_type = BaseIntBig;
    using allocator_type = Allocator;
    static constexpr base_int_type max_digit = std::numeric_limits<base_int_type>::max();
    static constexpr base_int_big_type radix = static_cast<base_int_big_type>(max_digit) + 1;
    static constexpr std::size_t base_int_type_size = sizeof(base_int_type);
    static constexpr bitcount_t base_int_type_bits = base_int_type_size * bits_in_byte;

    friend struct calc;
    friend struct std::hash<basic_N>;
    friend struct std::hash<basic_Z_type>;
    friend struct std::hash<basic_Q_type>;

    friend class basic_Q<BaseInt, BaseIntBig, Allocator>;

    friend struct rand<basic_N>;
    friend struct rand<basic_Z_type>;

    friend struct format_output<basic_N>;

   public:
    /**
     * @class bit_reference
     * @brief Proxy class for modifiable bit access
     * Allows assignment to individual bits via operator[]
     */
    class bit_reference;

    /**
     * @class const_bit_reference
     * @brief Proxy class for const bit access
     * Read-only access to individual bits
     */
    class const_bit_reference;

    /**
     * @struct detail
     * @brief Internal implementation details and helper functions
     * Contains static methods for core arithmetic and logical operations
     */
    struct detail;

    // Compiler-generated special members
    constexpr basic_N(const basic_N &) = default;
    constexpr basic_N(basic_N &&) = default;
    constexpr basic_N & operator=(const basic_N &) = default;
    constexpr basic_N & operator=(basic_N &&) = default;

    /**
     * @brief Default constructor - creates zero
     * POSTCONDITION: is_zero() == true
     */
    constexpr basic_N();

    /**
     * @brief Construct from string in specified base
     * @param num_str String representation of number
     * @param base Numeric base (2-64), default is 10
     * @throws error::invalid_base if base out of range
     *
     * SUPPORTED BASES: 2-64
     * - 2-36: Uses digits 0-9, A-Z
     * - 37-64: Uses extended character set
     */
    explicit constexpr basic_N(std::string_view num_str, unsigned base = default_base);

    /**
     * @brief Construct from integral type
     * @param num Integer value to convert
     *
     * ALGORITHM: Splits integer into base_int_type chunks, stores in little-endian
     * Supports up to 128-bit integers on supporting platforms
     */
    constexpr basic_N(std::integral auto num);

    /**
     * @brief Check if value is zero
     * @return true if number equals zero
     * COMPLEXITY: O(1)
     */
    [[nodiscard]] constexpr bool is_zero() const;

    /**
     * @brief Check if value is one
     * @return true if number equals one
     * COMPLEXITY: O(1)
     */
    [[nodiscard]] constexpr bool is_one() const;

    /**
     * @brief Check if value is even
     * @return true if least significant bit is 0
     * COMPLEXITY: O(1)
     */
    [[nodiscard]] constexpr bool is_even() const;

    /**
     * @brief Check if value is odd
     * @return true if least significant bit is 1
     * COMPLEXITY: O(1)
     */
    [[nodiscard]] constexpr bool is_odd() const;

    /**
     * @brief Count trailing zeros (number of consecutive zero bits from LSB)
     * @return Number of trailing zero bits
     * COMPLEXITY: O(1) to O(n) depending on position of first 1-bit
     *
     * USAGE: Used in Binary GCD algorithm
     */
    [[nodiscard]] constexpr bitcount_t ctz() const;

    /**
     * @brief Count total number of bits in number
     * @return Number of bits needed to represent this number
     * COMPLEXITY: O(1)
     *
     * FORMULA: bits() = floor(log₂(number)) + 1
     */
    [[nodiscard]] constexpr bitcount_t bits() const;

    /**
     * @brief Size of this object in bytes (including dynamic allocation)
     * @return Total memory usage
     * COMPLEXITY: O(1)
     */
    [[nodiscard]] constexpr std::size_t size() const;

    /**
     * @brief Convert to string in specified base
     * @param base Numeric base (2-64)
     * @return String representation
     * COMPLEXITY: O(n log n) where n is number of digits
     *
     * ALGORITHM: Repeated division by base, collect remainders
     */
    [[nodiscard]] constexpr std::string to_str(unsigned base = default_base) const;

    /**
     * @brief Convert to hexadecimal string
     * @return String in base 16
     * COMPLEXITY: O(n) - optimized for power-of-2 base
     */
    [[nodiscard]] constexpr std::string to_hex() const;

    /**
     * @brief Convert to binary string
     * @return String in base 2
     * COMPLEXITY: O(n) - optimized for base 2
     */
    [[nodiscard]] constexpr std::string to_bin() const;

    /**
     * @brief Convert to bool (true if non-zero)
     * COMPLEXITY: O(1)
     */
    explicit constexpr operator bool() const;

    /**
     * @brief Try to fit value into unsigned integral type
     * @return std::optional<T> containing value, or std::nullopt if too large
     * COMPLEXITY: O(1)
     */
    template <std::unsigned_integral T> [[nodiscard]] constexpr std::optional<T> fits_into() const;

    /**
     * @brief Try to fit value into signed integral type
     * @return std::optional<T> containing value, or std::nullopt if too large
     * COMPLEXITY: O(1)
     */
    template <std::signed_integral T> [[nodiscard]] constexpr std::optional<T> fits_into() const;

    /**
     * @brief Access bit at position (mutable reference)
     * @param pos Bit position (0 = LSB)
     * @return Proxy object for bit modification
     *
     * USAGE: num[5] = true; // Set bit 5
     */
    [[nodiscard]] constexpr bit_reference operator[](bitpos_t pos) &;

    /**
     * @brief Access bit at position (const reference)
     * @param pos Bit position (0 = LSB)
     * @return Proxy object for bit reading
     */
    [[nodiscard]] constexpr const_bit_reference operator[](bitpos_t pos) const &;

    /**
     * @brief Set value to zero
     * POSTCONDITION: is_zero() == true
     * COMPLEXITY: O(1)
     */
    constexpr void set_zero();

    /**
     * @brief Pre-increment operator
     * @return Reference to *this
     * COMPLEXITY: O(1) average, O(n) worst case (when carry propagates)
     */
    constexpr basic_N & operator++();

    /**
     * @brief Pre-decrement operator
     * @return Reference to *this
     * PRECONDITION: is_zero() == false
     * COMPLEXITY: O(1) average, O(n) worst case (when borrow propagates)
     */
    constexpr basic_N & operator--();

    // Compound assignment operators - modify lhs in place
    constexpr basic_N & operator+=(const basic_N & rhs);
    constexpr basic_N & operator-=(const basic_N & rhs);
    constexpr basic_N & operator*=(const basic_N & rhs);

    constexpr basic_N & operator&=(const basic_N & rhs);
    constexpr basic_N & operator|=(const basic_N & rhs);
    constexpr basic_N & operator^=(const basic_N & rhs);

    /**
     * @brief Bitwise NOT operator
     * @return New number with all bits flipped
     * NOTE: Result may be very large since it flips all bits up to MSB
     */
    [[nodiscard]] constexpr basic_N operator~() const;

    /**
     * @brief Left bit shift operator
     * @param pos Number of positions to shift
     * @return New number shifted left (multiplied by 2^pos)
     * COMPLEXITY: O(n)
     */
    [[nodiscard]] constexpr basic_N operator<<(bitcount_t pos) const;

    /**
     * @brief Right bit shift operator
     * @param pos Number of positions to shift
     * @return New number shifted right (divided by 2^pos, rounded down)
     * COMPLEXITY: O(n)
     */
    [[nodiscard]] constexpr basic_N operator>>(bitcount_t pos) const;

    constexpr basic_N & operator<<=(bitcount_t pos);
    constexpr basic_N & operator>>=(bitcount_t pos);

    /**
     * @brief Assign from string
     * @param num_str String in default base (10)
     * @return Reference to *this
     */
    constexpr basic_N & operator=(std::string_view num_str);

    /**
     * @brief Assign from integral type
     * @param rhs Integer value
     * @return Reference to *this
     */
    constexpr basic_N & operator=(std::integral auto rhs);

   protected:
    // Protected methods for internal use and inheritance

    /**
     * @brief Get size of dynamic memory allocation
     * @return Number of bytes allocated for digits_
     */
    [[nodiscard]] constexpr std::size_t dynamic_size_() const;

    // Internal operation methods (implementations call detail:: static methods)
    constexpr void opr_incr_();
    constexpr void opr_decr_();
    constexpr void opr_add_assign_(const basic_N & rhs);
    constexpr void opr_subtr_assign_(const basic_N & rhs);
    constexpr void opr_mult_assign_(const basic_N & rhs);
    constexpr void opr_and_assign_(const basic_N & rhs);
    constexpr void opr_or_assign_(const basic_N & rhs);
    constexpr void opr_xor_assign_(const basic_N & rhs);

    [[nodiscard]] constexpr basic_N opr_compl_() const;
    [[nodiscard]] constexpr basic_N opr_bitshift_l_(bitcount_t pos) const;
    [[nodiscard]] constexpr basic_N opr_bitshift_r_(bitcount_t pos) const;

    constexpr void opr_bitshift_l_assign_(bitcount_t pos);
    constexpr void opr_bitshift_r_assign_(bitcount_t pos);

    constexpr void opr_assign_(std::string_view num_str);
    constexpr void opr_assign_(std::integral auto rhs);

    /**
     * @brief Convert to string in specified base
     * @param base Numeric base (2-64)
     * @return String representation
     *
     * ALGORITHM: Repeated division by base, collecting remainders
     * For power-of-2 bases, optimized to direct bit extraction
     */
    [[nodiscard]] constexpr std::string conv_to_base_(unsigned base = default_base) const;

   private:
    /**
     * @brief Digit storage - little-endian order
     * digits_[0] = least significant digit
     * digits_[size-1] = most significant digit
     * Empty vector represents zero
     */
    std::vector<base_int_type, allocator_type> digits_;

    /**
     * @brief Remove leading zero digits to maintain canonical form
     * POSTCONDITION: digits_.empty() || digits_.back() != 0
     */
    constexpr void remove_leading_zeroes_();

    /**
     * @brief Get most significant digit
     * @return First (most significant) digit, or 0 if empty
     */
    [[nodiscard]] constexpr base_int_type front_() const;

    /**
     * @brief Parse string representation and populate digits_
     * @param num_str String to parse
     * @param base Numeric base
     */
    constexpr void handle_str_(std::string_view num_str, unsigned base);

    /**
     * @brief Convert integral type to digit representation
     * @param num Integer to convert
     */
    constexpr void handle_int_(std::integral auto num);

    /**
     * @brief Internal conversion to unsigned integral type
     * @param max_byte Maximum bytes to extract
     * @return Converted value
     */
    template <std::unsigned_integral T>
    [[nodiscard]] constexpr T fit_into_(std::size_t max_byte) const;

    /**
     * @brief Get bit at position
     * @param pos Bit position (0 = LSB)
     * @return Bit value (0 or 1)
     */
    [[nodiscard]] constexpr bool bit_(bitpos_t pos) const;

    /**
     * @brief Set bit at position
     * @param pos Bit position
     * @param val Bit value to set
     */
    constexpr void bit_(bitpos_t pos, bool val);

    template <typename T> class bit_reference_base_;

    /**
     * @brief Constant representing 1 (used internally)
     */
    static const basic_N one_;
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
template <TMP::decays_to<basic_N<BaseInt, BaseIntBig, Allocator>> T>
class basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference_base_<T> {
    // because of error: type constraint differs in template redeclaration
    // otherwise: class basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference_base_ {
   public:
    bit_reference_base_() = delete;
    bit_reference_base_(T &&, bitpos_t) = delete;
    constexpr bit_reference_base_(T & num, bitpos_t pos);

    constexpr operator bool() const;
    explicit constexpr operator int() const;

   protected:
    T & num_;
    const bitpos_t pos_;
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
class basic_N<BaseInt, BaseIntBig, Allocator>::bit_reference : public bit_reference_base_<basic_N> {
   public:
    using bit_reference_base_<basic_N>::bit_reference_base_;

    constexpr bit_reference(const bit_reference & ref);

    constexpr bit_reference & operator=(bool val);
    constexpr bit_reference & operator=(const bit_reference & ref);
    constexpr bit_reference & operator=(const const_bit_reference & ref);
};

template <typename BaseInt, typename BaseIntBig, typename Allocator>
class basic_N<BaseInt, BaseIntBig, Allocator>::const_bit_reference :
    public bit_reference_base_<const basic_N> {
   public:
    using bit_reference_base_<const basic_N>::bit_reference_base_;

    constexpr const_bit_reference(const const_bit_reference & ref);
    constexpr const_bit_reference(const bit_reference & ref);

    auto & operator=(bool) = delete;
    auto & operator=(const bit_reference &) = delete;
    auto & operator=(const const_bit_reference &) = delete;
};

}  // namespace jmaths

namespace jmaths {

/**
 * @struct basic_N::detail
 * @brief Static helper functions for core operations
 *
 * This struct contains the actual implementations of arithmetic and logical
 * operations. They are static functions to allow calling without creating
 * temporary objects.
 */
template <typename BaseInt, typename BaseIntBig, typename Allocator>
struct basic_N<BaseInt, BaseIntBig, Allocator>::detail {
    // I/O operations
    static constexpr std::ostream & opr_ins(std::ostream & os, const basic_N & n);
    static constexpr std::istream & opr_extr(std::istream & is, basic_N & n);

    // Arithmetic operations (see basic_N_detail_impl.hpp for detailed algorithm documentation)
    static constexpr basic_N opr_add(const basic_N & lhs, const basic_N & rhs);
    static constexpr basic_N opr_subtr(basic_N lhs, const basic_N & rhs);
    static constexpr basic_N opr_mult(const basic_N & lhs, const basic_N & rhs);
    static constexpr std::pair<basic_N, basic_N> opr_div(const basic_N & lhs, const basic_N & rhs);

    // Bitwise operations
    static constexpr basic_N opr_and(const basic_N & lhs, const basic_N & rhs);
    static constexpr basic_N opr_or(const basic_N & lhs, const basic_N & rhs);
    static constexpr basic_N opr_xor(const basic_N & lhs, const basic_N & rhs);

    // Comparison operations
    static constexpr bool opr_eq(const basic_N & lhs, const basic_N & rhs);
    static constexpr bool opr_eq(const basic_N & lhs, std::integral auto rhs);

    static constexpr std::strong_ordering opr_comp(const basic_N & lhs, const basic_N & rhs);
    static constexpr std::strong_ordering opr_comp(const basic_N & lhs, std::integral auto rhs);
};

}  // namespace jmaths

namespace jmaths {

// Type aliases for common configurations

#if defined(UINT64_MAX) && defined(UINT32_MAX)

    /**
     * @typedef N
     * @brief Default arbitrary-precision unsigned integer type
     * Uses 32-bit digits with 64-bit intermediate calculations
     * This is the recommended configuration for most use cases
     */
    #define JMATHS_TYPEDEF_N 32
using N = basic_N<std::uint32_t, std::uint64_t>;

#elif defined(UINT32_MAX) && defined(UINT16_MAX)

    #define JMATHS_TYPEDEF_N 16
using N = basic_N<std::uint16_t, std::uint32_t>;

#elif defined(UINT16_MAX) && defined(UINT8_MAX)

    #define JMATHS_TYPEDEF_N 8
using N = basic_N<std::uint8_t, std::uint16_t>;

#endif

}  // namespace jmaths

template <jmaths::TMP::instance_of<jmaths::basic_N> N>
struct std::formatter<N> : jmaths::format_output<N> {};

#include "basic_N_detail_impl.hpp"
#include "basic_N_impl.hpp"
