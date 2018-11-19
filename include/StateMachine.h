#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "EventQueue.h"
#include "State.h"
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

private:
    friend class State;
    State *_currentState;

    void changeState(State *nextState) {
        _currentState->onStateExit(this);
        nextState->onStateEnter(this);
        _currentState = nextState;
    }
};

#endif
