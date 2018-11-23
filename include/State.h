#ifndef STATE_H
#define STATE_H

#include "EventQueue.h"

class State {
public:
    virtual void onStateExecution(message_t msg) {}
    virtual void onStateEnter() {}
    virtual void onStateExit() {}

protected:
    State();
    void changeState(State *nextState);
    void post(EventT event, uint32_t data = 0);
};

#endif
