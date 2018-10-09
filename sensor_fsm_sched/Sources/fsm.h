/*
 * fsm.h
 *
 *  Created on: 18 Jul 2018
 *      Author: belsonba
 */

#ifndef SOURCES_FSM_H_
#define SOURCES_FSM_H_

#ifndef __PE_Types_H
#include "PE_Types.h"
#endif

#ifdef __cplusplus
#ifndef SCHEDULER_SOURCES_H
#include "scheduler.h"
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Shared state variable
typedef enum {
	FSM_UNINITIALISED = 0,
	FSM_WAIT_CALIBRATION,
	FSM_CALIBRATION_COMPLETE,
	FSM_WAIT_TIMER,
	FSM_TIMER_COMPLETE,
	FSM_WAIT_ADC,
	FSM_ADC_COMPLETE
} fsm_state_t;
extern volatile fsm_state_t fsm_state;

// Shared data buffer
extern word sensor_value;

// Finite state machine
bool fsm_execute();

// FSM transition helper
bool fsm_transition(fsm_state_t from, fsm_state_t to);

// Interrupt handlers
void fsm_handle_calibration_complete();
void fsm_handle_timer_interrupt();
void fsm_handle_adc_complete();

#ifdef __cplusplus
} // extern "C"
#endif

// Task adapter
#ifdef __cplusplus
extern scheduling::task_control_block_t sensor_task;
#endif

#endif /* SOURCES_FSM_H_ */
