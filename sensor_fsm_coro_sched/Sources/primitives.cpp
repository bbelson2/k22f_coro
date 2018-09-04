/*
 * primitives.cpp
 *
 *  Created on: 3 Sep 2018
 *      Author: belsonba
 */

#include <primitives.h>
#include <stlcommon.h>
#include <PE_Types.h>

interrupt_guard::interrupt_guard(bool guard_now /*= true*/) {
	if (guard_now) {
		begin_guarding();
	}
}
interrupt_guard::~interrupt_guard() {
	end_guarding();
}
void interrupt_guard::begin_guarding() {
	if (!is_guarding) {
#ifdef TARGET_ARM
		__DI();
#endif
		is_guarding = true;
	}
}
void interrupt_guard::end_guarding() {
	if (is_guarding) {
		is_guarding = false;
#ifdef TARGET_ARM
		__EI();
#endif
	}
}

