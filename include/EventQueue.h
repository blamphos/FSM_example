#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <iostream>
#include "Fifo.h"

enum EventT {
    EVENT_IDLE,
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

class EventQueue {
public:
	message_t get();
	void post(EventT event, uint32_t data = 0);
	static EventQueue *instance();

private:
	EventQueue(); // Prevent creating object

	Fifo<message_t> *_fifo;
};

#endif
