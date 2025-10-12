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

/**
 * @file formatter.hpp
 * @brief std::format support for jmaths arbitrary-precision types
 *
 * This file provides integration with C++20's std::format for formatting
 * arbitrary-precision numbers (N, Z, Q) in various bases.
 *
 * USAGE EXAMPLES:
 * - std::format("{}", n)         // Format in base 10 (default)
 * - std::format("{:2}", n)       // Format in binary
 * - std::format("{:16}", n)      // Format in hexadecimal
 * - std::format("{:36}", n)      // Format in base 36
 */

namespace jmaths {

/**
 * @struct format_parser
 * @brief Base parser for extracting format specifiers
 *
 * ALGORITHM: Parses the format string to extract base specification
 * - Default base: 10 (decimal)
 * - Valid range: 2-64 (enforced by error::invalid_base::check)
 *
 * FORMAT SYNTAX: {:<base>}
 * Examples: {:2} for binary, {:16} for hex, {:10} for decimal
 */
struct format_parser {
    unsigned base = default_base;

    /**
     * @brief Parse format specifier from format string
     * @param ctx Format context from std::formatter
     * @return Iterator to end of format specifier
     * @throws std::format_error if format string is invalid
     * @throws error::invalid_base if base is out of range
     *
     * ALGORITHM:
     * 1. Skip if at end or closing brace
     * 2. Parse digits to extract base number
     * 3. Validate base is in range [2, 64]
     */
    constexpr auto parse(auto & ctx) {
        auto it = ctx.begin();

        if (it == ctx.end()) { return it; }

        if (*it == '}') { return it; }

        base = 0U;

        // Parse base as decimal number
        do {
            base *= 10U;

            if (!(*it >= '0' && *it <= '9')) {
                throw std::format_error("Invalid format arguments.");
            }

            base += static_cast<unsigned>(*it - '0');

            ++it;
        } while (*it != '}');

        error::invalid_base::check(base);

        return it;
    }
};

/**
 * @struct format_output
 * @brief Formatter implementation for jmaths types (N, Z, Q)
 * @tparam T Type to format (must be instance of basic_N, basic_Z, or basic_Q)
 *
 * OPTIMIZATION: Uses specialized methods for common bases:
 * - Base 2: to_bin() - optimized binary conversion
 * - Base 16: to_hex() - optimized hexadecimal conversion
 * - Other bases: conv_to_base_() - general conversion algorithm
 */
template <TMP::instance_of<basic_N, basic_Z, basic_Q> T> struct format_output : format_parser {
    /**
     * @brief Format the arbitrary-precision number to output
     * @param obj The number to format
     * @param ctx Format context
     * @return Iterator to end of output
     *
     * ALGORITHM: Routes to appropriate conversion method based on base
     */
    auto format(const T & obj, auto & ctx) const {
        JMATHS_FUNCTION_TO_LOG;

        switch (base) {
            case 2U:
                return std::ranges::copy(obj.to_bin(), ctx.out()).out;
            case 16U:
                return std::ranges::copy(obj.to_hex(), ctx.out()).out;
            default:
                return std::ranges::copy(obj.conv_to_base_(base), ctx.out()).out;
        }
    }
};

}  // namespace jmaths
