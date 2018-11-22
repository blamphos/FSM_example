#include "SpdifState.h"
#include "IrState.h"
#include "EventQueue.h"

SpdifState::SpdifState()
{
    //ctor
}

void SpdifState::onStateEnter() {
    std::cout << "Enter IR state\n";
    //ticker.attach_us(&ticker_isr, IR_SAMPLING_PERIOD_US);
    //spdifDecoder.toggleNextInput();
    //ir_receiver.startMeas();
}

void SpdifState::onStateExecution(message_t msg) {
    static int input_counter = 0;
    static int volume = 0;

    switch (msg.event) {
        case EVENT_IR_COMMAND_RECEIVED:
            std::cout << "IR command received\n";
            EventQueue::instance()->post(EVENT_SET_VOLUME, volume % 100);
            volume++;
            break;
        case EVENT_IR_MEAS_READY:
            //EventQueue::post(EVENT_CHANGE_STATE);
            changeState(SpdifState::instance());
            break;
        case EVENT_SET_VOLUME:
            std::cout << "Set volume " << msg.data << std::endl;
            break;
        case EVENT_CHANGE_INPUT:
            std::cout << "SPDIF input changed " << msg.data << std::endl;
            break;
        case EVENT_BUTTON_PRESSED:
            EventQueue::instance()->post(EVENT_CHANGE_INPUT, input_counter++);
            if (input_counter > 2) {
                input_counter = 0;
            }
            break;
    }
}

void SpdifState::onStateExit() {
    std::cout << "Exit IR state\n";
    //ticker.detach();
}

State *SpdifState::instance() {
    static SpdifState inst;
    return &inst;
}

