/*
 * stl_mutex.cpp
 *
 *  Created on: 3 Sep 2018
 *      Author: belsonba
 */

#include <stl_mutex.h>

namespace std {

constexpr mutex::mutex() _NOEXCEPT {
}

mutex::~mutex() {
}

void mutex::lock() {
}

bool mutex::try_lock() {
	return true;
}

void mutex::unlock() {
}

}

