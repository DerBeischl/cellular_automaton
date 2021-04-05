
#include "engine.hpp"
#include "state.hpp"
#include "rule.hpp"
#include "memory"
#include <iostream>

using namespace gol;

int main(int argc, char** argv)
{
    assert((argc == 3));

    int grid_width = atoi(argv[1]);
    int grid_height = atoi(argv[2]);

    sf::RenderWindow window(sf::VideoMode(grid_width , grid_height ), "Game of Life");
    window.setSize(sf::Vector2u(1500,1500));
    Rule rule;
    State<Rule::state_type> state(grid_width, grid_height);

    state(51,51) = 1; 
    state(52,51) = 1; 
    state(51,52) = 1; 
    state(53,54) = 1; 
    state(54,54) = 1; 
    state(54,53) = 1; 

    state(32,31) = 1;
    state(33,32) = 1; 
    state(31,33) = 1; 
    state(32,33) = 1; 
    state(33,33) = 1; 

    state(47,31) = 1; 
    state(48,31) = 1; 
    state(49,31) = 1; 
    state(47,32) = 1; 
    state(49,32) = 1; 
    state(47,33) = 1; 
    state(49,33) = 1; 
    state(47,35) = 1; 
    state(49,35) = 1; 
    state(47,36) = 1; 
    state(49,36) = 1; 
    state(47,36) = 1; 
    state(48,36) = 1; 
    state(49,36) = 1; 

    auto bool_to_color = [](const State<Rule::state_type>& state,
                            const size_t x, const size_t y)
    {
        return sf::Color(state(x, y) * 255, state(x, y) * 255, state(x, y) * 255);
    };

    Engine<Rule::state_type> engine(+bool_to_color, grid_width, grid_height);

    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        rule.step(state);
        sf::sleep(sf::seconds(.1f)); 

        window.clear();
        window.draw(engine.step(state));
        window.display(); 

    }


}