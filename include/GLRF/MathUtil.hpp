#pragma once

namespace GLRF {
	/**
	 * Default signum function.
	 * -1   if val <  0
	 *  0   if val == 0
	 *  1   if val >  0
	 */
	template <typename T>
	int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

	/**
	 * Positive signum function.
	 * -1   if val <  0
	 *  1   if val >= 0
	 */
	template <typename T>
	int psgn(T val) {
		return sgn(sgn(val) * T(2) + T(1));
	}
}