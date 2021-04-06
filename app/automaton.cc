
#include "argparse.hpp"
#include "engine.hpp"
#include "memory"
#include "rule.hpp"
#include "state.hpp"
#include <charconv>
#include <iostream>
#include <random>

using namespace cellular_automaton;

template <typename T>
bool integral_from_string_view(const std::string_view sv, T &res)
{

    // Do conversion and check success.
    return std::from_chars(sv.data(), sv.data() + sv.size(), res).ec ==
           std::errc::invalid_argument;
}

int main(int argc, char **argv)
{

    // Create argparser.
    Argparse parser(
        "Run a cellular automaton with a ruleset in an interactive window.",
        {{"-w", "Number of world grid width."},
         {"-h", "Number of world grid height."},
         {"-grid_size", "Grid cell size in visualization.", true},
         {"-state_duration", "Tick simulation speed in milliseconds.", true}});

    // Make sure args were parsed.
    if (not parser.parse(argc, argv))
        exit(1);

    // Parse values. Some optional values have defaults.
    int grid_width, grid_height, grid_size, state_duration;
    integral_from_string_view(parser.get_argument("-w").value(), grid_width);
    integral_from_string_view(parser.get_argument("-h").value(), grid_height);
    integral_from_string_view(parser.get_argument("-grid_size").value_or("15"),
                              grid_size);
    integral_from_string_view(
        parser.get_argument("-state-duration").value_or("100"), state_duration);

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 1);

    sf::RenderWindow window(sf::VideoMode(grid_width, grid_height),
                            "Game of Life");
    window.setSize(
        sf::Vector2u(grid_width * grid_size, grid_height * grid_size));
    Rule rule;
    State<Rule::state_type> state(grid_width, grid_height);

    auto bool_to_color = [](const State<Rule::state_type> &state,
                            const size_t x, const size_t y) {
        return sf::Color(state(x, y) * 255, state(x, y) * 255,
                         state(x, y) * 255);
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
                switch (event.key.code)
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
        sf::sleep(sf::milliseconds(state_duration));

        window.clear();
        window.draw(engine.step(state));
        window.display();
    }
}