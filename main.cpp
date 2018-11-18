#include <iostream>
#include <thread>
#include <chrono>
#include "Fifo.h"

enum StateT {
	STATE_IR_MEAS,
	STATE_SPDIF_MEAS
};

enum EventT {
    EVENT_CHANGE_STATE,
	//EVENT_START_IR_MEAS,
	EVENT_IR_COMMAND_RECEIVED,
	EVENT_IR_MEAS_READY,
	//EVENT_START_SPDIF_MEAS,
	EVENT_SPDIF_MEAS_READY,
	EVENT_BUTTON_PRESSED,
	EVENT_CHANGE_INPUT,
	EVENT_SET_VOLUME
};

typedef struct {
	EventT event;
	uint32_t data;
} message_t;

void wait_ms(int t) {
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}

class EventHandler {
public:
	static bool getNext(message_t *msg) {
		if (eventQueue->empty()) {
			return false;
		}

		*msg = eventQueue->get();
		return true;
	}

	static void postEvent(EventT event, uint32_t data = 0) {
		message_t msg;
		msg.event = event;
		msg.data = data;
		eventQueue->put(msg);
	}

	static Fifo<message_t> *eventQueue;

private:
	EventHandler();
};

class State {
public:
    virtual void OnStateExecution(message_t msg) {}
    virtual void OnStateEnter() {}
    virtual void OnStateExit() {}
};

class IrState: public State {
public:
    virtual void OnStateEnter() {
        std::cout << "Enter IR state\n";
    }

    virtual void OnStateExecution(message_t msg) {
        static int input_counter = 0;

        switch (msg.event) {
            case EVENT_IR_COMMAND_RECEIVED:
                std::cout << "IR command received\n";
                EventHandler::postEvent(EVENT_SET_VOLUME, 35);
                break;
            case EVENT_IR_MEAS_READY:
                EventHandler::postEvent(EVENT_CHANGE_STATE, STATE_SPDIF_MEAS);
                break;
            case EVENT_SET_VOLUME:
                std::cout << "Set volume " << msg.data << std::endl;
                break;
            case EVENT_CHANGE_INPUT:
                std::cout << "SPDIF input changed " << msg.data << std::endl;
                break;
            case EVENT_BUTTON_PRESSED:
                EventHandler::postEvent(EVENT_CHANGE_INPUT, input_counter++);
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
                EventHandler::postEvent(EVENT_CHANGE_STATE, STATE_IR_MEAS);
                break;
        }
    }

    virtual void OnStateExit() {
        std::cout << "Exit SPDIF state\n";
    }
};

class StateMachine {
public:
    StateMachine() {
        _currentState = &_spdifState;
        _currentState->OnStateEnter();
    }

    void changeState(State *nextState) {
        _currentState->OnStateExit();
        nextState->OnStateEnter();
        _currentState = nextState;
    }

    void update() {
        message_t msg;
    	if (!EventHandler::getNext(&msg)) {
    		return;
    	}

        if (msg.event == EVENT_CHANGE_STATE) {
            switch(static_cast<EventT>(msg.data)) {
            case STATE_IR_MEAS:
                changeState(&_irState);
                break;
            case STATE_SPDIF_MEAS:
                changeState(&_spdifState);
                break;
            }
            return;
        }
        _currentState->OnStateExecution(msg);
    }

private:
    IrState _irState;
    SpdifState _spdifState;
    State *_currentState;
};

Fifo<message_t> *EventHandler::eventQueue = new Fifo<message_t>(4);

void ticker_isr() {
    static int counter = 0;

    while (1) {
        switch (counter) {
        case 2:
            EventHandler::postEvent(EVENT_IR_COMMAND_RECEIVED);
            break;
        case 3:
            EventHandler::postEvent(EVENT_BUTTON_PRESSED);
            break;
        break;
        case 4:
            EventHandler::postEvent(EVENT_IR_MEAS_READY);
            break;
        case 5:
            EventHandler::postEvent(EVENT_SPDIF_MEAS_READY);
            counter = 0;
            break;
        }

        wait_ms(1000);
        ++counter;
    }
}

int main() {
    StateMachine fsm;

    EventHandler::postEvent(EVENT_SPDIF_MEAS_READY);

    std::thread t1(&ticker_isr);

    while (1) {
        fsm.update();
    }

    return 0;
}
