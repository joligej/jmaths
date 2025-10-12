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

#include <cstddef>
#include <string_view>

#include "basic_N.hpp"
#include "basic_Q.hpp"
#include "basic_Z.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "hash.hpp"

template <jmaths::TMP::instance_of<jmaths::basic_N> N>
inline std::size_t std::hash<N>::operator()(const N & n) {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Hash function for arbitrary-precision unsigned integers
    //
    // This hash function treats the internal digit representation as a byte sequence
    // and hashes it using std::hash<std::string_view>. This provides:
    // - Consistent hashing: same value always produces same hash
    // - Avalanche effect: small changes in input cause large changes in output
    // - Good distribution: values are well-distributed across hash space
    //
    // Special case: empty (zero) numbers get a pre-computed hash value for efficiency.
    //
    // The digits are stored contiguously in memory, so we can safely reinterpret
    // them as a byte array without copying.

    if (n.digits_.empty()) {
        static const auto empty_hash = std::hash<std::string_view>{}(std::string_view{});
        return empty_hash;
    }

    return std::hash<std::string_view>{}(
        std::string_view(reinterpret_cast<const char *>(n.digits_.data()),
                         n.digits_.size() * N::base_int_type_size));
}

template <jmaths::TMP::instance_of<jmaths::basic_Z> Z>
inline std::size_t std::hash<Z>::operator()(const Z & z) {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Hash function for arbitrary-precision signed integers
    //
    // Combines the hash of the magnitude (unsigned part) with the sign bit.
    // The sign is incorporated by XORing with the magnitude hash, shifted by
    // a position derived from the first digit. This ensures:
    // - Positive and negative versions of the same magnitude have different hashes
    // - The hash is still well-distributed
    //
    // Formula: hash(magnitude) XOR (sign << (first_digit % bits_in_size_t))

    using hash_N = std::hash<typename Z::N>;

    return hash_N{}(z) ^ (static_cast<std::size_t>(z.sign_)
                          << (z.front_() % (sizeof(std::size_t) * jmaths::bits_in_byte)));
}

template <jmaths::TMP::instance_of<jmaths::basic_Q> Q>
inline std::size_t std::hash<Q>::operator()(const Q & q) {
    JMATHS_FUNCTION_TO_LOG;

    // ALGORITHM: Hash function for arbitrary-precision rational numbers
    //
    // Combines hashes of the numerator, denominator, and sign using XOR operations.
    // This provides a reasonably distributed hash while being efficient to compute.
    //
    // The formula:
    // hash(Q) = (hash(numerator) XOR hash(denominator)) XOR
    //           (sign << ((numerator_first XOR denominator_first) % bits_in_size_t))
    //
    // This ensures that fractions with the same value in reduced form will have
    // the same hash (assuming they're always stored in reduced form).

    using hash_N = std::hash<typename Q::N>;

    return (hash_N{}(q.num_) ^ hash_N{}(q.denom_)) ^
           (static_cast<std::size_t>(q.sign_) << ((q.num_.front_() ^ q.denom_.front_()) %
                                                  (sizeof(std::size_t) * jmaths::bits_in_byte)));
}
