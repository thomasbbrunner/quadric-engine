#ifndef TIME_H
#define TIME_H

#include <common.h>

#include <GLFW/glfw3.h>

#define TIME_REAL 1
#define TIME_TICK 2

#define TIME_USE TIME_TICK

class Time
{
  public:
    double time_real = 0.0;
    double time_tick = 0.0;
    double time_tick_step = 1.0 / 60.0;
    short type = 0;

    Time(short type_in)
    {
        if (type_in == TIME_REAL || type_in == TIME_TICK)
            type = type_in;
        else
        {
            printf("error in time\n");
            exit(0);
        }
    }

    double get()
    {
        if (type == TIME_REAL)
            return time_real;
        else if (type == TIME_TICK)
            return time_tick;
        else
            return -1.0;
    }

    void update()
    {
        time_real = glfwGetTime();
        time_tick += time_tick_step;
    }
};

Time tiktok(TIME_USE);

#endif