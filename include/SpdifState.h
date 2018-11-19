#ifndef SPDIFSTATE_H
#define SPDIFSTATE_H

#include "State.h"

class SpdifState: public State {
public:
    SpdifState();
    virtual void onStateEnter(StateMachine *fsm);
    virtual void onStateExecution(StateMachine *fsm, message_t msg);
    virtual void onStateExit(StateMachine *fsm);
    static State *instance();
};


#endif
