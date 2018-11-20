#ifndef IRSTATE_H
#define IRSTATE_H

#include "State.h"

class IrState: public State {
public:
    virtual void onStateEnter();
    virtual void onStateExecution(message_t msg);
    virtual void onStateExit();
    static State *instance();

private:
    IrState();
};

#endif
