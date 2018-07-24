/*
 * fsm_cpp.cpp
 *
 *  Created on: 17 Jul 2018
 *      Author: belsonba
 */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
//#include "Events.h"
#include "AD1.h"
#include "TI1.h"
//#include "Red.h"
#include "Green.h"
extern "C" {
#include "Term1.h"
}
#include "PE_Types.h"
#include <stdbool.h>
#include "fsm.h"

volatile fsm_state_t fsm_state = FSM_UNINITIALISED;
word sensor_value;

word scale_value(word value);
void report_value(word value);

void call_tests();

/**
 * @brief Finite state machine transition
 * @return true if the transition was successful, false if not.
 */
extern "C"
bool fsm_transition(fsm_state_t from, fsm_state_t to) {
	if (fsm_state == from) {
		fsm_state = to;
		return true;
	}
	return false;
}

/**
 * @brief Finite state machine
 * @return true if the MCU should sleep until the next interrupt.
 * 		   false if the scheduler should continue immediately with the next action.
 */
extern "C"
bool fsm_execute() {
	word value;

	switch (fsm_state) {
	case FSM_UNINITIALISED:
		Term1_Cls();
		Term1_MoveTo(1,1);
		Term1_SendStr((void*)"sensor_fsm_coro initialisation\r\n");
		PIT_MCR |= 1; // set the freeze bit in PIT_MCR so timers are stopped in debug mode
		if (AD1_Calibrate(false) == ERR_OK) {
			return fsm_transition(FSM_UNINITIALISED, FSM_WAIT_CALIBRATION);
		}
		else {
			Term1_SendStr((void*)"Fatal error: Calibration failed\r\n");
		}
		break;
	case FSM_WAIT_CALIBRATION:
		break;
	case FSM_CALIBRATION_COMPLETE:
		// Start a timer
		fsm_transition(FSM_CALIBRATION_COMPLETE, FSM_WAIT_TIMER);
		if (TI1_Enable()) {
			return true;
		}
		else {
			// Try again
			fsm_transition(FSM_WAIT_TIMER, FSM_CALIBRATION_COMPLETE);
		}
		break;
	case FSM_WAIT_TIMER:
		break;
	case FSM_TIMER_COMPLETE:
		// Request ADC measurement
		if (AD1_Measure(false) == ERR_OK) {
			return fsm_transition(FSM_TIMER_COMPLETE, FSM_WAIT_ADC);
		}
		else {
			fsm_transition(FSM_TIMER_COMPLETE, FSM_WAIT_TIMER);
		}
		break;
	case FSM_WAIT_ADC:
		break;
	case FSM_ADC_COMPLETE:
		report_value(sensor_value);
		call_tests();
		return fsm_transition(FSM_ADC_COMPLETE, FSM_WAIT_TIMER);
	}

	return false;
}

extern "C" void fsm_handle_calibration_complete() {
	switch (fsm_state) {
	case FSM_UNINITIALISED:
		break;
	case FSM_WAIT_CALIBRATION:
		Term1_SendStr((void*)"Finished calibration.\r\n");
		fsm_transition(FSM_WAIT_CALIBRATION, FSM_CALIBRATION_COMPLETE);
		break;
	case FSM_CALIBRATION_COMPLETE:
		break;
	case FSM_WAIT_TIMER:
		break;
	case FSM_TIMER_COMPLETE:
		break;
	case FSM_WAIT_ADC:
		break;
	case FSM_ADC_COMPLETE:
		break;
	}
}

extern "C" void fsm_handle_timer_interrupt() {
	switch (fsm_state) {
	case FSM_UNINITIALISED:
		break;
	case FSM_WAIT_CALIBRATION:
		break;
	case FSM_CALIBRATION_COMPLETE:
		break;
	case FSM_WAIT_TIMER:
		fsm_transition(FSM_WAIT_TIMER, FSM_TIMER_COMPLETE);
		break;
	case FSM_TIMER_COMPLETE:
		break;
	case FSM_WAIT_ADC:
		break;
	case FSM_ADC_COMPLETE:
		break;
	}
}

extern "C" void fsm_handle_adc_complete() {
	switch (fsm_state) {
	case FSM_UNINITIALISED:
		break;
	case FSM_WAIT_CALIBRATION:
		break;
	case FSM_CALIBRATION_COMPLETE:
		break;
	case FSM_WAIT_TIMER:
		break;
	case FSM_TIMER_COMPLETE:
		break;
	case FSM_WAIT_ADC:
		if (ERR_OK == AD1_GetValue16(&sensor_value)) {
			fsm_transition(FSM_WAIT_ADC, FSM_ADC_COMPLETE);
		}
		else {
			fsm_transition(FSM_WAIT_ADC, FSM_WAIT_TIMER);
		}
		break;
	case FSM_ADC_COMPLETE:
		break;
	}
}

/**
 * Converts a sensor to a useful PWM ratio.
 *
 * The maximum value so far will map to 32000 (approximately MAX_INT) and
 * zero will map to 0.
 */
word scale_value(word value) {
	static word max_value = 0;
	if (value > max_value) {
		max_value = value;
	}
	// Scale to a legal and useful PWM ratio
	word scaled_value = max_value == 0 ? 0.0 : (((float)value/(float)max_value) * 32000.0);
	return scaled_value;
}

/**
 * @brief Reports the value retrieved from the sensor
 *
 * Dumps the value to the terminal, and sets the green LED
 * to a brightness matching the value.
 */
void report_value(word value) {
	// Scale to a legal and useful PWM ratio
	word scaled_value = scale_value(value);
	Green_SetRatio16(scaled_value);

	// Print the raw and scaled DC values
	static int display_count = 0;
	if (++display_count >= 50) {
		Term1_SendStr((void*)"ADC raw=");
		Term1_SendNum(value);
		Term1_SendStr((void*)"; scaled=");
		Term1_SendNum((int)scaled_value);
		Term1_SendStr((void*)"\r\n");
		display_count = 0;
	}
}

/*
 * TEST_CONTROL
 * 0: No coroutine infrastructure
 * 1: 1 coroutine; 1 caller
 * 2: 2 coroutines; 2 callers
 * 3: 3 coroutines; 3 callers
 * 4: 3 coroutines; 5 callers
 * 5: 3 coroutines + 1 with 2x internals; 5 callers + 1 with 2x internals
 */

#define TEST_CONTROL 5

#if TEST_CONTROL >= 1

#include "experimental/coroutine.h"
#include "resumable.h"
using namespace std;

#endif

#if TEST_CONTROL >= 1

resumable fsm_coro() {
	Term1_SendStr((void*)"counter: called\r\n");
	for (unsigned i = 1;; i++) {
		co_await std::experimental::suspend_always { };
		Term1_SendStr((void*)"counter: resumed (#");
		Term1_SendNum(i);
		Term1_SendStr((void*)")\r\n");
	}
}

void test_coro() {
	auto coro = fsm_coro();
	for (int i = 0; i < 5; i++) {
		coro.resume();
	}
}

#endif

#if TEST_CONTROL >= 2

resumable fsm_coro2() {
	Term1_SendStr((void*)"coro2(): called\r\n");
	for (unsigned i = 1;; i++) {
		co_await std::experimental::suspend_always { };
		Term1_SendStr((void*)"coro2(): resumed (#");
		Term1_SendNum(i);
		Term1_SendStr((void*)")\r\n");
	}
}

void test_coro2() {
	auto coro = fsm_coro2();
	for (int i = 0; i < 5; i++) {
		coro.resume();
	}
}

#endif

#if TEST_CONTROL >= 3

resumable fsm_coro3() {
	Term1_SendStr((void*)"coro3(): called\r\n");
	for (unsigned i = 1;; i++) {
		co_await std::experimental::suspend_always { };
		Term1_SendStr((void*)"coro3(): resumed (#");
		Term1_SendNum(i);
		Term1_SendStr((void*)")\r\n");
	}
}

void test_coro3() {
	auto coro = fsm_coro3();
	for (int i = 0; i < 5; i++) {
		coro.resume();
	}
}

#endif

#if TEST_CONTROL >= 4

void test_coro3_1() {
	auto coro = fsm_coro3();
	for (int i = 0; i < 6; i++) {
		coro.resume();
	}
}

void test_coro3_2() {
	auto coro = fsm_coro3();
	for (int i = 0; i < 7; i++) {
		coro.resume();
	}
}

#endif

#if TEST_CONTROL >= 5

resumable fsm_coro4() {
	Term1_SendStr((void*)"coro4(): called\r\n");
	for (unsigned i = 1;; i++) {
		co_await std::experimental::suspend_always { };
		Term1_SendStr((void*)"coro4(): resumed (#");
		Term1_SendNum(i);
		Term1_SendStr((void*)")\r\n");
	}
	Term1_SendStr((void*)"coro4(): called\r\n");
	for (unsigned i = 1;; i++) {
		co_await std::experimental::suspend_always { };
		Term1_SendStr((void*)"coro4(): resumed (#");
		Term1_SendNum(i);
		Term1_SendStr((void*)")\r\n");
	}
}

void test_coro4() {
	auto coro = fsm_coro4();
	for (int i = 0; i < 5; i++) {
		coro.resume();
	}
}

#endif

void call_tests() {
#if TEST_CONTROL >= 1
	test_coro();
#endif
#if TEST_CONTROL >= 2
	test_coro2();
#endif
#if TEST_CONTROL >= 3
	test_coro3();
#endif
#if TEST_CONTROL >= 4
	test_coro3_1();
	test_coro3_2();
#endif
#if TEST_CONTROL >= 5
	test_coro4();
#endif
}
