
#pragma once

#include "api.hpp"
#include "error.hpp"
#include "print.hpp"

#include <exception>

class Tiktok
{
    // Singleton initialization
public:
    static Tiktok &get_instance()
    {
        static Tiktok instance;
        return instance;
    }

    Tiktok(Tiktok const &) = delete;
    void operator=(Tiktok const &) = delete;

private:
    Tiktok() {}
    // End of Singleton initialization

public:
    enum class Type
    {
        REAL,
        TICKS,
    };

private:
    double tiktok_wall_ = 0.0;
    double tiktok_tick_ = 0.0;
    double tiktok_tick_step_ = 1.0 / 60.0;
    enum Type time_type_ = Type::TICKS;

public:
    void set_type(enum Type type)
    {
        time_type_ = type;
    }

    // templated to allow use of normal and double precision time values
    // most of use-cases only accept floats (glm), so it is the standard type
    template <typename T = float>
    T get()
    {
        if (time_type_ == Type::TICKS)
        {
            return (T)tiktok_tick_;
        }
        else if (time_type_ == Type::TICKS)
        {
            return (T)tiktok_wall_;
        }
        else
        {
            throw quad::fatal_error("Unknown time type");
        }
    }

    void update()
    {
        tiktok_wall_ = glfwGetTime();
        tiktok_tick_ += tiktok_tick_step_;
    }
};
