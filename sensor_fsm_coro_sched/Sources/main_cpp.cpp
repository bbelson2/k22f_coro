/*
 * main_cpp.cpp
 *
 *  Created on: 1 Aug 2018
 *      Author: belsonba
 */

#include "scheduler.h"
#include "fsm.h"

#include <stl_string.h>

extern "C"
int main_cpp() {
	scheduling::scheduler::instance().registerTask(&sensor_task);
	scheduling::scheduler::instance().run();
	return 0;
}
