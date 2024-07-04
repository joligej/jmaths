#include <ostream>
#include <istream>
#include <string>
#include <utility>
#include <compare>

#include "headers/jmaths_constants_and_types.hpp"
#include "headers/jmaths_detail.hpp"
#include "headers/jmaths_Z.hpp"
#include "headers/jmaths_sign_type.hpp"
#include "headers/jmaths_N.hpp"

#include "headers/jmaths_def.hh"

namespace jmaths {

/**********************************************************/
// implementation functions

std::ostream & detail::opr_ins (std::ostream & os, const Z & z) {
    FUNCTION_TO_LOG;

    if (z.is_negative()) return os << negative_sign << z.abs();
    else return os << z.abs();
}

std::istream & detail::opr_extr (std::istream & is, Z & z) {
    FUNCTION_TO_LOG;

    std::string num_str;
    is >> num_str;
    z = num_str;
    return is;
}

Z detail::opr_add (const Z & lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    using sign_type::positive, sign_type::negative;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            return Z(detail::opr_add(lhs.abs(), rhs.abs()), positive);
        } else {
            const auto difference = detail::opr_comp(lhs.abs(), rhs.abs());

            if (difference == 0) {
                return Z{};
            } else if (difference > 0) {
                return Z(detail::opr_subtr(lhs.abs(), rhs.abs()), positive);
            } else {
                return Z(detail::opr_subtr(rhs.abs(), lhs.abs()), negative);
            }

        }
    } else {
        if (rhs.is_positive()) {
            const auto difference = detail::opr_comp(lhs.abs(), rhs.abs());

            if (difference == 0) {
                return Z{};
            } else if (difference > 0) {
                return Z(detail::opr_subtr(lhs.abs(), rhs.abs()), negative);
            } else {
                return Z(detail::opr_subtr(rhs.abs(), lhs.abs()), positive);
            }

        } else {
            return Z(detail::opr_add(lhs.abs(), rhs.abs()), negative);
        }
    }
}

Z detail::opr_subtr (const Z & lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    using sign_type::positive, sign_type::negative;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            const auto difference = detail::opr_comp(lhs.abs(), rhs.abs());

            if (difference == 0) {
                return Z{};
            } else if (difference > 0) {
                return Z(detail::opr_subtr(lhs.abs(), rhs.abs()), positive);
            } else {
                return Z(detail::opr_subtr(rhs.abs(), lhs.abs()), negative);
            }
        } else {
            return Z(detail::opr_add(lhs.abs(), rhs.abs()), positive);
        }
    } else {
        if (rhs.is_positive()) {
            return Z(detail::opr_add(lhs.abs(), rhs.abs()), negative);
        } else {
            const auto difference = detail::opr_comp(lhs.abs(), rhs.abs());

            if (difference == 0) {
                return Z{};
            } else if (difference > 0) {
                return Z(detail::opr_subtr(lhs.abs(), rhs.abs()), negative);
            } else {
                return Z(detail::opr_subtr(rhs.abs(), lhs.abs()), positive);
            }
        }
    }
}

Z detail::opr_mult (const Z & lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    N && product = lhs.abs() * rhs.abs();

    if (product.is_zero()) return Z{};

    return Z(std::move(product), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_));

}

std::pair<Z, Z> detail::opr_div (const Z & lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    auto && quotient = lhs.abs() / rhs.abs();

    if (quotient.first.is_zero()) {
        if (quotient.second.is_zero()) {
            return {Z{}, Z{}};
        } else {
            return {Z{}, Z(std::move(quotient.second), lhs.sign_)};
        }
    } else {
        if (quotient.second.is_zero()) {
            return {Z(std::move(quotient.first), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)), Z{}};
        } else {
            return {Z(std::move(quotient.first), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_)), Z(std::move(quotient.second), lhs.sign_)};
        }
    }
}

Z detail::opr_and (const Z & lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    N && and_result = lhs.abs() & rhs.abs();

    if (and_result.is_zero()) return Z{};

    return Z(std::move(and_result), static_cast<sign_type::sign_bool>(lhs.sign_ & rhs.sign_));
}

Z detail::opr_or (const Z & lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    N && or_result = lhs.abs() | rhs.abs();

    if (or_result.is_zero()) return Z{};

    return Z(std::move(or_result), static_cast<sign_type::sign_bool>(lhs.sign_ | rhs.sign_));
}

Z detail::opr_xor (const Z & lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    N && xor_result = lhs.abs() ^ rhs.abs();

    if (xor_result.is_zero()) return Z{};

    return Z(std::move(xor_result), static_cast<sign_type::sign_bool>(lhs.sign_ ^ rhs.sign_));
}

bool detail::opr_eq (const Z & lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    return (lhs.sign_ == rhs.sign_ && lhs.abs() == rhs.abs());
}

std::strong_ordering detail::opr_comp (const Z & lhs, const Z & rhs) {
    FUNCTION_TO_LOG;

    if (lhs.is_positive()) {
        if (rhs.is_positive()) {
            return detail::opr_comp(lhs.abs(), rhs.abs());
        } else {
            return std::strong_ordering::greater;
        }
    } else {
        if (rhs.is_positive()) {
            return std::strong_ordering::less;
        } else {
            return detail::opr_comp(rhs.abs(), lhs.abs());
        }
    }
}

} // /namespace jmaths

#include "headers/jmaths_undef.hh"
