#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "State.h"
#include "Fifo.h"

class StateMachine {
public:
    void dispatch();
    void changeState(State *nextState);
    void post(EventT event, uint32_t data = 0);
    static StateMachine *instance();

private:
    StateMachine();

    Fifo<message_t> *_fifo;
    State *_currentState;
};

#endif
