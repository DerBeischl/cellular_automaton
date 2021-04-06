#pragma once

#include "state.hpp"

namespace cellular_automaton
{
class Rule
{
  public:
    using state_type = bool;

    void step(State<state_type> &state);
};
} // namespace cellular_automaton