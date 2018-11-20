#ifndef SPDIFSTATE_H
#define SPDIFSTATE_H

#include "State.h"

class SpdifState: public State {
public:
    virtual void onStateEnter();
    virtual void onStateExecution(message_t msg);
    virtual void onStateExit();
    static State *instance();

private:
    SpdifState();
};

#endif
