/*
 * app_events.cpp
 *
 *  Created on: 9 Oct 2018
 *      Author: Bruce Belson
 */

#include "app_events.h"
#include <string.h>

struct callback_reg {
	app_event_callback_t callback;
	callback_reg* next;
	callback_reg* prev;
};

#define MAX_CALLBACKS 256

callback_reg the_heap[MAX_CALLBACKS];
callback_reg* root_array[APP_EVENT_COUNT];

static bool app_event_init_done = false;
static int max_app_event_id = -1;
static int min_non_root_index = MAX_CALLBACKS;

static void app_event_init() {
	if (app_event_init_done)
		return;
	app_event_init_done  = true;
	memset(the_heap, 0, sizeof(the_heap));
	memset(root_array, 0, sizeof(root_array));
}

static callback_reg* find_reg(int event_id) {
	callback_reg* p = (event_id >= 0 && event_id < APP_EVENT_COUNT) ? root_array[event_id] : 0;
	while (p && p->next) {
		p = p->next;
	}
	return p;
}

static callback_reg* get_next_reg() {
	callback_reg* p;
	int i;
	for (i = 0, p = &the_heap[0]; i < MAX_CALLBACKS; i++, p++) {
		if (!p->callback) {
			return p;
		}
	}
	return 0;
}

extern "C"
bool app_event_reg_isr(int event_id, app_event_callback_t callback) {
	app_event_init();
	callback_reg* p = find_reg(event_id);
	if (!p) {
		p = get_next_reg();
		if (!p) {
			return false;
		}
		p->prev = p->next = 0;
		p->callback = callback;
		root_array[event_id] = p;
	}
	else {
		callback_reg* pn = get_next_reg();
		if (!pn) {
			return false;
		}
		p->next = pn;
		pn->prev = p;
		pn->callback = callback;
	}
	return true;
}

extern "C"
bool app_event_unreg_isr(int event_id, app_event_callback_t callback) {
	app_event_init();
	callback_reg* p = find_reg(event_id);
	if (p && (p->callback == callback)) {
		p->callback = 0;
		if (p->prev) {
			p->prev->next = 0;
			p->prev = 0;
		}
		else {
			root_array[event_id] = 0;
		}
		return true;
	}
	else {
		return false;
	}
}

extern "C"
void app_event_invoke_isr(int event_id, bool unregister) {
	app_event_init();
	callback_reg* p = find_reg(event_id);
	if (p) {
		p->callback();
		if (unregister) {
			app_event_unreg_isr(event_id, p->callback);
		}
	}
}

