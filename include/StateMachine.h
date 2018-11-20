#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "SpdifState.h"

class StateMachine {
public:
    StateMachine() {
        _currentState = SpdifState::instance();
        _currentState->onStateEnter(this);
    }

    void dispatch(message_t msg) {
        _currentState->onStateExecution(this, msg);
    }

    void changeState(State *nextState) {
        _currentState->onStateExit(this);
        nextState->onStateEnter(this);
        _currentState = nextState;
    }

private:
    State *_currentState;
};

#endif
