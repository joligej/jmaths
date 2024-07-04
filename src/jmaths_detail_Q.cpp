#include <ostream>
#include <istream>
#include <string>
#include <utility>
#include <compare>

#include "headers/jmaths_constants_and_types.hpp"
#include "headers/jmaths_detail.hpp"
#include "headers/jmaths_Q.hpp"
#include "headers/jmaths_sign_type.hpp"
#include "headers/jmaths_N.hpp"
#include "headers/jmaths_error.hpp"

#include "headers/jmaths_def.hh"

namespace jmaths {

/**********************************************************/
// implementation functions

std::ostream & detail::opr_ins (std::ostream & os, const Q & q) {
    FUNCTION_TO_LOG;

    return os << (std::string)q;
}

std::istream & detail::opr_extr (std::istream & is, Q & q) {
    FUNCTION_TO_LOG;

    std::string num_str;
    is >> num_str;
    q = num_str;
    return is;
}

Q detail::opr_add (const Q & lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    using sign_type::positive, sign_type::negative;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
            first_product.opr_add_assign_(detail::opr_mult(lhs.denom_, rhs.num_));
            return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
        } else {
            N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
            N && second_product = detail::opr_mult(lhs.denom_, rhs.num_);

            const auto difference = detail::opr_comp(first_product, second_product);

            if (difference == 0) {
                return Q{};
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
            } else {
                second_product.opr_subtr_assign_(first_product);
                return Q(std::move(second_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
            }
        }
    } else {
        if (rhs.is_positive()) {
            N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
            N && second_product = detail::opr_mult(lhs.denom_, rhs.num_);

            const auto difference = detail::opr_comp(first_product, second_product);

            if (difference == 0) {
                return Q{};
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
            } else {
                second_product.opr_subtr_assign_(first_product);
                return Q(std::move(second_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
            }
        } else {
            N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
            first_product.opr_add_assign_(detail::opr_mult(lhs.denom_, rhs.num_));
            return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
        }
    }
}

Q detail::opr_subtr (const Q & lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    using sign_type::positive, sign_type::negative;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
            N && second_product = detail::opr_mult(lhs.denom_, rhs.num_);

            const auto difference = detail::opr_comp(first_product, second_product);

            if (difference == 0) {
                return Q{};
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
            } else {
                second_product.opr_subtr_assign_(first_product);
                return Q(std::move(second_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
            }
        } else {
            N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
            first_product.opr_add_assign_(detail::opr_mult(lhs.denom_, rhs.num_));
            return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
        }
    } else {
        if (rhs.is_positive()) {
            N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
            first_product.opr_add_assign_(detail::opr_mult(lhs.denom_, rhs.num_));
            return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
        } else {
            N && first_product = detail::opr_mult(lhs.num_, rhs.denom_);
            N && second_product = detail::opr_mult(lhs.denom_, rhs.num_);

            const auto difference = detail::opr_comp(first_product, second_product);

            if (difference == 0) {
                return Q{};
            } else if (difference > 0) {
                first_product.opr_subtr_assign_(second_product);
                return Q(std::move(first_product), detail::opr_mult(lhs.denom_, rhs.denom_), negative);
            } else {
                second_product.opr_subtr_assign_(first_product);
                return Q(std::move(second_product), detail::opr_mult(lhs.denom_, rhs.denom_), positive);
            }
        }
    }
}

Q detail::opr_mult (const Q & lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    N && numerator = lhs.num_ * rhs.num_;

    if (numerator.is_zero()) return Q{};

    return Q(std::move(numerator), lhs.denom_ * rhs.denom_, static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_));
}

Q detail::opr_div (const Q & lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    N && numerator = lhs.num_ * rhs.denom_;

    if (numerator.is_zero()) return Q{};

    return Q(std::move(numerator), lhs.denom_ * rhs.num_, static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_));
}

Q detail::opr_and (const Q & lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    N && denominator = lhs.denom_ & rhs.denom_;

    if (denominator.is_zero()) throw error::division_by_zero();

    N && numerator = lhs.num_ & rhs.num_;

    if (numerator.is_zero()) return Q{};

    return Q(std::move(numerator), std::move(denominator), static_cast<sign_type::sign_bool>(lhs.sign_ & rhs.sign_));
}

Q detail::opr_or (const Q & lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    N && numerator = lhs.num_ | rhs.num_;

    if (numerator.is_zero()) return Q{};

    return Q(std::move(numerator), lhs.denom_ | rhs.denom_, static_cast<sign_type::sign_bool>(lhs.sign_ | rhs.sign_));
}

Q detail::opr_xor (const Q & lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    N && denominator = lhs.denom_ ^ rhs.denom_;

    if (denominator.is_zero()) throw error::division_by_zero();

    N && numerator = lhs.num_ ^ rhs.num_;

    if (numerator.is_zero()) return Q{};

    return Q(std::move(numerator), std::move(denominator), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_));
}

bool detail::opr_eq (const Q & lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    return (lhs.sign_ == rhs.sign_ && lhs.num_ == rhs.num_ && lhs.denom_ == rhs.denom_);
}

std::strong_ordering detail::opr_comp (const Q & lhs, const Q & rhs) {
    FUNCTION_TO_LOG;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            return detail::opr_comp(lhs.num_ * rhs.denom_, rhs.num_ * lhs.denom_);
        } else {
            return std::strong_ordering::greater;
        }
    } else {
        if (rhs.is_positive()) {
            return std::strong_ordering::less;
        } else {
            return detail::opr_comp(rhs.num_ * lhs.denom_, lhs.num_ * rhs.denom_);
        }
    }
}

} // /namespace jmaths

#include "headers/jmaths_undef.hh"
