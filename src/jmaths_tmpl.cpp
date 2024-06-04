#include "../config/jmaths_def.cfg"

#define REPEAT(x) for (decltype(x) for_loop_repition_counter_ = 0; for_loop_repition_counter_ < x; ++for_loop_repition_counter_)

// comparison functions for N with integral types
namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
bool detail::opr_eq (const N & lhs, INT rhs) {
	if (rhs == 0) return lhs.is_zero();
	if (lhs.is_zero()) return false;
	
	if (lhs.digits_.size() * BASE_INT_SIZE - 1 > sizeof(rhs)) return false;
	
	if (lhs.digits_.size() * BASE_INT_SIZE < sizeof(rhs) && (rhs >> (BASE_INT_BITS * lhs.digits_.size())) != 0) return false;
	
	for (const auto & digit : lhs.digits_) {
		if (digit != (BASE_INT)rhs) return false;
		REPEAT(BASE_INT_SIZE) rhs >>= BITS_IN_BYTE;
	}
	
	return true;
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering detail::opr_comp (const N & lhs, INT rhs) {
	if (rhs == 0) return lhs.is_zero() ? std::strong_ordering::equal : std::strong_ordering::greater;
	if (lhs.is_zero()) return std::strong_ordering::less;
	
	if (lhs.digits_.size() * BASE_INT_SIZE - 1 > sizeof(rhs)) return std::strong_ordering::greater;
	
	if (lhs.digits_.size() * BASE_INT_SIZE < sizeof(rhs) && (rhs >> (BASE_INT_BITS * lhs.digits_.size())) != 0) return std::strong_ordering::less;
	
	size_t pos = 1;
	for (auto crit = lhs.digits_.crbegin(); crit != lhs.digits_.crend(); ++crit) {
		INT curr_byte = rhs;
		REPEAT(BASE_INT_SIZE * (lhs.digits_.size() - pos)) curr_byte >>= BITS_IN_BYTE;
		if (*crit < (BASE_INT)curr_byte) return std::strong_ordering::less;
		if (*crit > (BASE_INT)curr_byte) return std::strong_ordering::greater;
		
		++pos;
	}
	
	return std::strong_ordering::equal;
}

template <typename INT>
requires std::is_integral_v<INT>
bool operator == (const N & lhs, INT rhs) {
	return detail::opr_eq(lhs, rhs);
}

template <typename INT>
requires std::is_integral_v<INT>
bool operator == (INT lhs, const N & rhs) {
	return detail::opr_eq(rhs, lhs);
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering operator <=> (const N & lhs, INT rhs) {
	return detail::opr_comp(lhs, rhs);
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering operator <=> (INT lhs, const N & rhs) {
	return (0 <=> detail::opr_comp(rhs, lhs));
}

} // /namespace jmaths

// comparison functions for Z with integral types
namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
bool detail::opr_eq (const Z & lhs, INT rhs) {
    if (rhs < 0) {
        if (lhs.is_positive()) return false;
        return (lhs.abs() == -rhs);
    }
    
    return (lhs.abs() == rhs);
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering detail::opr_comp (const Z & lhs, INT rhs) {
    if (lhs.is_positive()) {
        if (rhs >= 0) {
            return (lhs.abs() <=> rhs);
        } else {
            return std::strong_ordering::greater;
        }
    } else {
        if (rhs >= 0) {
            return std::strong_ordering::less;
        } else {
            return (-rhs <=> lhs.abs());
        }
    }
}

template <typename INT>
requires std::is_integral_v<INT>
bool operator == (const Z & lhs, INT rhs) {
    return detail::opr_eq(lhs, rhs);
}
	
template <typename INT>
requires std::is_integral_v<INT>
bool operator == (INT lhs, const Z & rhs) {
    return detail::opr_eq(rhs, lhs);
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering operator <=> (const Z & lhs, INT rhs) {
    return detail::opr_comp(lhs, rhs);
}

template <typename INT>
requires std::is_integral_v<INT>
std::strong_ordering operator <=> (INT lhs, const Z & rhs) {
    return (0 <=> detail::opr_comp(rhs, lhs));
}

} // /namespace jmaths

// comparison functions for Q with floating point types
namespace jmaths {

template <typename FLOAT>
requires std::is_floating_point_v<FLOAT>
bool detail::opr_eq (const Q & lhs, FLOAT rhs) {
    return (lhs == Q(rhs));
}

template <typename FLOAT>
requires std::is_floating_point_v<FLOAT>
std::strong_ordering detail::opr_comp (const Q & lhs, FLOAT rhs) {    
    return (lhs <=> Q(rhs));
}

template <typename FLOAT>
requires std::is_floating_point_v<FLOAT>
bool operator == (const Q & lhs, FLOAT rhs) {
    return detail::opr_eq(lhs, rhs);
}

template <typename FLOAT>
requires std::is_floating_point_v<FLOAT>
bool operator == (FLOAT lhs, const Q & rhs) {
    return detail::opr_eq(rhs, lhs);
}

template <typename FLOAT>
requires std::is_floating_point_v<FLOAT>
std::strong_ordering operator <=> (const Q & lhs, FLOAT rhs) {
    return detail::opr_comp(lhs, rhs);
}

template <typename FLOAT>
requires std::is_floating_point_v<FLOAT>
std::strong_ordering operator <=> (FLOAT lhs, const Q & rhs) {
    return (0 <=> detail::opr_comp(rhs, lhs));
}

} // /namespace jmaths

// member function templates of N
namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
void N::opr_assign_ (INT rhs) {
	digits_.clear();
		
	digits_.reserve((size_t)((double)sizeof(INT)/BASE_INT_SIZE - MAX_RATIO + 1));
	
	// this assumes that sizeof(smallest type)/BASE_INT_SIZE >= MAX_RATIO
	REPEAT((size_t)((double)sizeof(INT)/BASE_INT_SIZE - MAX_RATIO)) {
		digits_.emplace_back((BASE_INT)rhs);
		REPEAT(BASE_INT_SIZE) rhs >>= BITS_IN_BYTE;
	}
	
	digits_.emplace_back((BASE_INT)rhs);
	
	remove_leading_zeroes_();
}

template <typename INT>
requires std::is_integral_v<INT>
N::N (INT num) {
	digits_.reserve((size_t)((double)sizeof(INT)/BASE_INT_SIZE - MAX_RATIO + 1));
	
	// this assumes that sizeof(smallest type)/BASE_INT_SIZE >= MAX_RATIO
	REPEAT((size_t)((double)sizeof(INT)/BASE_INT_SIZE - MAX_RATIO)) {
		digits_.emplace_back((BASE_INT)num);
		REPEAT(BASE_INT_SIZE) num >>= BITS_IN_BYTE;
	}
	
	digits_.emplace_back((BASE_INT)num);
	
	remove_leading_zeroes_();
}

template <typename INT>
requires std::is_integral_v<INT> && std::is_unsigned_v<INT>
std::optional<INT> N::fits_into() const {
	// !!! perhaps use std::numeric_limits<T>::max from <limits> for signed types ???
	
	/*if constexpr (std::is_signed_v<INT>) {
		//if (is_positive()) return N::fits_into<INT>();
		//else return std::nullopt;
		// !!! implement this perhaps ???
	}*/
	
	if (is_zero()) return 0;
	
	if constexpr (sizeof(INT) < BASE_INT_SIZE) {
		if ((INT)digits_.front() != digits_.front() || digits_.size() > 1) return std::nullopt;
		return digits_.front();
	} else {
		if (digits_.size() * BASE_INT_SIZE > sizeof(INT)) return std::nullopt;
		
		/*INT converted (digits_.back());
		for (size_t i = digits_.size() - 1; i --> 0;) {
			converted <<= BASE_INT_BITS;
		  	converted |= digits_[i];
		}*/
	
		INT converted (digits_.front());
	
		for (size_t i = 1; i < digits_.size(); ++i) {
			converted |= ((INT)digits_[i] << (BASE_INT_BITS * i));
		}
	
		return converted;	
	}
}

template <typename INT>
requires std::is_integral_v<INT>
N & N::operator = (INT rhs) {
	opr_assign_(rhs);
	return *this;
}

} // /namespace jmaths

// functions for N::bit_reference_base_
namespace jmaths {

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
N::bit_reference_base_<T>::bit_reference_base_ (T & num, BIT_TYPE pos) : num_(num), pos_(pos) {}

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
N::bit_reference_base_<T>::operator bool () const {
	return num_.bit_(pos_);
}

template <typename T>
requires std::is_same_v<N, T> || std::is_same_v<const N, T>
N::bit_reference_base_<T>::operator int () const {
	return operator bool();
}

} // /namespace jmaths

// member function templates of sign_type
namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
sign_type::sign_type (INT & num) : sign_(handle_int_(num)) {}

template <typename INT>
requires std::is_integral_v<INT>
sign_type::sign_bool sign_type::handle_int_ (INT & num) {
	if (num < 0) {
		num *= -1;
		return negative;
	} else {
		return positive;
	}
}

template <typename BOOL>
requires std::is_convertible_v<BOOL, std::underlying_type_t<sign_type::sign_bool>>
void sign_type::set_sign_ (BOOL val) {
	sign_ = static_cast<sign_bool>(val);
}
	
} // /namespace jmaths

// member function templates of Z
namespace jmaths {

template <typename INT>
requires std::is_integral_v<INT>
Z::Z (INT num) : sign_type(num), N(num) {}

template <typename INT>
requires std::is_integral_v<INT>
std::optional<INT> Z::fits_into() const {
	if constexpr (std::is_unsigned_v<INT>) {
		if (is_positive()) return N::fits_into<INT>();
		else return std::nullopt;
	}
	
	if (is_zero()) return 0;
	
	if constexpr (sizeof(INT) < BASE_INT_SIZE) {
		if (digits_.front() > (is_negative() ? -std::numeric_limits<INT>::min() : std::numeric_limits<INT>::max()) || digits_.size() > 1) return std::nullopt;
		return (is_negative() ? -digits_.front() : digits_.front());
	} else {
		if (digits_.size() * BASE_INT_SIZE > sizeof(INT)) return std::nullopt;
		
		if (detail::opr_comp(*this, (is_negative() ? -std::numeric_limits<INT>::min() : std::numeric_limits<INT>::max())) > 0) return std::nullopt;
		
		/*INT converted (digits_.back());
		for (size_t i = digits_.size() - 1; i --> 0;) {
			converted <<= BASE_INT_BITS;
		  	converted |= digits_[i];
		}*/
	
		INT converted (digits_.front());
	
		for (size_t i = 1; i < digits_.size(); ++i) {
			converted += (INT)((std::make_unsigned_t<INT>)digits_[i] << (BASE_INT_BITS * i));
		}
		
		if (is_negative()) -converted;
		else return converted;	
	}
}

template <typename INT>
requires std::is_integral_v<INT>
Z & Z::operator = (INT rhs) {
	set_sign_(sign_type::handle_int_(rhs));
	N::opr_assign_(rhs);
	return *this;
}
	
} // /namespace jmaths

// member function templates of Q
namespace jmaths {

template <typename FLOAT>
requires std::is_floating_point_v<FLOAT>
std::tuple<N, N, sign_type::sign_bool> Q::handle_float_ (FLOAT num) {
	int exponent;
    FLOAT significant_part = std::frexp(num, &exponent);
    significant_part = std::scalbn(significant_part, std::numeric_limits<FLOAT>::digits);
    exponent -= std::numeric_limits<FLOAT>::digits;
    
    auto numerator = std::llrint(significant_part);
    auto denominator = std::llrint(std::exp2(-exponent));
    
    const auto sign = (numerator < 0) ? sign_type::negative : sign_type::positive;
    
    if (sign == sign_type::negative) numerator *= -1;
    
    // it is assumed here that exponent is negative at this point

	return {(std::make_unsigned_t<decltype(numerator)>)numerator, (std::make_unsigned_t<decltype(denominator)>)denominator, sign};
}

template <typename FLOAT>
requires std::is_floating_point_v<FLOAT>
Q::Q (FLOAT num) : Q(handle_float_(num)) {}

template <typename FLOAT>
requires std::is_floating_point_v<FLOAT> && std::numeric_limits<FLOAT>::is_iec559
std::optional<FLOAT> Q::fits_into() const {
	typedef std::numeric_limits<FLOAT> nlf;

	constexpr bool is_allowed_type = std::is_same_v<FLOAT, float> || std::is_same_v<FLOAT, double>;
	static_assert(is_allowed_type, "Template type parameter is not one of the allowed types: float and double.");
	static_assert(nlf::radix == 2, "The radix of the floating point type is currently not supported. Please make sure it is equal to 2.");

	#if defined (NATIVELY_BIG_ENDIAN) && defined (NATIVELY_LITTLE_ENDIAN)
			#error "Both little and big endian. Unclear how to implement floating point manipulation."
	#endif

	struct float_sizes {
		enum : BIT_TYPE {
			sign = 1,
			exponent = 8,
			mantissa = 23
		};
	};

	struct double_sizes {
		enum : BIT_TYPE {
			sign = 1,
			exponent = 11,
			mantissa = 52
		};
	};

	union float_access {
		std::uint32_t float_size[1];
		struct {
		#ifdef NATIVELY_BIG_ENDIAN
			std::uint32_t sign : float_sizes::sign;
			std::uint32_t exponent : float_sizes::exponent;
			std::uint32_t mantissa : float_sizes::mantissa;
		#else
			std::uint32_t mantissa : float_sizes::mantissa;
			std::uint32_t exponent : float_sizes::exponent;
			std::uint32_t sign : float_sizes::sign;
		#endif
		} fields;
	};

	union double_access {
		std::uint64_t double_size[1];
		struct {
		#ifdef NATIVELY_BIG_ENDIAN
			std::uint64_t sign : double_sizes::sign;
			std::uint64_t exponent : double_sizes::exponent;
			std::uint64_t mantissa : double_sizes::mantissa;
		#else
			std::uint64_t mantissa : double_sizes::mantissa;
			std::uint64_t exponent : double_sizes::exponent;
			std::uint64_t sign : double_sizes::sign;
		#endif
		} fields;
	};

	/*union long_double_access {
		std::uint64_t long_double_size[2];
		struct {
		#ifdef NATIVELY_BIG_ENDIAN
			std::uint64_t sign : 1;
			std::uint64_t exponent : 15;
			std::uint64_t mantissa : 112;
		#else
			std::uint64_t mantissa : 112;
			std::uint64_t exponent : 15;
			std::uint64_t sign : 1;
		#endif
		} fields;
	};*/

	typedef std::conditional_t<std::is_same_v<FLOAT, float>, float_access, std::enable_if_t<std::is_same_v<FLOAT, double>, double_access>> access_type;
	typedef std::conditional_t<std::is_same_v<FLOAT, float>, float_sizes, std::enable_if_t<std::is_same_v<FLOAT, double>, double_sizes>> sizes_type;
	
	static_assert(sizeof(float_access) == sizeof(float) && sizeof(float_access) == sizeof(std::uint32_t[1]), "There seems to be a problem with the padding bits for type: float_acess.");
	static_assert(sizeof(double_access) == sizeof(double) && sizeof(double_access) == sizeof(std::uint64_t[1]), "There seems to be a problem with the padding bits for type: double_access.");
	//static_assert(sizeof(long_double_access) == sizeof(long double) && sizeof(long_double_access) == sizeof(std::uint64_t[2]), "There seems to be a problem with the padding bits for type: long_double_access.");

	if (num_.is_zero()) return 0;
	if (is_one()) return 1;
	if (is_neg_one()) return -1;

	// check if radix == 2 !!! ...

	if (num_ > denom_) {
		const auto div_pair = detail::opr_div(num_, denom_);

		if (div_pair.first.bits() > nlf::max_exponent) { // maybe the right checking condition ??? !!!
			if constexpr (nlf::has_infinity) {
				return nlf::infinity();
			} else {
				return std::nullopt;
			}
		} else if (div_pair.first.bits() == nlf::max_exponent) {
			FLOAT converted{};
			access_type * const converted_help = &converted;

			if constexpr (sizes_type::mantissa <= BASE_INT_BITS) {
				converted_help->fields.mantissa = div_pair.first.digits_.back() >> (BASE_INT_BITS - sizes_type::mantissa);
			} else {
				const auto & last_digit = div_pair.first.digits_.back();
				converted_help->fields.mantissa = 0;
				for (BIT_TYPE i = 0; i < sizes_type::mantjssa; ++i) {
					const std::size_t j = i / BASE_INT_BITS;
					const BIT_TYPE k = i % BASE_INT_BITS;

					typedef decltype(converted_help->fields.mantjssa) mantissa_type;
					static constexpr auto mask = (mantissa_type)1 << (sizes_type::mantissa - 1);

					converted_help->fields.mantissa |= (((mantissa_type)(*(&last_digit - j) << k) & mask) >> i);
				}
			}

			converted_help->fields.exponent = div_pair.first.bits() - 1;

			converted_help->fields.sign = is_negative();
			
			return converted;
		} else {
			
		}

	} else {

	}

}

template <>
std::optional<long double> Q::fits_into() const;

template <typename FLOAT>
requires std::is_floating_point_v<FLOAT>
Q & Q::operator = (FLOAT rhs) {
	auto fraction_info = handle_float_(rhs);
	num_ = std::move(std::get<0>(fraction_info));
	denom_ = std::move(std::get<1>(fraction_info));
	set_sign_(std::get<2>(fraction_info));

	return *this;
}

} // /namespace jmaths

#undef REPEAT

#include "../config/jmaths_undef.cfg"
