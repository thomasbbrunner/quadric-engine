
#pragma once

#include "api.h"
#include "error.hpp"
#include "print.h"

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
    enum class TimeType
    {
        REAL,
        TICKS,
    };

private:
    double tiktok_wall_ = 0.0;
    double tiktok_tick_ = 0.0;
    double tiktok_tick_step_ = 1.0 / 60.0;
    enum TimeType time_type_ = TimeType::TICKS;

public:
    void set_type(enum TimeType type)
    {
        time_type_ = type;
    }

    double get()
    {
        if (time_type_ == TimeType::TICKS)
        {
            return tiktok_tick_;
        }
        else if (time_type_ == TimeType::TICKS)
        {
            return tiktok_wall_;
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
