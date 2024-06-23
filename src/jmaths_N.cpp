#include "../src/jmaths.hpp"

#include "../config/jmaths_def.cfg"

namespace jmaths {

namespace {

static base_int base_converter (char c) noexcept; // convert char to number for base >= 2 and <= 64

static base_int base_converter (char c) noexcept {
	FUNCTION_TO_LOG;

	if (c >= '0' && c <= '9') return (c - '0');
	if (c >= 'A' && c <= 'Z') return (c - 'A' + 10);
	if (c >= 'a' && c <= 'z') return (c - 'a' + 10 + 26);
	if (c == '+') return 62;

	assert(c == '/'); // it is assumed that c == '/' because no other character would be valid or logical

	return 63;
}

} // /anonymous namespace

} // /namespace jmaths

namespace jmaths {

/**********************************************************/
// implementation functions

std::ostream & detail::opr_ins (std::ostream & os, const N & n) {
	FUNCTION_TO_LOG;

	return os << (std::string)n;
}

std::istream & detail::opr_extr (std::istream & is, N & n) {
	FUNCTION_TO_LOG;

	std::string num_str;
	is >> num_str;
	n.opr_assign_(num_str);

	assert(n == N(num_str));

	return is;
}

N detail::opr_add (const N & lhs, const N & rhs) {
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
		const bool next_carry = !(lhs.digits_[i] < (carry ? max_digit : base) - rhs.digits_[i]);

		sum.digits_.emplace_back(lhs.digits_[i] + rhs.digits_[i] + carry);
		carry = next_carry;
	}

	for (; carry && i < longest->digits_.size(); ++i) {
		const bool next_carry = !(longest->digits_[i] < (carry ? max_digit : base));

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

N detail::opr_subtr (N lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	// this functions assumes that lhs >= rhs and for effiency reasons should only be called when lhs > rhs

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

N detail::opr_mult (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	#ifndef KARATSUBA
		#error KARATSUBA is not defined, please define it when compiling!
	#endif

	#if KARATSUBA

	#error The KARATSUBA algorithm is still a work in progress, please define KARATSUBA as 0

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

		for (std::size_t i = 0; i < longest_digits_right && cit != shortest->digits_.cend(); ++cit, ++i) {
			shortest_num_r.digits_.emplace_back(*cit);
		}

		if (cit != shortest->digits_.cend()) {
			shortest_num_l.digits_.reserve(longest_digits_left);
		}

		for (std::size_t i = 0; i < longest_digits_left && cit != shortest->digits_.cend(); ++cit, ++i) {
			shortest_num_l.digits_.emplace_back(*cit);
		}
	}
	
	const auto multiply = [] (const N & lhs, const N & rhs, std::size_t exp) -> N {
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
				const base_int_big temp2 = (base_int_big)lhs.digits_[i] * (base_int_big)rhs.digits_[j];
				const base_int temp3 = (base_int)temp2;
				temp1.digits_.emplace_back(carry + temp3);
				const base_int temp_carry = ((base_int_big)carry + (base_int_big)temp3) >> base_int_bits;
				carry = (temp2 >> base_int_bits) + temp_carry;
			}

			if (carry) temp1.digits_.emplace_back(carry);
			product.opr_add_assign_(temp1);
		}

		return product;
	};

	const auto insert_front = [] (const N & n, std::size_t exp) -> N {
		N exponentiated;
		exponentiated.digits_.reserve(n.digits_.size() + exp);
		exponentiated.digits_.insert(exponentiated.digits_.begin(), exp, 0);
		std::copy(n.digits_.begin(), n.digits_.end(), std::back_inserter(exponentiated.digits_));
		return exponentiated;
	};

	const auto XlYl = multiply(shortest_num_l, longest_num_l, 2 * longest_digits_right);
	const auto XrYr = multiply(shortest_num_r, longest_num_r, 0); // should this really be 0 ???

	return insert_front(XlYl, 2 * longest_digits_right) + insert_front(((multiply(shortest_num_l + shortest_num_r, longest_num_l + longest_num_r, 0) - XlYl) - XrYr), longest_digits_right) + XrYr;
	
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
			const base_int_big temp2 = (base_int_big)lhs.digits_[i] * (base_int_big)rhs.digits_[j];
			const base_int temp3 = (base_int)temp2;
			temp1.digits_.emplace_back(carry + temp3);
			const base_int temp_carry = ((base_int_big)carry + (base_int_big)temp3) >> base_int_bits;
			carry = (temp2 >> base_int_bits) + temp_carry;
		}

		if (carry) temp1.digits_.emplace_back(carry);
		product.opr_add_assign_(temp1);
	}

	return product;

	#endif
}

std::pair<N, N> detail::opr_div (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	if (lhs.is_zero()) return {N{}, N{}};

	// check if lhs == rhs
	if (opr_eq(lhs, rhs)) {
		static const N one (1);
		return {one, N{}};
	}

	N q, r;

	if (const auto max_size = lhs.digits_.size() - rhs.digits_.size() + 1; lhs.digits_.size() + 1 >= rhs.digits_.size()) {
		// rhs.digits_.size() is always >= 1 so even if lhs.digits_.size() + 1 overflows to 0 no errors shall occur
		#ifndef NDEBUG
			using nlim = std::numeric_limits<decltype(lhs.digits_.size())>;
		#endif

		assert(lhs.digits_.size() == nlim::max() ? 1 <= rhs.digits_.size() : lhs.digits_.size() >= rhs.digits_.size() ? lhs.digits_.size() - rhs.digits_.size() <= nlim::max() - 1 : true /*-(rhs.digits_.size() - lhs.digits_.size()) <= nlim::max() - 1*/); // assert for overflow
		assert(lhs.digits_.size() == nlim::max() ? true : lhs.digits_.size() + 1 >= rhs.digits_.size()); // assert for overflow to 0

		q.digits_.reserve(max_size);
	}

	r.digits_.reserve(rhs.digits_.size());

	for (bit_type i = lhs.digits_.size() * base_int_bits; i --> 0;) {
		r.opr_bitshift_l_assign_(1);
		r.bit_(0, lhs.bit_(i));
		if (opr_comp(r, rhs) >= 0) {
			r.opr_subtr_assign_(rhs);
			q.bit_(i, 1);
		}
	}

	assert(q * rhs + r == lhs);

	return {q, r};
}

N detail::opr_and (const N & lhs, const N & rhs) {
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

N detail::opr_or (const N & lhs, const N & rhs) {
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

N detail::opr_xor (const N & lhs, const N & rhs) {
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

bool detail::opr_eq (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	return (lhs.digits_ == rhs.digits_);
}

std::strong_ordering detail::opr_comp (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	if (lhs.digits_.size() < rhs.digits_.size()) return std::strong_ordering::less;
	if (lhs.digits_.size() > rhs.digits_.size()) return std::strong_ordering::greater;

	for (auto crit_lhs = lhs.digits_.crbegin(), crit_rhs = rhs.digits_.crbegin(); crit_lhs != lhs.digits_.crend(); ++crit_lhs, ++crit_rhs) {
		if (*crit_lhs < *crit_rhs) return std::strong_ordering::less;
		if (*crit_lhs > *crit_rhs) return std::strong_ordering::greater;
	}

	return std::strong_ordering::equal;
}

/**********************************************************/
// forwarding functions

std::ostream & operator << (std::ostream & os, const N & n) {
	FUNCTION_TO_LOG;

	return detail::opr_ins(os, n);
}

std::istream & operator >> (std::istream & is, N & n) {
	FUNCTION_TO_LOG;

	return detail::opr_extr(is, n);
}

N operator + (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_add(lhs, rhs);
}

N operator - (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	const auto difference = detail::opr_comp(lhs, rhs);

	if (difference == 0) return N{};
	return (difference > 0 ? detail::opr_subtr(lhs, rhs) : detail::opr_subtr(rhs, lhs));
}

N operator * (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_mult(lhs, rhs);
}

std::pair<N, N> operator / (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	if (rhs.is_zero()) throw error::division_by_zero();
	return detail::opr_div(lhs, rhs);
}

N operator & (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_and(lhs, rhs);
}

N operator | (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_or(lhs, rhs);
}

N operator ^ (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_xor(lhs, rhs);
}

bool operator == (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_eq(lhs, rhs);
}

std::strong_ordering operator <=> (const N & lhs, const N & rhs) {
	FUNCTION_TO_LOG;

	return detail::opr_comp(lhs, rhs);
}

} // /namespace jmaths

namespace jmaths {

void N::remove_leading_zeroes_() {
	FUNCTION_TO_LOG;

	while (!digits_.empty()) {
		if (digits_.back() != 0) break;
		digits_.pop_back();
	}
}

base_int N::front_() const {
	FUNCTION_TO_LOG;

	if (digits_.empty()) return 0;

	return digits_.front();
}

std::string N::conv_to_base_ (unsigned base) const {
	FUNCTION_TO_LOG;

	static constexpr const char base_converter[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

	if (is_zero()) return "0";

	std::string num_str;

	num_str.reserve(((double)base_int_bits / base) * digits_.size() + 1);

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
	FUNCTION_TO_LOG;

	if (num_str.empty() || (num_str.size() == 1 && num_str.front() == '0')) return;

	for (auto cit = num_str.cbegin(); &*cit != &num_str.back(); ++cit) {
		opr_add_assign_(base_converter(*cit));
		opr_mult_assign_(base);
	}

	opr_add_assign_(base_converter(num_str.back()));
}

bool N::bit_ (bit_type pos) const {
	FUNCTION_TO_LOG;

	const N shifted = opr_bitshift_r_(pos);

	return (shifted.front_() & 1);
}

void N::bit_ (bit_type pos, bool val) {
	FUNCTION_TO_LOG;

	const std::size_t pos_whole = pos / base_int_bits;
	const bit_type pos_mod = pos % base_int_bits;

	if (is_zero()) {
		digits_.reserve(pos_whole + 1);
		digits_.insert(digits_.begin(), pos_whole, 0);
		digits_.emplace_back((base_int)val << pos_mod);
	} else {
		if (pos_whole < digits_.size()) {
			const base_int bitmask = (base_int)1 << pos_mod;
			if (val) {
				digits_[pos_whole] |= bitmask;
			} else {
				digits_[pos_whole] &= ~bitmask;
			}
		} else {
			digits_.reserve(digits_.size() + pos_whole - digits_.size() + 1);
			digits_.insert(digits_.end(), pos_whole - digits_.size(), 0);
			digits_.emplace_back((base_int)val << pos_mod);
		}
	}
}

std::size_t N::dynamic_size_() const {
	FUNCTION_TO_LOG;

	return (digits_.size() * base_int_size);
}

void N::opr_incr_() {
	FUNCTION_TO_LOG;

	for (auto & digit : digits_)
		if (digit++ < max_digit) return;

	digits_.emplace_back(1);
}

void N::opr_decr_() {
	FUNCTION_TO_LOG;

	for (auto & digit : digits_) {
		if (digit-- > 0) {
			remove_leading_zeroes_();
			return;
		}
	}

	remove_leading_zeroes_();
}

void N::opr_add_assign_ (const N & rhs) {
	FUNCTION_TO_LOG;

	// check for additive identity
	if (rhs.is_zero()) return;
	if (this->is_zero()) return (void)operator=(rhs);

	// minimum number of digits in sum; this expression is >= number of digits in rhs
	digits_.reserve(std::max(this->digits_.size(), rhs.digits_.size()) + 1);
	digits_.resize(std::max(this->digits_.size(), rhs.digits_.size()));

	bool carry = false;

	std::size_t i = 0;

	for (; i < rhs.digits_.size(); ++i) {
		const bool next_carry = !(rhs.digits_[i] < (carry ? max_digit : base) - digits_[i]);

		digits_[i] += carry + rhs.digits_[i];
		carry = next_carry;
	}

	for (; carry && i < digits_.size(); ++i) {
		const bool next_carry = !(digits_[i] < max_digit);

		++digits_[i];
		carry = next_carry;
	}

	if (carry) digits_.emplace_back(1);
}

void N::opr_subtr_assign_ (const N & rhs) {
	FUNCTION_TO_LOG;

	// this functions assumes that *this >= rhs and for effiency reasons should only be called when *this > rhs

	// check for additive identity
	if (rhs.is_zero()) return;

	std::size_t i = 0;

	for (; i < rhs.digits_.size(); ++i) {
		if (this->digits_[i] < rhs.digits_[i]) {
			for (std::size_t j = i + 1; j < digits_.size(); ++j) {
				if ((digits_[j])-- > 0) break;
			}
		}

		this->digits_[i] -= rhs.digits_[i];
	}

	remove_leading_zeroes_();
}

void N::opr_mult_assign_ (const N & rhs) {
	FUNCTION_TO_LOG;

	// !!! remove temporary (at least temp1) !!!

	// check for multiplicative identity
	if (this->is_one()) return (void)operator=(rhs);
	if (rhs.is_one()) return;

	// check for multiplicative zero
	if (this->is_zero()) return;
	if (rhs.is_zero()) return (void)digits_.clear();

	N product;

	// max overhead would be base_int_size
	product.digits_.reserve(this->digits_.size() + rhs.digits_.size());

	for (std::size_t i = 0; i < rhs.digits_.size(); ++i) {
		N temp1;
		temp1.digits_.reserve(i + this->digits_.size() + 1);
		temp1.digits_.insert(temp1.digits_.begin(), i, 0);
		base_int carry = 0;
		for (std::size_t j = 0; j < this->digits_.size(); ++j) {
			const base_int_big temp2 = (base_int_big)rhs.digits_[i] * (base_int_big)this->digits_[j];
			const base_int temp3 = (base_int)temp2;
			temp1.digits_.emplace_back(carry + temp3);
			const base_int temp_carry = ((base_int_big)carry + (base_int_big)temp3) >> base_int_bits;
			carry = (temp2 >> base_int_bits) + temp_carry;
		}

		if (carry) temp1.digits_.emplace_back(carry);
		product.opr_add_assign_(temp1);
	}

	operator=(std::move(product));
}

void N::opr_and_assign_ (const N & rhs) {
	FUNCTION_TO_LOG;

	if (this->is_zero()) return;
	if (rhs.is_zero()) return (void)digits_.clear();

	digits_.resize(std::min(this->digits_.size(), rhs.digits_.size()));

	for (std::size_t i = 0; i < digits_.size(); ++i) {
		this->digits_[i] &= rhs.digits_[i];
	}

	remove_leading_zeroes_();
}

void N::opr_or_assign_ (const N & rhs) {
	FUNCTION_TO_LOG;

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

	std::size_t i = 0;

	for (; i < shortest->digits_.size(); ++i) {
		this->digits_[i] |= rhs.digits_[i];
	}

	if (longest == this) return;

	for (; i < rhs.digits_.size(); ++i) {
		this->digits_.emplace_back(rhs.digits_[i]);
	}
}

void N::opr_xor_assign_ (const N & rhs) {
	FUNCTION_TO_LOG;

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

	std::size_t i = 0;

	for (; i < shortest->digits_.size(); ++i) {
		this->digits_[i] ^= rhs.digits_[i];
	}

	if (longest == this) return;

	for (; i < rhs.digits_.size(); ++i) {
		this->digits_.emplace_back(rhs.digits_[i]);
	}

}

N N::opr_compl_() const {
	FUNCTION_TO_LOG;

	// this function returns 0 if *this == 0

	N inverted;

	inverted.digits_.reserve(digits_.size());

	for (const auto & digit : digits_)
		inverted.digits_.emplace_back(~digit);

	inverted.remove_leading_zeroes_();

	return inverted;
}

N N::opr_bitshift_l_ (bit_type pos) const {
	FUNCTION_TO_LOG;

	if (is_zero()) return N{};

	const std::size_t pos_whole = pos / base_int_bits;
	const bit_type pos_mod = pos % base_int_bits;

	N shifted;

	shifted.digits_.reserve(digits_.size() + pos_whole + 1);

	shifted.digits_.emplace_back(digits_.front() << pos_mod);

	for (std::size_t i = 1; i < digits_.size(); ++i) {
		shifted.digits_.emplace_back((digits_[i - 1] >> (base_int_bits - pos_mod)) + (digits_[i] << pos_mod));
	}

	shifted.digits_.emplace_back(digits_.back() >> (base_int_bits - pos_mod));

	shifted.digits_.insert(shifted.digits_.begin(), pos_whole, 0);

	shifted.remove_leading_zeroes_();

	return shifted;
}

N N::opr_bitshift_r_ (bit_type pos) const {
	FUNCTION_TO_LOG;

	if (is_zero()) return N{};

	const std::size_t pos_whole = pos / base_int_bits;

	if (pos_whole >= digits_.size()) return N{};

	const bit_type pos_mod = pos % base_int_bits;

	N shifted;

	shifted.digits_.reserve(digits_.size() - pos_whole);

	for (std::size_t i = pos_whole; i < digits_.size() - 1; ++i) {
		shifted.digits_.emplace_back((digits_[i] >> pos_mod) + (digits_[i + 1] << (base_int_bits - pos_mod)));
	}

	shifted.digits_.emplace_back(digits_.back() >> pos_mod);

	shifted.remove_leading_zeroes_();

	return shifted;
}

void N::opr_bitshift_l_assign_ (bit_type pos) {
	FUNCTION_TO_LOG;

	if (is_zero()) return;

	const std::size_t pos_whole = pos / base_int_bits;
	const bit_type pos_mod = pos % base_int_bits;

	digits_.reserve(digits_.size() + pos_whole + 1);

	base_int previous = digits_.front();

	digits_.front() <<= pos_mod;

	for (std::size_t i = 1; i < digits_.size(); ++i) {
		const base_int current = digits_[i];
		digits_[i] = (previous >> (base_int_bits - pos_mod)) | (digits_[i] << pos_mod);
		previous = current;
	}

	digits_.emplace_back(previous >> (base_int_bits - pos_mod));

	digits_.insert(digits_.begin(), pos_whole, 0);

	remove_leading_zeroes_();
}

void N::opr_bitshift_r_assign_ (bit_type pos) {
	FUNCTION_TO_LOG;

	if (is_zero()) return;

	const std::size_t pos_whole = pos / base_int_bits;

	if (pos_whole >= digits_.size()) return (void)digits_.clear();

	const bit_type pos_mod = pos % base_int_bits;

	for (std::size_t i = pos_whole, j = 0; i < digits_.size() - 1; ++i, ++j) {
		digits_[j] = ((digits_[i] >> pos_mod) + (digits_[i + 1] << (base_int_bits - pos_mod)));
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
	FUNCTION_TO_LOG;

	digits_.clear();

	handle_str_(num_str, default_base);
}

N::N() = default;

N::N (std::string_view num_str, unsigned base) {
	FUNCTION_TO_LOG;

	if (base < 2 || base > 64) throw error::invalid_base("You need to provide a string in a base between 2 and 64!");

	handle_str_(num_str, base);
}

bool N::is_zero() const {
	FUNCTION_TO_LOG;

	return digits_.empty();
}

bool N::is_one() const {
	FUNCTION_TO_LOG;

	return (digits_.size() == 1 && digits_.front() == 1);
}

bool N::is_even() const {
	FUNCTION_TO_LOG;

	return !is_odd();
}

bool N::is_odd() const {
	FUNCTION_TO_LOG;

	if (is_zero()) return false;

	return (digits_.front() & 1);
}

bit_type N::ctz() const {
	FUNCTION_TO_LOG;

	bit_type tz = 0;
	for (const auto & digit : digits_) {
		if (digit != 0) {
			tz += std::countr_zero(digit);
			break;
		}

		tz += base_int_bits;
	}

	return tz;
}

bit_type N::bits() const {
	FUNCTION_TO_LOG;

	if (is_zero()) return 1;
	return (digits_.size() * base_int_bits - std::countl_zero(digits_.back()));
}

std::size_t N::size() const {
	FUNCTION_TO_LOG;

	return (sizeof(*this) + dynamic_size_());
}

std::string N::to_str (unsigned base) const {
	FUNCTION_TO_LOG;

	if (base < 2 || base > 64) throw error::invalid_base("You need to enter a base between 2 and 64!");

	return conv_to_base_(base);
}

N::operator std::string() const {
	FUNCTION_TO_LOG;

	return conv_to_base_(default_base);
}

std::string N::to_base16() const {
	FUNCTION_TO_LOG;

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
	FUNCTION_TO_LOG;

	return !is_zero();
}

N::bit_reference N::operator [] (bit_type pos) {
	FUNCTION_TO_LOG;

	return bit_reference(*this, pos);
}

N::const_bit_reference N::operator [] (bit_type pos) const {
	FUNCTION_TO_LOG;

	return const_bit_reference(*this, pos);
}

N & N::operator ++ () {
	FUNCTION_TO_LOG;

	opr_incr_();
	return *this;
}

N & N::operator -- () {
	FUNCTION_TO_LOG;

	opr_decr_();
	return *this;
}

N & N::operator += (const N & rhs) {
	FUNCTION_TO_LOG;

	opr_add_assign_(rhs);
	return *this;
}

N & N::operator -= (const N & rhs) {
	FUNCTION_TO_LOG;

	const auto difference = detail::opr_comp(*this, rhs);

	if (difference == 0) digits_.clear();
	else if (difference > 0) opr_subtr_assign_(rhs);
	else operator=(detail::opr_subtr(rhs, *this));

	return *this;
}

N & N::operator *= (const N & rhs) {
	FUNCTION_TO_LOG;

	opr_mult_assign_(rhs);
	return *this;
}

N & N::operator &= (const N & rhs) {
	FUNCTION_TO_LOG;

	opr_and_assign_(rhs);
	return *this;
}

N & N::operator |= (const N & rhs) {
	FUNCTION_TO_LOG;

	opr_or_assign_(rhs);
	return *this;
}

N & N::operator ^= (const N & rhs) {
	FUNCTION_TO_LOG;

	opr_xor_assign_(rhs);
	return *this;
}

N N::operator ~ () const {
	FUNCTION_TO_LOG;

	return opr_compl_();
}

N N::operator << (bit_type pos) const {
	FUNCTION_TO_LOG;

	return opr_bitshift_l_(pos);
}

N N::operator >> (bit_type pos) const {
	FUNCTION_TO_LOG;

	return opr_bitshift_r_(pos);
}

N & N::operator <<= (bit_type pos) {
	FUNCTION_TO_LOG;

	opr_bitshift_l_assign_(pos);
	return *this;
}

N & N::operator >>= (bit_type pos) {
	FUNCTION_TO_LOG;

	opr_bitshift_r_assign_(pos);
	return *this;
}

N & N::operator = (std::string_view num_str) {
	FUNCTION_TO_LOG;

	opr_assign_(num_str);
	return *this;
}

} // /namespace jmaths

namespace jmaths {

N::bit_reference::bit_reference (const bit_reference & ref) = default;
N::const_bit_reference::const_bit_reference (const const_bit_reference & ref) = default;
N::const_bit_reference::const_bit_reference (const bit_reference & ref) : bit_reference_base_(ref.num_, ref.pos_) {
	FUNCTION_TO_LOG;
}

N::bit_reference & N::bit_reference::operator = (bool val) {
	FUNCTION_TO_LOG;

	num_.bit_(pos_, val);
	num_.remove_leading_zeroes_();
	return *this;
}

N::bit_reference & N::bit_reference::operator = (const bit_reference & ref) {
	FUNCTION_TO_LOG;

	num_.bit_(pos_, ref.num_.bit_(ref.pos_));
	num_.remove_leading_zeroes_();
	return *this;
}

N::bit_reference & N::bit_reference::operator = (const const_bit_reference & ref) {
	FUNCTION_TO_LOG;

	num_.bit_(pos_, ref.num_.bit_(ref.pos_));
	num_.remove_leading_zeroes_();
	return *this;
}

} // /namespace jmaths

#include "../config/jmaths_undef.cfg"
