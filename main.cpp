#include <iostream>
#include <thread>
#include <chrono>

#include "StateMachine.h"

void wait_ms(int t) {
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}

Fifo<message_t> *EventQueue::fifo = new Fifo<message_t>(4);

void ticker_isr() {
    static int counter = 0;

    while (1) {
        switch (counter) {
        case 2:
            EventQueue::post(EVENT_IR_COMMAND_RECEIVED);
            break;
        case 3:
            EventQueue::post(EVENT_BUTTON_PRESSED);
            break;
        break;
        case 4:
            EventQueue::post(EVENT_IR_MEAS_READY);
            break;
        case 5:
            EventQueue::post(EVENT_SPDIF_MEAS_READY);
            counter = 0;
            break;
        }

        wait_ms(200);
        ++counter;
    }
}

int main() {
    std::cout << "Starting..." << std::endl;

    std::thread t1(&ticker_isr);

    while (1) {
        message_t msg = EventQueue::get();
        if (msg.event != EVENT_IDLE) {
            StateMachine::instance()->dispatch(msg);
        }
        wait_ms(10);
    }

    return 0;
}
