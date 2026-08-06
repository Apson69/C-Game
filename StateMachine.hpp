#pragma once

#include<memory>
#include<stack>
#include "State.hpp"

namespace CPP {

    typedef std::unique_ptr<State> StateRef;
    class StateMachine {
        public:
        StateMachine(){};
        ~StateMachine(){};

        void AddState(StateRef state, bool isReplacing=true);
        void RemoveState();
        void ProcessStateChanges();
    };
}