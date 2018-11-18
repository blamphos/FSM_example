#ifndef GLOBALDEFINITIONS_H
#define GLOBALDEFINITIONS_H

#include "EventQueue.h"

enum StateT {
	STATE_IR_MEAS,
	STATE_SPDIF_MEAS
};

class State {
public:
    virtual void OnStateExecution(message_t msg) {}
    virtual void OnStateEnter() {}
    virtual void OnStateExit() {}
};

#endif // GLOBALDEFINITIONS_H
