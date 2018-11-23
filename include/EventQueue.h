#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <iostream>

enum EventT {
	EVENT_IR_COMMAND_RECEIVED,
	EVENT_IR_MEAS_READY,
	EVENT_SPDIF_MEAS_READY,
	EVENT_BUTTON_PRESSED,
	EVENT_CHANGE_INPUT,
	EVENT_SET_VOLUME
};

typedef struct {
	EventT event;
	uint32_t data;
} message_t;

#endif
