#include "rule.hpp"
#include <iostream>

namespace cellular_automaton
{
    size_t mod(int a, int b)
    {
        return (a % b + b) % b; 
    }

    void Rule::step(State<Rule::state_type>& state)
    {
        for (size_t y = 0; y < state.height(); ++y)
            for (size_t x = 0; x < state.width(); ++x)
            {
                int count = -state(x, y);
                for (int j = -1; j < 2; ++j)
                    for (int i = -1; i < 2; ++i)
                        count += state(mod(x + i, state.width()), mod(y+j, state.height()));
            
                if(!state(x,y))
                {
                    if(count==3)
                        state.back(x,y) = 1; 
                    else
                        state.back(x,y) = 0; 
                } 
                else if(state(x,y)) 
                {
                    if(count < 2)
                        state.back(x,y) = 0; 
                    else if(count < 4)
                        state.back(x,y) = 1; 
                    else if(count >= 4)
                        state.back(x,y) = 0;  
                }
                    
            }

        state.swap();
    }

}