/*
 * fsm.h
 *
 *  Created on: 18 Jul 2018
 *      Author: belsonba
 */

#ifndef SOURCES_FSM_H_
#define SOURCES_FSM_H_

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

#endif /* SOURCES_FSM_H_ */
