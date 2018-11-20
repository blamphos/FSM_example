#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "State.h"

class StateMachine {
public:
    void dispatch(message_t msg);
    void changeState(State *nextState);
    static StateMachine *instance();

private:
    StateMachine();
    State *_currentState;
};

#endif
