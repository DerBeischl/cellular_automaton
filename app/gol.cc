
#include "engine.hpp"
#include "state.hpp"
#include "rule.hpp"
#include "memory"
#include <iostream>
#include <random>

using namespace gol;

int main(int argc, char **argv)
{
    assert(((argc >= 3) || (argc <= 5)));

    int grid_width = atoi(argv[1]);
    int grid_height = atoi(argv[2]);
    int grid_size = 15;
    float state_durration = .1f;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 1);

    if (argc >= 4)
        grid_size = atoi(argv[3]);

    if (argc == 5)
        state_durration = atof(argv[4]);

    sf::RenderWindow window(sf::VideoMode(grid_width, grid_height), "Game of Life");
    window.setSize(sf::Vector2u(grid_width * grid_size, grid_height * grid_size));
    Rule rule;
    State<Rule::state_type> state(grid_width, grid_height);

    auto bool_to_color = [](const State<Rule::state_type> &state,
                            const size_t x, const size_t y) {
        return sf::Color(state(x, y) * 255, state(x, y) * 255, state(x, y) * 255);
    };

    Engine<Rule::state_type> engine(+bool_to_color, grid_width, grid_height);

    bool wait = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Allows keyboard input durring use 
            // Space: Start/Stop iteration, at the beginning it is set to Stop
            // C = Clear: all cells will be set to 0 
            // R = Random: every cell will be assigned either 1 or 0
            if (event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Space: 
                        wait = !wait; 
                        break; 
                    
                    case sf::Keyboard::C: 
                        for (int i = 0; i < grid_width * grid_height; i++)
                            state[i] = 0;
                        window.clear();
                        window.draw(engine.step(state));
                        window.display();
                        break; 
                    
                    case sf::Keyboard::R: 
                        for (size_t i = 0; i < state.width(); i++)
                            for (size_t k = 0; k < state.height(); k++)
                                state(i, k) = distribution(generator);

                        window.clear();
                        window.draw(engine.step(state));
                        window.display();
                        break; 
                    
                    default: 
                        break; 
                }
            }

            // Draw cells
            // left mouse button: set cell to 1 
            // right mouse button: set cell to 0 
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i point = sf::Mouse::getPosition(window);
                    state(point.x / grid_size, point.y / grid_size) = 1;
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2i point = sf::Mouse::getPosition(window);
                    state(point.x / grid_size, point.y / grid_size) = 0;
                }
                window.clear();
                window.draw(engine.step(state));
                window.display();
            }
        }
        if (wait)
            continue;

        rule.step(state);
        sf::sleep(sf::seconds(state_durration));

        window.clear();
        window.draw(engine.step(state));
        window.display();
    }
}