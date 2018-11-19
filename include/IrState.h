#ifndef IRSTATE_H
#define IRSTATE_H

#include "State.h"

class IrState: public State {
public:
    IrState();
    virtual void onStateEnter(StateMachine *fsm);
    virtual void onStateExecution(StateMachine *fsm, message_t msg);
    virtual void onStateExit(StateMachine *fsm);
    static State *instance();
};


#endif
