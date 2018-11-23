#include <iostream>
#include <thread>
#include <chrono>

#include "StateMachine.h"

void wait_ms(int t) {
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}

void ticker_isr() {
    static int counter = 0;

    while (1) {
        switch (counter) {
        case 2:
            StateMachine::instance()->post(EVENT_IR_COMMAND_RECEIVED);
            break;
        case 3:
            StateMachine::instance()->post(EVENT_BUTTON_PRESSED);
            break;
        break;
        case 4:
            StateMachine::instance()->post(EVENT_IR_MEAS_READY);
            break;
        case 5:
            StateMachine::instance()->post(EVENT_SPDIF_MEAS_READY);
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
        StateMachine::instance()->dispatch();

        wait_ms(10);
    }

    return 0;
}
