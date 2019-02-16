#ifndef COMMON_H
#define COMMON_H

#ifdef OPENGL_ES // Including files for OpenGL ES
#include <GLES3/gl3.h>
#include <emscripten/emscripten.h>
#else // Including files for OpenGL
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <signal.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>

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

Time tetra_time(TIME_USE);

class Print
{
  public:
    static void mat4(glm::mat4 mat)
    {
        for (int i = 0; i < 4; i++)
        {
            printf("\t");
            for (int j = 0; j < 4; j++)
            {
                printf("%4.3f\t", mat[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    static void vec4(glm::vec4 vec)
    {
        std::cout << "\t" << vec.x << "\n"
                  << "\t" << vec.y << "\n"
                  << "\t" << vec.z << "\n"
                  << "\t" << vec.w << std::endl;
    }

    static void vec3(glm::vec3 vec)
    {
        std::cout << "\t" << vec.x << "\n"
                  << "\t" << vec.y << "\n"
                  << "\t" << vec.z << std::endl;
    }

    static void array(float *arr, int dim1, int dim2 = 1)
    {
        for (int i = 0; i < dim1; i++)
        {
            for (int j = 0; j < dim2; j++)
            {
                printf("%.4f ", arr[i * dim2 + j]);
            }
        }
        printf("\n");
    }

    static void array(unsigned int *arr, int dim1, int dim2 = 1)
    {
        for (int i = 0; i < dim1; i++)
        {
            for (int j = 0; j < dim2; j++)
            {
                printf("%u ", arr[i * dim2 + j]);
            }
        }
        printf("\n");
    }

    static void array(glm::vec3 *arr, int dim1, int dim2 = 1)
    {
        for (int i = 0; i < dim1; i++)
        {
            for (int j = 0; j < dim2; j++)
            {
                printf("%.4f ", arr[i * dim2 + j].x);
                printf("%.4f ", arr[i * dim2 + j].y);
                printf("%.4f ", arr[i * dim2 + j].z);
                printf("\n");
            }
            printf("\n");
        }
    }

    static void array(glm::vec3 **arr, int dim1, int dim2 = 1)
    {
        for (int i = 0; i < dim1; i++)
        {
            for (int j = 0; j < dim2; j++)
            {
                printf("%.4f ", arr[i][j].x);
                printf("%.4f ", arr[i][j].y);
                printf("%.4f ", arr[i][j].z);
                printf("\n");
            }
            printf("\n");
        }
    }

    static void array(std::vector<std::vector<glm::vec3>> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            for (unsigned int j = 0; j < arr.at(i).size(); j++)
            {
                printf("%.4f ", arr.at(i).at(j).x);
                printf("%.4f ", arr.at(i).at(j).y);
                printf("%.4f ", arr.at(i).at(j).z);
                printf("\n");
            }
            printf("\n");
        }
    }

    static void array(std::vector<glm::vec3> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            printf("%.4f ", arr.at(i).x);
            printf("%.4f ", arr.at(i).y);
            printf("%.4f ", arr.at(i).z);
            printf("\n");
        }
        printf("\n");
    }

    static void array(std::vector<unsigned int> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            printf("%u ", arr.at(i));
            printf("\n");
        }
        printf("\n");
    }

    static void array(std::vector<float> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            printf("%.4f ", arr.at(i));
            printf("\n");
        }
        printf("\n");
    }
};

#endif