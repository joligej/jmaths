#include <bit>
#include <sstream>

#include "jmaths.hpp"

#include "jmaths_def.cfg"

namespace {
	
static constexpr BASE_INT base_converter (char c) noexcept; // convert char to number for base >= 2 and <= 64

static constexpr BASE_INT base_converter (char c) noexcept {
	if (c >= '0' && c <= '9') return (c - '0');
	if (c >= 'A' && c <= 'Z') return (c - 'A' + 10);
	if (c >= 'a' && c <= 'z') return (c - 'a' + 10 + 26);
	if (c == '+') return 62;
	return 63;
}
	
} // /anonymous namespace

namespace jmaths {
	
/* ******************************************************** */
// implementation functions
	
std::ostream & detail::opr_ins (std::ostream & os, const N & n) {
	return os << (std::string)n;
}

std::istream & detail::opr_extr (std::istream & is, N & n) {
	std::string num_str;
	is >> num_str;
	n.opr_assign_(num_str);
	return is;
}

N detail::opr_add (const N & lhs, const N & rhs) {
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
	
	N sum;
	sum.digits_.reserve(longest->digits_.size() + 1);
	
	bool carry = false;
	
	size_t i = 0;
	
	for (; i < shortest->digits_.size(); ++i) {	
		// checks if lhs + rhs + carry < base
		const bool next_carry = !(lhs.digits_[i] < (carry ? MAX_DIGIT : BASE) - rhs.digits_[i]);
		
		sum.digits_.emplace_back(lhs.digits_[i] + rhs.digits_[i] + carry);
		carry = next_carry;
	}
	
	for (; carry && i < longest->digits_.size(); ++i) {
		const bool next_carry = !(longest->digits_[i] < (carry ? MAX_DIGIT : BASE));
	
		sum.digits_.emplace_back(longest->digits_[i] + carry);
		carry = next_carry;
	}
	
	for (; i < longest->digits_.size(); ++i) {
		sum.digits_.emplace_back(longest->digits_[i]);
	}
	
	if (carry) sum.digits_.emplace_back(1);
	
	return sum;
}

N detail::opr_subtr (N lhs, const N & rhs) {
	// this functions assumes that lhs >= rhs and for effiency reason should only be called when lhs > rhs
	
	// check for additive identity
	if (rhs.is_zero()) return lhs;
	
	N difference;
	difference.digits_.reserve(lhs.digits_.size());
	
	size_t i = 0;
	
	for (; i < rhs.digits_.size(); ++i) {
		if (lhs.digits_[i] < rhs.digits_[i]) {
			for (size_t j = i + 1; j < lhs.digits_.size(); ++j) {
				if ((lhs.digits_[j])-- > 0) break;
			}
		}
		
		difference.digits_.emplace_back(lhs.digits_[i] - rhs.digits_[i]);
	}
	
	for (; i < lhs.digits_.size(); ++i) {
		difference.digits_.emplace_back(lhs.digits_[i]);
	}
	
	difference.remove_leading_zeroes_();
	
	return difference;
}

N detail::opr_mult (const N & lhs, const N & rhs) {
	// check for multiplicative identity
	if (lhs.is_one()) return rhs;
	if (rhs.is_one()) return lhs;
	
	// check for multiplicative zero
	if (lhs.is_zero() || rhs.is_zero()) return N();
	
	N product;
	
	// max overhead would be BASE_INT_SIZE
	product.digits_.reserve(lhs.digits_.size() + rhs.digits_.size());
	
	for (size_t i = 0; i < lhs.digits_.size(); ++i) {
		N temp1;
		temp1.digits_.reserve(i + rhs.digits_.size() + 1);
		temp1.digits_.insert(temp1.digits_.begin(), i, 0);
		BASE_INT carry = 0;
		for (size_t j = 0; j < rhs.digits_.size(); ++j) {
			const BASE_INT_BIG temp2 = (BASE_INT_BIG)lhs.digits_[i] * (BASE_INT_BIG)rhs.digits_[j];
			const BASE_INT temp3 = (BASE_INT)temp2;
			temp1.digits_.emplace_back(carry + temp3);
			const BASE_INT temp_carry = ((BASE_INT_BIG)carry + (BASE_INT_BIG)temp3) >> BASE_INT_BITS;
			carry = (temp2 >> BASE_INT_BITS) + temp_carry;
		}
		
		if (carry) temp1.digits_.emplace_back(carry);
		product.opr_add_assign_(temp1);
	}
	
	return product;
}

std::pair<N, N> detail::opr_div (const N & lhs, const N & rhs) {
	if (lhs.is_zero()) return {N(), N()};
	
	// check if lhs == rhs
	if (opr_eq(lhs, rhs)) return {N(1), N()};
	
	N q, r;
	
	if (const auto max_size = lhs.digits_.size() - rhs.digits_.size() + 1; lhs.digits_.size() + 1 >= rhs.digits_.size()) {
		// rhs.digits_.size() is always >= 1 so even if lhs.digits_.size() + 1 overflows to 0 no errors shall occur
		q.digits_.reserve(max_size);
	}
	
	r.digits_.reserve(rhs.digits_.size());
	
	for (BIT_TYPE i = lhs.digits_.size() * BASE_INT_BITS; i --> 0;) {
		r.opr_bitshift_l_assign_(1);
		r.bit_(0, lhs.bit_(i));
		if (opr_comp(r, rhs) >= 0) {
			r.opr_subtr_assign_(rhs);
			q.bit_(i, 1);
		}
	}
	
	return {q, r};
}

N detail::opr_and (const N & lhs, const N & rhs) {
	if (lhs.is_zero() || rhs.is_zero()) return N();
	
	const N & shortest = (lhs.digits_.size() < rhs.digits_.size() ? lhs : rhs);
	
	N and_result;
	and_result.digits_.reserve(shortest.digits_.size());
	
	for (size_t i = 0; i < shortest.digits_.size(); ++i) {
		and_result.digits_.emplace_back(lhs.digits_[i] & rhs.digits_[i]);
	}
	
	and_result.remove_leading_zeroes_();
	
	return and_result;
}

N detail::opr_or (const N & lhs, const N & rhs) {
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
	
	size_t i = 0;
	
	for (; i < shortest->digits_.size(); ++i) {
		or_result.digits_.emplace_back(lhs.digits_[i] | rhs.digits_[i]);
	}
	
	for (; i < longest->digits_.size(); ++i) {
		or_result.digits_.emplace_back(longest->digits_[i]);
	}
	
	return or_result;
}

N detail::opr_xor (const N & lhs, const N & rhs) {
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
	
	size_t i = 0;
	
	for (; i < shortest->digits_.size(); ++i) {
		xor_result.digits_.emplace_back(lhs.digits_[i] ^ rhs.digits_[i]);
	}
	
	for (; i < longest->digits_.size(); ++i) {
		xor_result.digits_.emplace_back(longest->digits_[i]);
	}
	
	xor_result.remove_leading_zeroes_();
	
	return xor_result;
}

bool detail::opr_eq (const N & lhs, const N & rhs) {
	return (lhs.digits_ == rhs.digits_);
}

std::strong_ordering detail::opr_comp (const N & lhs, const N & rhs) {
	if (lhs.digits_.size() < rhs.digits_.size()) return std::strong_ordering::less;
	if (lhs.digits_.size() > rhs.digits_.size()) return std::strong_ordering::greater;
	
	for (auto crit_lhs = lhs.digits_.crbegin(), crit_rhs = rhs.digits_.crbegin(); crit_lhs != lhs.digits_.crend(); ++crit_lhs, ++crit_rhs) {
		if (*crit_lhs < *crit_rhs) return std::strong_ordering::less;
		if (*crit_lhs > *crit_rhs) return std::strong_ordering::greater;
	}
	
	return std::strong_ordering::equal;
}

/* ******************************************************** */
// forwarding functions

std::ostream & operator << (std::ostream & os, const N & n) {
	return detail::opr_ins(os, n);
}

std::istream & operator >> (std::istream & is, N & n) {
	return detail::opr_extr(is, n);
}

N operator + (const N & lhs, const N & rhs) {
	return detail::opr_add(lhs, rhs);
}

N operator - (const N & lhs, const N & rhs) {
	const auto difference = detail::opr_comp(lhs, rhs);
	
	if (difference == 0) return N();
	return (difference > 0 ? detail::opr_subtr(lhs, rhs) : detail::opr_subtr(rhs, lhs));
}

N operator * (const N & lhs, const N & rhs) {
	return detail::opr_mult(lhs, rhs);
}

std::pair<N, N> operator / (const N & lhs, const N & rhs) {
	if (rhs.is_zero()) throw error::division_by_zero();
	return detail::opr_div(lhs, rhs);
}

N operator & (const N & lhs, const N & rhs) {
	return detail::opr_and(lhs, rhs);
}

N operator | (const N & lhs, const N & rhs) {
	return detail::opr_or(lhs, rhs);
}

N operator ^ (const N & lhs, const N & rhs) {
	return detail::opr_xor(lhs, rhs);
}

bool operator == (const N & lhs, const N & rhs) {
	return detail::opr_eq(lhs, rhs);
}

std::strong_ordering operator <=> (const N & lhs, const N & rhs) {
	return detail::opr_comp(lhs, rhs);
}

} // /namespace jmaths

namespace jmaths {

void N::remove_leading_zeroes_() {
	while (!digits_.empty()) {
		if (digits_.back() != 0) break;
		digits_.pop_back();
	}
}

BASE_INT N::front_() const {
	if (digits_.empty()) return 0;
	
	return digits_.front();
}

std::string N::conv_to_base_ (unsigned base) const {
	static constexpr const char base_converter[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";
		
	if (is_zero()) return "0";
	
	std::string num_str;
	
	num_str.reserve(((double)BASE_INT_BITS / base) * digits_.size() + 1);
	
	N helper (*this);
	
	while (!helper.is_zero()) {
		const auto div_pair = detail::opr_div(helper, base);
		num_str.push_back(base_converter[div_pair.second.front_()]);
		helper = div_pair.first;
	}
	
	num_str.shrink_to_fit();
	
	std::reverse(num_str.begin(), num_str.end());
	
	return num_str;
}

void N::handle_str_ (std::string_view num_str, unsigned base) {
	if (num_str.empty() || (num_str.size() == 1 && num_str.front() == '0')) return;
	
	for (auto cit = num_str.cbegin(); &*cit != &num_str.back(); ++cit) {
		opr_add_assign_(base_converter(*cit));
		opr_mult_assign_(base);
	}
	
	opr_add_assign_(base_converter(num_str.back()));
}

bool N::bit_ (BIT_TYPE pos) const {
	const N shifted = opr_bitshift_r_(pos);

	return (shifted.front_() & 1);
}

void N::bit_ (BIT_TYPE pos, bool val) {
	const size_t pos_whole = pos / BASE_INT_BITS;
	const BIT_TYPE pos_mod = pos % BASE_INT_BITS;
	
	if (is_zero()) {
		digits_.reserve(pos_whole + 1);
		digits_.insert(digits_.begin(), pos_whole, 0);
		digits_.emplace_back((BASE_INT)val << pos_mod);
	} else {
		if (pos_whole < digits_.size()) {
			const BASE_INT bitmask = (BASE_INT)1 << pos_mod;
			if (val) {
				digits_[pos_whole] |= bitmask;
			} else {
				digits_[pos_whole] &= ~bitmask;
			}
		} else {
			digits_.reserve(digits_.size() + pos_whole - digits_.size() + 1);
			digits_.insert(digits_.end(), pos_whole - digits_.size(), 0);
			digits_.emplace_back((BASE_INT)val << pos_mod);
		}
	}
}

void N::opr_incr_() {
	for (auto & digit : digits_)
		if (digit++ < MAX_DIGIT) return;
	
	digits_.emplace_back(1);
}

void N::opr_decr_() {
	for (auto & digit : digits_) {
		if (digit-- > 0) {
			remove_leading_zeroes_();
			return;
		}
	}

	remove_leading_zeroes_();
}

void N::opr_add_assign_ (const N & rhs) {
	// check for additive identity
	if (rhs.is_zero()) return;
	if (this->is_zero()) return (void)operator=(rhs);
	
	// minimum number of digits in sum; this expression is >= number of digits in rhs
	digits_.reserve(std::max(this->digits_.size(), rhs.digits_.size()) + 1);
	digits_.resize(std::max(this->digits_.size(), rhs.digits_.size()));
	
	bool carry = false;
	
	size_t i = 0;
	
	for (; i < rhs.digits_.size(); ++i) {
		const bool next_carry = !(rhs.digits_[i] < (carry ? MAX_DIGIT : BASE) - digits_[i]);
		
		digits_[i] += carry + rhs.digits_[i];
		carry = next_carry;
	}
	
	for (; carry && i < digits_.size(); ++i) {
		const bool next_carry = !(digits_[i] < MAX_DIGIT);
		
		++digits_[i];
		carry = next_carry;
	}
	
	if (carry) digits_.emplace_back(1);
}

void N::opr_subtr_assign_ (const N & rhs) {
	// this functions assumes that *this >= rhs and for effiency reasons should only be called when *this > rhs
	
	// check for additive identity
	if (rhs.is_zero()) return;
	
	size_t i = 0;
	
	for (; i < rhs.digits_.size(); ++i) {
		if (this->digits_[i] < rhs.digits_[i]) {
			for (size_t j = i + 1; j < digits_.size(); ++j) {
				if ((digits_[j])-- > 0) break;
			}
		}
		
		this->digits_[i] -= rhs.digits_[i];
	}
	
	remove_leading_zeroes_();
}

void N::opr_mult_assign_ (const N & rhs) {
	// !!! remove temporary (at least temp1) !!!
	
	// check for multiplicative identity
	if (this->is_one()) return (void)operator=(rhs);
	if (rhs.is_one()) return;
	
	// check for multiplicative zero
	if (this->is_zero()) return;
	if (rhs.is_zero()) return (void)digits_.clear();
	
	N product;
	
	// max overhead would be BASE_INT_SIZE
	product.digits_.reserve(this->digits_.size() + rhs.digits_.size());
	
	for (size_t i = 0; i < rhs.digits_.size(); ++i) {
		N temp1;
		temp1.digits_.reserve(i + this->digits_.size() + 1);
		temp1.digits_.insert(temp1.digits_.begin(), i, 0);
		BASE_INT carry = 0;
		for (size_t j = 0; j < this->digits_.size(); ++j) {
			const BASE_INT_BIG temp2 = (BASE_INT_BIG)rhs.digits_[i] * (BASE_INT_BIG)this->digits_[j];
			const BASE_INT temp3 = (BASE_INT)temp2;
			temp1.digits_.emplace_back(carry + temp3);
			const BASE_INT temp_carry = ((BASE_INT_BIG)carry + (BASE_INT_BIG)temp3) >> BASE_INT_BITS;
			carry = (temp2 >> BASE_INT_BITS) + temp_carry;
		}
		
		if (carry) temp1.digits_.emplace_back(carry);
		product.opr_add_assign_(temp1);
	}
	
	operator=(std::move(product));
}

void N::opr_and_assign_ (const N & rhs) {
	if (this->is_zero()) return;
	if (rhs.is_zero()) return (void)digits_.clear();
	
	digits_.resize(std::min(this->digits_.size(), rhs.digits_.size()));
		
	for (size_t i = 0; i < digits_.size(); ++i) {
		this->digits_[i] &= rhs.digits_[i];
	}
	
	remove_leading_zeroes_();
}

void N::opr_or_assign_ (const N & rhs) {
	if (this->is_zero()) return (void)operator=(rhs);
	if (rhs.is_zero()) return;
	
	const N * longest;
	const N * shortest;
	
	if (this->digits_.size() < rhs.digits_.size()) {
		longest = &rhs;
		shortest = this;
	} else {
		longest = this;
		shortest = &rhs;
	}
	
	digits_.reserve(longest->digits_.size());
	
	size_t i = 0;
	
	for (; i < shortest->digits_.size(); ++i) {
		this->digits_[i] |= rhs.digits_[i];
	}
	
	if (longest == this) return;
	
	for (; i < rhs.digits_.size(); ++i) {
		this->digits_.emplace_back(rhs.digits_[i]);
	}
}

void N::opr_xor_assign_ (const N & rhs) {
	if (this->is_zero()) return (void)operator=(rhs);
	if (rhs.is_zero()) return;
	
	const N * longest;
	const N * shortest;
	
	if (this->digits_.size() < rhs.digits_.size()) {
		longest = &rhs;
		shortest = this;
	} else {
		longest = this;
		shortest = &rhs;
	}
	
	digits_.reserve(longest->digits_.size());
	
	size_t i = 0;
	
	for (; i < shortest->digits_.size(); ++i) {
		this->digits_[i] ^= rhs.digits_[i];
	}
	
	if (longest == this) return;
	
	for (; i < rhs.digits_.size(); ++i) {
		this->digits_.emplace_back(rhs.digits_[i]);
	}
	
}

N N::opr_compl_() const {
	// this function returns 0 if *this == 0
	
	N inverted;
	
	inverted.digits_.reserve(digits_.size());
	
	for (const auto & digit : digits_)
		inverted.digits_.emplace_back(~digit);
	
	inverted.remove_leading_zeroes_();
	
	return inverted;
}

N N::opr_bitshift_l_ (BIT_TYPE pos) const {
	if (is_zero()) return N();
	
	const size_t pos_whole = pos / BASE_INT_BITS;
	const BIT_TYPE pos_mod = pos % BASE_INT_BITS;
	
	N shifted;
	
	shifted.digits_.reserve(digits_.size() + pos_whole + 1);
	
	shifted.digits_.emplace_back(digits_.front() << pos_mod);
	
	for (size_t i = 1; i < digits_.size(); ++i) {
		shifted.digits_.emplace_back((digits_[i - 1] >> (BASE_INT_BITS - pos_mod)) + (digits_[i] << pos_mod));
	}
	
	shifted.digits_.emplace_back(digits_.back() >> (BASE_INT_BITS - pos_mod));
	
	shifted.digits_.insert(shifted.digits_.begin(), pos_whole, 0);
	
	shifted.remove_leading_zeroes_();
	
	return shifted;
}

N N::opr_bitshift_r_ (BIT_TYPE pos) const {
	if (is_zero()) return N();
	
	const size_t pos_whole = pos / BASE_INT_BITS;
	
	if (pos_whole >= digits_.size()) return N();
	
	const BIT_TYPE pos_mod = pos % BASE_INT_BITS;
	
	N shifted;
	
	shifted.digits_.reserve(digits_.size() - pos_whole);
	
	for (size_t i = pos_whole; i < digits_.size() - 1; ++i) {
		shifted.digits_.emplace_back((digits_[i] >> pos_mod) + (digits_[i + 1] << (BASE_INT_BITS - pos_mod)));
	}
	
	shifted.digits_.emplace_back(digits_.back() >> pos_mod);
	
	shifted.remove_leading_zeroes_();
	
	return shifted;
}

void N::opr_bitshift_l_assign_ (BIT_TYPE pos) {
	if (is_zero()) return;
	
	const size_t pos_whole = pos / BASE_INT_BITS;
	const BIT_TYPE pos_mod = pos % BASE_INT_BITS;
	
	digits_.reserve(digits_.size() + pos_whole + 1);
	
	BASE_INT previous = digits_.front();
	
	digits_.front() <<= pos_mod;
	
	for (size_t i = 1; i < digits_.size(); ++i) {
		const BASE_INT current = digits_[i];
		digits_[i] = (previous >> (BASE_INT_BITS - pos_mod)) | (digits_[i] << pos_mod);
		previous = current;
	}
	
	digits_.emplace_back(previous >> (BASE_INT_BITS - pos_mod));
	
	digits_.insert(digits_.begin(), pos_whole, 0);
	
	remove_leading_zeroes_();
}

void N::opr_bitshift_r_assign_ (BIT_TYPE pos) {
	if (is_zero()) return;
	
	const size_t pos_whole = pos / BASE_INT_BITS;
	
	if (pos_whole >= digits_.size()) return (void)digits_.clear();
	
	const BIT_TYPE pos_mod = pos % BASE_INT_BITS;
	
	for (size_t i = pos_whole, j = 0; i < digits_.size() - 1; ++i, ++j) {
		digits_[j] = ((digits_[i] >> pos_mod) + (digits_[i + 1] << (BASE_INT_BITS - pos_mod)));
	}
	
	if (pos_whole == 0) {
		digits_.back() >>= pos_mod;
	} else {
		digits_[digits_.size() - pos_whole] = digits_.back() >> pos_mod;
		digits_.resize(digits_.size() - pos_whole + 1);
	}
	
	remove_leading_zeroes_();
}

void N::opr_assign_ (std::string_view num_str) {
	digits_.clear();
	
	handle_str_(num_str, DEFAULT_BASE);
}

size_t N::dynamic_size_() const {
	return (digits_.size() * BASE_INT_SIZE);
}

N::N() = default;

N::N (std::string_view num_str, unsigned base) {
	if (base < 2 || base > 64) throw error::invalid_base("You need to provide a string in a base between 2 and 64!");
	
	handle_str_(num_str, base);
}

bool N::is_zero() const {
	return digits_.empty();
}

bool N::is_one() const {
	return (digits_.size() == 1 && digits_.front() == 1);
}

bool N::is_even() const {
	return !is_odd();
}

bool N::is_odd() const {
	if (is_zero()) return false;
	
	return (digits_.front() & 1);
}

BIT_TYPE N::ctz() const {
	BIT_TYPE tz = 0;
	for (const auto & digit : digits_) {
		if (digit != 0) {
			tz += std::countr_zero(digit);
			break;
		}
		
		tz += BASE_INT_BITS;
	}
	
	return tz;
}

BIT_TYPE N::bits() const {
	if (is_zero()) return 1;
	return (digits_.size() * BASE_INT_BITS - std::countl_zero(digits_.back()));
}

size_t N::size() const {
	return (sizeof(*this) + dynamic_size_());
}

std::string N::to_str (unsigned base) const {
	if (base < 2 || base > 64) throw error::invalid_base("You need to enter a base between 2 and 64!");
	
	return conv_to_base_(base);
}

N::operator std::string() const {
	return conv_to_base_(DEFAULT_BASE);
}

std::string N::to_base16() const {
	if (is_zero()) return "0";
	
	std::ostringstream oss;
	
	for (auto crit = digits_.crbegin(); crit != digits_.crend(); ++crit) {
		oss << std::hex << *crit;
	}
	
	std::string num_str = oss.str();
	
	for (auto & digit : num_str) {
		if (digit >= 'a' && digit <= 'z') {
			digit -= 'a' - 'A';
		}
	}
	
	return num_str;
}

N::operator bool() const {
	return !is_zero();
}

N::bit_reference N::operator [] (BIT_TYPE pos) {
	return bit_reference(*this, pos);
}

N::const_bit_reference N::operator [] (BIT_TYPE pos) const {
	return const_bit_reference(*this, pos);
}

N & N::operator ++ () {
	opr_incr_();
	return *this;
}

N & N::operator -- () {
	opr_decr_();
	return *this;
}

N & N::operator += (const N & rhs) {
	opr_add_assign_(rhs);
	return *this;
}

N & N::operator -= (const N & rhs) {
	const auto difference = detail::opr_comp(*this, rhs);
	
	if (difference == 0) digits_.clear();
	else if (difference > 0) opr_subtr_assign_(rhs);
	else operator=(detail::opr_subtr(rhs, *this));
	
	return *this;
}

N & N::operator *= (const N & rhs) {
	opr_mult_assign_(rhs);
	return *this;
}

N & N::operator &= (const N & rhs) {
	opr_and_assign_(rhs);
	return *this;
}

N & N::operator |= (const N & rhs) {
	opr_or_assign_(rhs);
	return *this;
}

N & N::operator ^= (const N & rhs) {
	opr_xor_assign_(rhs);
	return *this;
}

N N::operator ~ () const {
	return opr_compl_();
}

N N::operator << (BIT_TYPE pos) const {
	return opr_bitshift_l_(pos);
}

N N::operator >> (BIT_TYPE pos) const {
	return opr_bitshift_r_(pos);
}

N & N::operator <<= (BIT_TYPE pos) {
	opr_bitshift_l_assign_(pos);
	return *this;
}

N & N::operator >>= (BIT_TYPE pos) {
	opr_bitshift_r_assign_(pos);
	return *this;
}

N & N::operator = (std::string_view num_str) {
	opr_assign_(num_str);
	return *this;
}

} // /namespace jmaths

namespace jmaths {
	
N::bit_reference::bit_reference (const bit_reference & ref) = default;
N::const_bit_reference::const_bit_reference (const const_bit_reference & ref) = default;
N::const_bit_reference::const_bit_reference (const bit_reference & ref) : bit_reference_base_(ref.num_, ref.pos_) {}

N::bit_reference & N::bit_reference::operator = (bool val) {
	num_.bit_(pos_, val);
	num_.remove_leading_zeroes_();
	return *this;
}

N::bit_reference & N::bit_reference::operator = (const bit_reference & ref) {
	num_.bit_(pos_, ref.num_.bit_(ref.pos_));
	num_.remove_leading_zeroes_();
	return *this;
}

N::bit_reference & N::bit_reference::operator = (const const_bit_reference & ref) {
	num_.bit_(pos_, ref.num_.bit_(ref.pos_));
	num_.remove_leading_zeroes_();
	return *this;
}
	
} // /namespace jmaths
