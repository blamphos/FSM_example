#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "GlobalDefinitions.h"

class StateMachine {
public:
    StateMachine(State *initState) : _currentState(initState) {
        _currentState->onStateEnter();
    }

    void addState(State *newState, State *nextState) {
        newState->setNextState(nextState);
    }

    void dispatch(message_t msg) {
        if (msg.event == EVENT_CHANGE_STATE) {
            changeNextState();
        }
        else {
            _currentState->onStateExecution(msg);
        }
    }

private:
    State *_currentState;

    void changeNextState() {
        _currentState->onStateExit();
        State *nextState = _currentState->getNextState();
        nextState->onStateEnter();
        _currentState = nextState;
    }
};

#endif
