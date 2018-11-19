#include "State.h"
#include "StateMachine.h"

State::State()
{
    //ctor
}

void State::changeState(StateMachine *fsm, State *nextState) {
    fsm->changeState(nextState);
}
