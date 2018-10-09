/*
 * app_events.h
 *
 *  Created on: 9 Oct 2018
 *      Author: Bruce Belson
 */

#ifndef SOURCES_APP_EVENTS_H_
#define SOURCES_APP_EVENTS_H_

#ifdef __cplusplus
extern "C" {
#endif

enum {
	APP_EVENT_NONE = -1,
	APP_EVENT_NMI,
	APP_EVENT_TI1,
	APP_EVENT_AD1_CALIBRATED,
	APP_EVENT_AD1_MEASURED,

	APP_EVENT_COUNT
};

typedef void (*app_event_callback_t)(void);

bool app_event_reg_isr(int event_id, app_event_callback_t callback);
bool app_event_unreg_isr(int event_id, app_event_callback_t callback);
void app_event_invoke_isr(int event_id, bool unregister);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* SOURCES_APP_EVENTS_H_ */
