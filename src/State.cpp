#include "State.h"
#include "StateMachine.h"

State::State() {

}

void State::changeState(State *nextState) {
    StateMachine::instance()->changeState(nextState);
}

void State::post(EventT event, uint32_t data) {
    StateMachine::instance()->post(event, data);
}
