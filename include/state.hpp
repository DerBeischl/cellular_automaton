#pragma once
#include <cstddef>
#include <memory>

#include <iostream>

namespace cellular_automaton
{

/**
 * @brief
 *
 * @tparam T
 */
template <typename T> class State
{
  public:
    /**
     * @brief Construct a new State object
     *
     * @param width
     * @param height
     */
    State(const size_t width, const size_t height)
        : width_(width), height_(height),
          buffer_(std::shared_ptr<T>(new T[width * height],
                                     std::default_delete<T[]>())),
          swap_buffer_(std::shared_ptr<T>(new T[width * height],
                                          std::default_delete<T[]>()))
    {
        std::fill(buffer_.get(), buffer_.get() + width * height, 0);
        std::fill(swap_buffer_.get(), swap_buffer_.get() + width * height, 0);
    }

    size_t width() const { return width_; }

    size_t height() const { return height_; }

    /**
     * @brief
     *
     * @param x
     * @param y
     * @return T&
     */
    T &operator()(const size_t x, const size_t y)
    {
        return buffer_.get()[x + y * width_];
    }

    /**
     * @brief
     *
     * @param x
     * @param y
     * @return const T&
     */
    const T &operator()(const size_t x, const size_t y) const
    {
        return buffer_.get()[x + y * width_];
    }

    T &operator[](const size_t i) { return buffer_.get()[i]; }

    const T &operator[](const size_t i) const { return buffer_.get()[i]; }

    T &back(const size_t x, const size_t y)
    {
        return swap_buffer_.get()[x + y * width_];
    }

    const T &back(const size_t x, const size_t y) const
    {
        return swap_buffer_.get()[x + y * width_];
    }

    T &back(const size_t i) { return swap_buffer_.get()[i]; }

    const T &back(const size_t i) const { return swap_buffer_.get()[i]; }

    /**
     * @brief
     *
     */
    void swap()
    {
        std::swap(buffer_, swap_buffer_);
        // std::cout << buffer_.get() << " " << swap_buffer_.get() << "\n";
    }

  private:
    size_t width_, height_;
    std::shared_ptr<T> buffer_, swap_buffer_;
};
} // namespace cellular_automaton