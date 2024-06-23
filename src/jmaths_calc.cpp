#include "../src/jmaths.hpp"

#include "../config/jmaths_def.cfg"

namespace jmaths {

N calc::gcd (N a, N b) {
	FUNCTION_TO_LOG;

	if (a.is_zero()) return b;
	if (b.is_zero()) return a;

	bit_type shift = 0;

	for (; (a.front_() | b.front_()) == 0; shift += base_int_bits) {
		a.digits_.erase(a.digits_.begin());
		b.digits_.erase(b.digits_.begin());
	}

	for (; ((a.front_() | b.front_()) & 1) == 0; ++shift) {
		a.opr_bitshift_r_assign_(1);
		b.opr_bitshift_r_assign_(1);
	}

	while ((a.front_() & 1) == 0) a.opr_bitshift_r_assign_(1);

	do {
		while ((b.front_() & 1) == 0) b.opr_bitshift_r_assign_(1);

		if (detail::opr_comp(a, b) > 0) a.digits_.swap(b.digits_);

		b.opr_subtr_assign_(a);
	} while (!b.is_zero());

	return a.opr_bitshift_l_(shift);
}

std::pair<N, N> calc::sqrt (const N & num) {
	FUNCTION_TO_LOG;

	if (num.is_zero() || num.is_one()) return {num, N{}};

	N start = 1, end = num.opr_bitshift_r_(1), ans;

	while (detail::opr_comp(start, end) <= 0) {
		N mid = detail::opr_add(start, end).opr_bitshift_r_(1);

		const N sqr = detail::opr_mult(mid, mid);

		const auto compared = detail::opr_comp(sqr, num);

		if (compared == 0) return {std::move(mid), N{}};

		if (compared < 0) {
			ans = mid;
			mid.opr_incr_();
			start = std::move(mid);
		} else {
			mid.opr_decr_();
			end = std::move(mid);
		}
	}

	N remainder = detail::opr_subtr(num, detail::opr_mult(ans, ans));

    return {std::move(ans), std::move(remainder)};
}

N calc::sqrt_whole (const N & num) {
	FUNCTION_TO_LOG;

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpessimizing-move"

	if (num.is_zero() || num.is_one()) return num;

	N start = 1, end = num.opr_bitshift_r_(1), ans;

	while (detail::opr_comp(start, end) <= 0) {
		N mid = detail::opr_add(start, end).opr_bitshift_r_(1);

		const N sqr = detail::opr_mult(mid, mid);

		const auto compared = detail::opr_comp(sqr, num);

		if (compared == 0) return std::move(mid);

		if (compared < 0) {
			ans = mid;
			mid.opr_incr_();
			start = std::move(mid);
		} else {
			mid.opr_decr_();
			end = std::move(mid);
		}
	}

    return std::move(ans);

	#pragma GCC diagnostic pop
}

N calc::pow (N base, N exponent) {
	FUNCTION_TO_LOG;

	if (exponent.is_zero()) return 1;

	N result (1);

	for (;;) {
		if (exponent.front_() & 1) result.opr_mult_assign_(base);
		exponent.opr_bitshift_r_assign_(1);
		if (exponent.is_zero()) break;
		base.opr_mult_assign_(base);
	}

	return result;
}

N calc::pow_mod (N base, N exponent, const N & mod) {
	FUNCTION_TO_LOG;

	if (mod.is_zero()) throw error::division_by_zero();

	if (exponent.is_zero()) return 1;

	N result (1);

	for (;;) {
		if (exponent.front_() & 1) {
			result.opr_mult_assign_(base);
			result = detail::opr_div(result, mod).second; // maybe use separate function just for mod ???
		}

		exponent.opr_bitshift_r_assign_(1);
		if (exponent.is_zero()) break;
		base.opr_mult_assign_(base);
	}

	return result;
}

Z calc::pow (Z base, N exponent) {
	FUNCTION_TO_LOG;

	const auto sign = base.is_negative() && exponent.is_odd() ? sign_type::negative : sign_type::positive;

	return Z(pow(std::move(std::move(base).abs()), std::move(exponent)), sign);
}

} // /namespace jmaths

#include "../config/jmaths_undef.cfg"
