#include "StateMachine.h"
#include "SpdifState.h"

StateMachine::StateMachine() {
    _currentState = SpdifState::instance();
    _currentState->onStateEnter();
}

void StateMachine::dispatch(message_t msg) {
    _currentState->onStateExecution(msg);
}

void StateMachine::changeState(State *nextState) {
    _currentState->onStateExit();
    nextState->onStateEnter();
    _currentState = nextState;
}

StateMachine *StateMachine::instance() {
    static StateMachine inst;
    return &inst;
}
