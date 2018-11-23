#include "StateMachine.h"
#include "SpdifState.h"

StateMachine::StateMachine() {
    _fifo = new Fifo<message_t>(4);

    _currentState = SpdifState::instance();
    _currentState->onStateEnter();
}

void StateMachine::dispatch() {
    if (!_fifo->empty()) {
        message_t msg = _fifo->get();
        _currentState->onStateExecution(msg);
    }
}

void StateMachine::changeState(State *nextState) {
    _currentState->onStateExit();
    nextState->onStateEnter();
    _currentState = nextState;
}

void StateMachine::post(EventT event, uint32_t data) {
    message_t msg;
    msg.event = event;
    msg.data = data;
    _fifo->put(msg);
}

StateMachine *StateMachine::instance() {
    static StateMachine inst;
    return &inst;
}
