#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

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
	static message_t get() {
		if (fifo->empty()) {
            message_t msg;
            msg.event = EVENT_IDLE;
            msg.data = 0;
            return msg;
		}

        return fifo->get();
	}

	static void post(EventT event, uint32_t data = 0) {
		message_t msg;
		msg.event = event;
		msg.data = data;
		fifo->put(msg);
	}

	static Fifo<message_t> *fifo;

private:
	EventQueue(); // Prevent creating object
};

#endif
