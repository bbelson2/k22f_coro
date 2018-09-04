/*
 * primitives.h
 *
 *  Created on: 3 Sep 2018
 *      Author: belsonba
 */

#ifndef SOURCES_PRIMITIVES_H_
#define SOURCES_PRIMITIVES_H_

#include <stlcommon.h>
#include <PE_Types.h>

class _NOVTABLE interrupt_guard
{
public:
	interrupt_guard(bool guard_now = true);
	~interrupt_guard();
protected:
	void begin_guarding();
	void end_guarding();
private:
	bool is_guarding;
};

#endif /* SOURCES_PRIMITIVES_H_ */
