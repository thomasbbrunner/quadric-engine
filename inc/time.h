#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#define TIKTOK_REAL 1
#define TIKTOK_TICK 2

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
    double tiktok_wall = 0.0;
    double tiktok_tick = 0.0;
    double tiktok_tick_step = 1.0 / 60.0;
    short type = TIKTOK_TICK;

    void set_type(short type_in)
    {
        if (type_in == TIKTOK_REAL || type_in == TIKTOK_TICK)
            type = type_in;
        else
        {
            printf("error in time\n");
            exit(0);
        }
    }

    double get()
    {
        if (type == TIKTOK_REAL)
            return tiktok_wall;
        else if (type == TIKTOK_TICK)
            return tiktok_tick;
        else
            return -1.0;
    }

    void update()
    {
        tiktok_wall = glfwGetTime();
        tiktok_tick += tiktok_tick_step;
    }
};
