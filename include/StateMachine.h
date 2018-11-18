#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "GlobalDefinitions.h"

class StateMachine {
public:
    StateMachine() : _currentState(NULL) {
    }

    void changeState(State *nextState) {
        if (_currentState != NULL) {
            _currentState->OnStateExit();
        }
        nextState->OnStateEnter();
        _currentState = nextState;
    }

    void update(message_t msg) {
        _currentState->OnStateExecution(msg);
    }

private:
    State *_currentState;
};

#endif // STATEMACHINE_H
