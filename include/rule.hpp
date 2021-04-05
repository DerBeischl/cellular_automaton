#pragma once 

#include "state.hpp"

namespace gol
{
    class Rule
    {
        public:
            using state_type = bool;

            void step(State<state_type>& state);

    };
}; 