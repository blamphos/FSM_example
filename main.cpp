#include <iostream>
#include <thread>
#include <chrono>
#include "GlobalDefinitions.h"
#include "StateMachine.h"

void wait_ms(int t) {
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}

class IrState: public State {
public:
    virtual void OnStateEnter() {
        std::cout << "Enter IR state\n";
    }

    virtual void OnStateExecution(message_t msg) {
        static int input_counter = 0;
        static int volume = 0;

        switch (msg.event) {
            case EVENT_IR_COMMAND_RECEIVED:
                std::cout << "IR command received\n";
                EventQueue::post(EVENT_SET_VOLUME, volume % 99);
                volume++;
                break;
            case EVENT_IR_MEAS_READY:
                EventQueue::post(EVENT_CHANGE_STATE, STATE_SPDIF_MEAS);
                break;
            case EVENT_SET_VOLUME:
                std::cout << "Set volume " << msg.data << std::endl;
                break;
            case EVENT_CHANGE_INPUT:
                std::cout << "SPDIF input changed " << msg.data << std::endl;
                break;
            case EVENT_BUTTON_PRESSED:
                EventQueue::post(EVENT_CHANGE_INPUT, input_counter++);
                if (input_counter > 2) {
                    input_counter = 0;
                }
                break;
        }
    }

    virtual void OnStateExit() {
        std::cout << "Exit IR state\n";
    }
};

class SpdifState: public State {
public:
    virtual void OnStateEnter() {
        std::cout << "Enter SPDIF state\n";
    }

    virtual void OnStateExecution(message_t msg) {
        switch (msg.event) {
            case EVENT_SPDIF_MEAS_READY:
                EventQueue::post(EVENT_CHANGE_STATE, STATE_IR_MEAS);
                break;
            default:
                break;
        }
    }

    virtual void OnStateExit() {
        std::cout << "Exit SPDIF state\n";
    }
};

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

        wait_ms(50);
        ++counter;
    }
}

int main() {
    StateMachine fsm;
    IrState irState;
    SpdifState spdifState;

    fsm.changeState(&spdifState);
    EventQueue::post(EVENT_SPDIF_MEAS_READY);

    std::thread t1(&ticker_isr);

    while (1) {
        message_t msg = EventQueue::get();

        switch (msg.event) {
        case EVENT_IDLE:
            wait_ms(10);
            break;
        case EVENT_CHANGE_STATE:
            switch(static_cast<EventT>(msg.data)) {
            case STATE_IR_MEAS:
                fsm.changeState(&irState);
                break;
            case STATE_SPDIF_MEAS:
                fsm.changeState(&spdifState);
                break;
            }
            break;
        default:
            fsm.update(msg);
            break;
        }
    }

    return 0;
}
