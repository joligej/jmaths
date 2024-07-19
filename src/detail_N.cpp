// The jmaths library for C++
// Copyright (C) 2024  Jasper de Smaele

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

#include <algorithm>
#include <bit>
#include <cassert>
#include <compare>
#include <cstddef>
#include <istream>
#include <iterator>  // only for std::back_inserter
#include <limits>
#include <ostream>
#include <string>
#include <utility>

#include "N.hpp"
#include "constants_and_types.hpp"
#include "def.hh"
#include "detail.hpp"

namespace jmaths {

/**********************************************************/
// implementation functions

std::ostream & detail::opr_ins(std::ostream & os, const N & n) {
    FUNCTION_TO_LOG;

    return os << n.to_str();
}

std::istream & detail::opr_extr(std::istream & is, N & n) {
    FUNCTION_TO_LOG;

    std::string num_str;
    is >> num_str;
    n.opr_assign_(num_str);

    assert(n == N(num_str));

    return is;
}

N detail::opr_add(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    // check for additive identity
    if (lhs.is_zero()) return rhs;
    if (rhs.is_zero()) return lhs;

    const N * longest;
    const N * shortest;

    if (lhs.digits_.size() < rhs.digits_.size()) {
        longest = &rhs;
        shortest = &lhs;
    } else {
        longest = &lhs;
        shortest = &rhs;
    }

    assert(longest && shortest);

    N sum;
    sum.digits_.reserve(longest->digits_.size() + 1);

    bool carry = false;

    std::size_t i = 0;

    for (; i < shortest->digits_.size(); ++i) {
        // checks if lhs + rhs + carry < base
        const bool next_carry =
            !(lhs.digits_[i] < (carry ? max_digit : radix) - rhs.digits_[i]);

        sum.digits_.emplace_back(lhs.digits_[i] + rhs.digits_[i] + carry);
        carry = next_carry;
    }

    for (; carry && i < longest->digits_.size(); ++i) {
        const bool next_carry =
            !(longest->digits_[i] < (carry ? max_digit : radix));

        sum.digits_.emplace_back(longest->digits_[i] + carry);
        carry = next_carry;
    }

    for (; i < longest->digits_.size(); ++i) {
        sum.digits_.emplace_back(longest->digits_[i]);
    }

    if (carry) sum.digits_.emplace_back(1);

    assert(sum.is_zero() || sum.digits_.back() != 0);

    return sum;
}

N detail::opr_subtr(N lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    // this functions assumes that lhs >= rhs and for effiency reasons should
    // only be called when lhs > rhs

    // check for additive identity
    if (rhs.is_zero()) return lhs;

    N difference;
    difference.digits_.reserve(lhs.digits_.size());

    std::size_t i = 0;

    for (; i < rhs.digits_.size(); ++i) {
        if (lhs.digits_[i] < rhs.digits_[i]) {
            for (std::size_t j = i + 1; j < lhs.digits_.size(); ++j) {
                if ((lhs.digits_[j])-- > 0) break;
            }
        }

        difference.digits_.emplace_back(lhs.digits_[i] - rhs.digits_[i]);
    }

    for (; i < lhs.digits_.size(); ++i) {
        difference.digits_.emplace_back(lhs.digits_[i]);
    }

    difference.remove_leading_zeroes_();

    assert(difference.is_zero() || difference.digits_.back() != 0);

    return difference;
}

N detail::opr_mult(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

#ifndef KARATSUBA
    #error "KARATSUBA is not defined, please define it when compiling!"
#endif

#if KARATSUBA

    #error \
        "The KARATSUBA algorithm is still a work in progress, please define KARATSUBA as 0"

    // check for multiplicative identity
    if (lhs.is_one()) return rhs;
    if (rhs.is_one()) return lhs;

    // check for multiplicative zero
    if (lhs.is_zero() || rhs.is_zero()) return N{};

    const N * longest;
    const N * shortest;

    if (lhs.digits_.size() < rhs.digits_.size()) {
        longest = &rhs;
        shortest = &lhs;
    } else {
        longest = &lhs;
        shortest = &rhs;
    }

    assert(longest && shortest);

    const std::size_t longest_digits_count = longest->digits_.size();
    const std::size_t longest_digits_left = longest_digits_count / 2;
    const std::size_t longest_digits_right = (longest_digits_count / 2) + 0.999;

    N longest_num_r, longest_num_l;

    {
        longest_num_r.digits_.reserve(longest_digits_right);
        longest_num_l.digits_.reserve(longest_digits_left);

        auto cit = longest->digits_.cbegin();

        for (std::size_t i = 0; i < longest_digits_right; ++cit, ++i) {
            longest_num_r.digits_.emplace_back(*cit);
        }

        for (std::size_t i = 0; i < longest_digits_left; ++cit, ++i) {
            assert((cit != longest->digits_.cend()));
            longest_num_l.digits_.emplace_back(*cit);
        }
    }

    N shortest_num_r, shortest_num_l;

    {
        shortest_num_r.digits_.reserve(longest_digits_right);

        auto cit = shortest->digits_.cbegin();

        for (std::size_t i = 0;
             i < longest_digits_right && cit != shortest->digits_.cend();
             ++cit, ++i) {
            shortest_num_r.digits_.emplace_back(*cit);
        }

        if (cit != shortest->digits_.cend()) {
            shortest_num_l.digits_.reserve(longest_digits_left);
        }

        for (std::size_t i = 0;
             i < longest_digits_left && cit != shortest->digits_.cend();
             ++cit, ++i) {
            shortest_num_l.digits_.emplace_back(*cit);
        }
    }

    const auto multiply =
        [](const N & lhs, const N & rhs, std::size_t exp) -> N {
        // check for multiplicative identity
        if (lhs.is_one()) return rhs;
        if (rhs.is_one()) return lhs;

        // check for multiplicative zero
        if (lhs.is_zero() || rhs.is_zero()) return N{};

        N product;

        // max overhead would be base_int_size
        product.digits_.reserve(lhs.digits_.size() + rhs.digits_.size() + exp);

        for (std::size_t i = 0; i < lhs.digits_.size(); ++i) {
            N temp1;
            temp1.digits_.reserve(i + rhs.digits_.size() + 1);
            temp1.digits_.insert(temp1.digits_.begin(), i, 0);
            base_int carry = 0;
            for (std::size_t j = 0; j < rhs.digits_.size(); ++j) {
                const base_int_big temp2 =
                    (base_int_big)lhs.digits_[i] * (base_int_big)rhs.digits_[j];
                const base_int temp3 = (base_int)temp2;
                temp1.digits_.emplace_back(carry + temp3);
                const base_int temp_carry =
                    ((base_int_big)carry + (base_int_big)temp3) >>
                    base_int_bits;
                carry = (temp2 >> base_int_bits) + temp_carry;
            }

            if (carry) temp1.digits_.emplace_back(carry);
            product.opr_add_assign_(temp1);
        }

        return product;
    };

    const auto insert_front = [](const N & n, std::size_t exp) -> N {
        N exponentiated;
        exponentiated.digits_.reserve(n.digits_.size() + exp);
        exponentiated.digits_.insert(exponentiated.digits_.begin(), exp, 0);
        std::copy(n.digits_.begin(),
                  n.digits_.end(),
                  std::back_inserter(exponentiated.digits_));
        return exponentiated;
    };

    const auto XlYl =
        multiply(shortest_num_l, longest_num_l, 2 * longest_digits_right);
    const auto XrYr = multiply(shortest_num_r,
                               longest_num_r,
                               0);  // should this really be 0 ???

    return insert_front(XlYl, 2 * longest_digits_right) +
           insert_front(((multiply(shortest_num_l + shortest_num_r,
                                   longest_num_l + longest_num_r,
                                   0) -
                          XlYl) -
                         XrYr),
                        longest_digits_right) +
           XrYr;

#else

    // check for multiplicative identity
    if (lhs.is_one()) return rhs;
    if (rhs.is_one()) return lhs;

    // check for multiplicative zero
    if (lhs.is_zero() || rhs.is_zero()) return N{};

    N product;

    // max overhead would be base_int_size
    product.digits_.reserve(lhs.digits_.size() + rhs.digits_.size());

    for (std::size_t i = 0; i < lhs.digits_.size(); ++i) {
        N temp1;
        temp1.digits_.reserve(i + rhs.digits_.size() + 1);
        temp1.digits_.insert(temp1.digits_.begin(), i, 0);
        base_int carry = 0;
        for (std::size_t j = 0; j < rhs.digits_.size(); ++j) {
            const base_int_big temp2 =
                (base_int_big)lhs.digits_[i] * (base_int_big)rhs.digits_[j];
            const base_int temp3 = (base_int)temp2;
            temp1.digits_.emplace_back(carry + temp3);
            const base_int temp_carry =
                ((base_int_big)carry + (base_int_big)temp3) >> base_int_bits;
            carry = (temp2 >> base_int_bits) + temp_carry;
        }

        if (carry) temp1.digits_.emplace_back(carry);
        product.opr_add_assign_(temp1);
    }

    return product;

#endif
}

std::pair<N, N> detail::opr_div(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    if (lhs.is_zero()) return {N{}, N{}};

    // check if lhs == rhs
    if (opr_eq(lhs, rhs)) {
        static const N one(1);
        return {one, N{}};
    }

    std::pair<N, N> result;

    auto & [q, r] = result;

    if (const auto max_size = lhs.digits_.size() - rhs.digits_.size() + 1;
        lhs.digits_.size() + 1 >= rhs.digits_.size()) {
        // rhs.digits_.size() is always >= 1 so even if lhs.digits_.size() + 1
        // overflows to 0 no errors shall occur
#ifndef NDEBUG
        using nlim = std::numeric_limits<decltype(lhs.digits_.size())>;
#endif

        assert(lhs.digits_.size() == nlim::max() ? 1 <= rhs.digits_.size() : lhs.digits_.size() >= rhs.digits_.size() ? lhs.digits_.size() - rhs.digits_.size() <= nlim::max() - 1 : true /*-(rhs.digits_.size() - lhs.digits_.size()) <= nlim::max() - 1*/); // assert for overflow
        assert(lhs.digits_.size() == nlim::max() ?
                   true :
                   lhs.digits_.size() + 1 >=
                       rhs.digits_.size());  // assert for overflow to 0

        q.digits_.reserve(max_size);
    }

    r.digits_.reserve(rhs.digits_.size());

    for (bit_type i = lhs.digits_.size() * base_int_bits -
                      std::countl_zero(lhs.digits_.back());
         i-- > 0;) {
        r.opr_bitshift_l_assign_(1);
        r.bit_(0, lhs.bit_(i));
        if (opr_comp(r, rhs) >= 0) {
            r.opr_subtr_assign_(rhs);
            q.bit_(i, 1);
        }
    }

    assert(q * rhs + r == lhs);

    return result;
}

N detail::opr_and(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    if (lhs.is_zero() || rhs.is_zero()) return N{};

    const N & shortest = (lhs.digits_.size() < rhs.digits_.size() ? lhs : rhs);

    N and_result;
    and_result.digits_.reserve(shortest.digits_.size());

    for (std::size_t i = 0; i < shortest.digits_.size(); ++i) {
        and_result.digits_.emplace_back(lhs.digits_[i] & rhs.digits_[i]);
    }

    and_result.remove_leading_zeroes_();

    return and_result;
}

N detail::opr_or(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    if (lhs.is_zero()) return rhs;
    if (rhs.is_zero()) return lhs;

    const N * longest;
    const N * shortest;

    if (lhs.digits_.size() < rhs.digits_.size()) {
        longest = &rhs;
        shortest = &lhs;
    } else {
        longest = &lhs;
        shortest = &rhs;
    }

    N or_result;
    or_result.digits_.reserve(longest->digits_.size());

    std::size_t i = 0;

    for (; i < shortest->digits_.size(); ++i) {
        or_result.digits_.emplace_back(lhs.digits_[i] | rhs.digits_[i]);
    }

    for (; i < longest->digits_.size(); ++i) {
        or_result.digits_.emplace_back(longest->digits_[i]);
    }

    return or_result;
}

N detail::opr_xor(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    if (lhs.is_zero()) return rhs;
    if (rhs.is_zero()) return lhs;

    const N * longest;
    const N * shortest;

    if (lhs.digits_.size() < rhs.digits_.size()) {
        longest = &rhs;
        shortest = &lhs;
    } else {
        longest = &lhs;
        shortest = &rhs;
    }

    N xor_result;
    xor_result.digits_.reserve(longest->digits_.size());

    std::size_t i = 0;

    for (; i < shortest->digits_.size(); ++i) {
        xor_result.digits_.emplace_back(lhs.digits_[i] ^ rhs.digits_[i]);
    }

    for (; i < longest->digits_.size(); ++i) {
        xor_result.digits_.emplace_back(longest->digits_[i]);
    }

    xor_result.remove_leading_zeroes_();

    return xor_result;
}

bool detail::opr_eq(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    return (lhs.digits_ == rhs.digits_);
}

std::strong_ordering detail::opr_comp(const N & lhs, const N & rhs) {
    FUNCTION_TO_LOG;

    if (lhs.digits_.size() < rhs.digits_.size())
        return std::strong_ordering::less;
    if (lhs.digits_.size() > rhs.digits_.size())
        return std::strong_ordering::greater;

    for (auto crit_lhs = lhs.digits_.crbegin(),
              crit_rhs = rhs.digits_.crbegin();
         crit_lhs != lhs.digits_.crend();
         ++crit_lhs, ++crit_rhs) {
        if (*crit_lhs < *crit_rhs) return std::strong_ordering::less;
        if (*crit_lhs > *crit_rhs) return std::strong_ordering::greater;
    }

    return std::strong_ordering::equal;
}

}  // namespace jmaths

#include "undef.hh"
