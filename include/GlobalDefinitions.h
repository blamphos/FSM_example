#ifndef GLOBALDEFINITIONS_H
#define GLOBALDEFINITIONS_H

#include "EventQueue.h"

enum StateT {
	STATE_IR_MEAS,
	STATE_SPDIF_MEAS
};

class State {
public:
    virtual void onStateExecution(message_t msg) {}
    virtual void onStateEnter() {}
    virtual void onStateExit() {}

    void setNextState(State *state) {
        _nextState = state;
    }

    State *getNextState() {
        return _nextState;
    }

private:
    State *_nextState;
};

#endif
