/*
 * main_cpp.cpp
 *
 *  Created on: 1 Aug 2018
 *      Author: belsonba
 */

#include "scheduler.h"
#include "fsm.h"
//#include "app_events.h"

//#include <stl_string.h>

//#error - See http://andybrown.me.uk/2011/01/15/the-standard-template-library-stl-for-avr-with-c-streams/

extern "C" int fut_test();

extern "C"
int main_cpp() {
	int t = fut_test();

	scheduling::scheduler::instance().registerTask(&sensor_task);
	scheduling::scheduler::instance().run();
	return 0;
}
