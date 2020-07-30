
#pragma once

#include "api.hpp"
#include "error.hpp"
#include "print.hpp"

#include <exception>

namespace qe
{
    class Time
    {
    public:
        enum class Type
        {
            WALL,
            TICKS,
        };

        Time(enum Type type) : type_(type)
        {
        }

    private:
        double wall_ = 0.0;
        double tick_ = 0.0;
        double tick_step_ = 1.0 / 60.0;
        enum Type type_;

    public:
        inline void set_type(enum Type type)
        {
            type_ = type;
        }

        // templated to allow use of normal and double precision time values
        // most of use-cases only accept floats (glm), so it is the standard type
        template <typename T = float>
        T get()
        {
            if (type_ == Type::TICKS)
            {
                return (T)tick_;
            }
            else if (type_ == Type::WALL)
            {
                return (T)wall_;
            }
            else
            {
                throw quad::fatal_error("Unknown time type");
            }
        }

        // steps can be negative or positive (fast forward time or revert time)
        // (only works when using ticks)
        inline void update(int steps = 1)
        {
            wall_ = glfwGetTime();
            tick_ += steps * tick_step_;
        }
    };
} // namespace qe
