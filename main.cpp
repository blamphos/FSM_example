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
    virtual void onStateEnter() {
        std::cout << "Enter IR state\n";
        //ticker.attach_us(&ticker_isr, IR_SAMPLING_PERIOD_US);
        //spdifDecoder.toggleNextInput();
        //ir_receiver.startMeas();
    }

    virtual void onStateExecution(message_t msg) {
        static int input_counter = 0;
        static int volume = 0;

        switch (msg.event) {
            case EVENT_IR_COMMAND_RECEIVED:
                std::cout << "IR command received\n";
                EventQueue::post(EVENT_SET_VOLUME, volume % 100);
                volume++;
                break;
            case EVENT_IR_MEAS_READY:
                EventQueue::post(EVENT_CHANGE_STATE);
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

    virtual void onStateExit() {
        std::cout << "Exit IR state\n";
        //ticker.detach();
    }
};

class SpdifState: public State {
public:
    virtual void onStateEnter() {
        std::cout << "Enter SPDIF state\n";
        //spdifDecoder.startMeas();
    }

    virtual void onStateExecution(message_t msg) {
        switch (msg.event) {
            case EVENT_SPDIF_MEAS_READY:
                EventQueue::post(EVENT_CHANGE_STATE);
                break;
            default:
                break;
        }
    }

    virtual void onStateExit() {
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

        wait_ms(200);
        ++counter;
    }
}

int main() {
    std::cout << "Starting..." << std::endl;

    IrState irState;
    SpdifState spdifState;

    StateMachine fsm(&spdifState);
    fsm.addState(&spdifState, &irState);
    fsm.addState(&irState, &spdifState);

    std::thread t1(&ticker_isr);

    while (1) {
        message_t msg = EventQueue::get();

        switch (msg.event) {
        case EVENT_IDLE:
            wait_ms(10);
            break;
        default:
            fsm.dispatch(msg);
            break;
        }
    }

    return 0;
}
