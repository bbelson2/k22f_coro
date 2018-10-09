/*
 * stl_future.cpp
 *
 *  Created on: 1 Sep 2018
 *      Author: belsonba
 */

#include <stl_future.h>

namespace std {
	error_code make_error_code(future_errc e) noexcept {
		return error_code(static_cast<int>(e), future_category());
	}
	error_condition make_error_condition(future_errc e) _NOEXCEPT {
		return error_condition(static_cast<int>(e), future_category());
	}
	const error_category& future_category() _NOEXCEPT {
		static future_error_category s;
		return s;
	}
}

#include <primitives.h>

/*
std::future<int> f() {
	return std::future<int>(0);
}
*/
std::future_shared_state<int> ss;

