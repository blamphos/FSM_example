#ifndef STATE_H
#define STATE_H

#include <iostream>
#include "EventQueue.h"

class StateMachine;

class State {
public:
    State();
    virtual void onStateExecution(StateMachine *fsm, message_t msg) {}
    virtual void onStateEnter(StateMachine *fsm) {}
    virtual void onStateExit(StateMachine *fsm) {}

protected:
    void changeState(StateMachine *fsm, State *nextState);
};

#endif
