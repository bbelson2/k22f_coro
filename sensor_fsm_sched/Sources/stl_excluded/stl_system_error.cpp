/*
 * stl_system_error.cpp
 *
 *  Created on: 1 Sep 2018
 *      Author: belsonba
 */

#include <stl_system_error.h>

namespace std {
	const error_category& generic_category() _NOEXCEPT {
		static generic_error_category s;
		return s;
	}

	const error_category& system_category() _NOEXCEPT {
		static system_error_category s;
		return s;
	}

	error_condition system_error_category::default_error_condition(int _Errval) const _NOEXCEPT {
		return (error_condition(_Errval, system_category()));
	}

	error_condition error_code::default_error_condition() const _NOEXCEPT {
		return category().default_error_condition(value());
	}

} // namespace std

