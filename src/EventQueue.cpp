#include "EventQueue.h"

EventQueue::EventQueue() {
    _fifo = new Fifo<message_t>(4);
}

message_t EventQueue::get() {
    if (_fifo->empty()) {
        message_t msg;
        msg.event = EVENT_IDLE;
        msg.data = 0;
        return msg;
    }

    return _fifo->get();
}

void EventQueue::post(EventT event, uint32_t data) {
    message_t msg;
    msg.event = event;
    msg.data = data;
    _fifo->put(msg);
}

EventQueue *EventQueue::instance() {
    static EventQueue inst;
    return &inst;
}
