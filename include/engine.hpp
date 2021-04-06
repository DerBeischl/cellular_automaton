#pragma once
#include <cassert>
#include <memory>
#include "SFML/Graphics.hpp"
#include "state.hpp"

namespace cellular_automaton
{
    template<typename state_type>
    class Engine
    {
        public:
        using map_type = sf::Color (*)(
                        const State<state_type>& state,
                        const size_t x, const size_t y);

        explicit Engine(map_type map_function, const size_t width, const size_t height)
            : map_function_(map_function) 
        {
            assert ((tex_.create(width, height)));
            img_.create(width, height);
            sprite_.setTexture(tex_);
        }

        const sf::Sprite& step(const State<state_type>& state)
        {
            for(size_t y = 0; y < state.height(); y++)
                for(size_t x = 0; x < state.width(); x++)
                    img_.setPixel(x, y, map_function_(state, x, y));

            tex_.update(img_); 
            return sprite_;
        }

        private:
            sf::Image img_;
            sf::Texture tex_;
            sf::Sprite sprite_;
            map_type map_function_;
    };
}