#include "State.h"
#include "StateMachine.h"

State::State()
{
    //ctor
}

void State::changeState(State *nextState) {
    StateMachine::instance()->changeState(nextState);
}
