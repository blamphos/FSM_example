#ifndef STATE_H
#define STATE_H

#include <iostream>
#include "EventQueue.h"

class StateMachine;

class State {
public:
    virtual void onStateExecution(message_t msg) {}
    virtual void onStateEnter() {}
    virtual void onStateExit() {}

protected:
    State();
    void changeState(State *nextState);
};

#endif
