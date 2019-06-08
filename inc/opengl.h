#pragma once

#include "api.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <signal.h>
#include <iostream>

// Toggle MSAA
#define MSAA

#define MSAA_SAMPLES 4
#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1080

// #define VIDEO_OUT

class OpenGL
{
    // Singleton initialization
public:
    static OpenGL &get_instance()
    {
        static OpenGL instance;
        return instance;
    }

    OpenGL(OpenGL const &) = delete;
    void operator=(OpenGL const &) = delete;

private:
    OpenGL()
    {
        int window_height = WINDOW_HEIGHT;
        int window_width = WINDOW_WIDTH;

        printf("--Initialising--\n");

        // Adding SIGINT handler
        // signal(SIGINT, signal_handler);

        // Initialising GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MSAA
        glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES);
#endif

        // Creating window
        window = glfwCreateWindow(window_width, window_height, "OpenGL", NULL, NULL); // Fullscreen: glfwGetPrimaryMonitor()
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            exit(0);
        }

        glfwMakeContextCurrent(window);

        // Initialising GLAD
#ifndef OPENGL_ES
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(0);
        }
#endif

        // For window resizing
        glfwSetFramebufferSizeCallback(window, this->framebuffer_resize_callback);

        // Setting window size and locationu
        glViewport(0, 0, window_width, window_height);

        // Enabling depth test
        glEnable(GL_DEPTH_TEST);

        // Enable MSAA
#ifdef MSAA
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_LINE_SMOOTH);
#endif

        // Debuging messages
        // glEnable(GL_DEBUG_OUTPUT);
        // glDebugMessageCallback();

        // Printing OpenGL version
        printf("%s\n", glGetString(GL_VERSION));

        // Recorder
#ifdef VIDEO_OUT
        start_recorder();
#endif
    }

public:
    GLFWwindow *window;

    static void framebuffer_resize_callback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    int window_height()
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return height;
    }

    int window_width()
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return width;
    }

    void update()
    {
        // Check if esc was pressed to quit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        check_error();

        // Check for events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();

#ifdef VIDEO_OUT
        record();
#endif

        // Clearing screen
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void terminate()
    {
        std::cout << "--Terminating--" << std::endl;
        glfwTerminate();
#ifdef VIDEO_OUT
        // pclose(ffmpeg);
#endif
        exit(0);
    }

    void mouse_input_callback(GLFWwindow *window, double xpos, double ypos)
    {
    }

    void check_error()
    {
        if (GLenum error = glGetError() != GL_NO_ERROR)
        {
            printf("Error occured: %d", error);
        }
    }

    // static void signal_handler(int signal = 0)
    // {
    //     glfwSetWindowShouldClose(gl_window, true);
    // }

    bool should_close()
    {
        return glfwWindowShouldClose(window);
    }

    // Recorder
private:
    FILE *ffmpeg;
    int *buffer;
    void start_recorder()
    {
        // Default from source
        // const char *cmd = "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 1280x720 -i - "
        //                   "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";

        char cmd[200] = "";

        sprintf(cmd, "ffmpeg "
                     "-r 30 -f rawvideo -pixel_format rgba -video_size %dx%d -i pipe:0 "              // input file configs
                     "-preset slow -threads 8 -video_size %dx%d -vf vflip -y -crf 18 ./vid/output.mp4", // output file configs
                window_width(), window_height(), window_width(), window_height());
                
                // Add "-c:v libx264 -profile:v main -vf format=yuv420p" for mobile compatibility?

        // Source: http://blog.mmacklin.com/2013/06/11/real-time-video-capture-with-ffmpeg/

        // open pipe to ffmpeg's stdin in binary write mode
        if ((ffmpeg = popen(cmd, "w")) == NULL)
        {
            perror("popen error");
            // opengl.terminate(0);
        }

        if ((buffer = new int[window_width() * window_height()]) == NULL)
        {
            printf("allocation error");
            //opengl.terminate(0);
        }
    }

    void record()
    {
        glReadPixels(0, 0, window_width(), window_height(), GL_RGBA, GL_UNSIGNED_BYTE, buffer);

        fwrite(buffer, sizeof(int) * window_width() * window_height(), 1, ffmpeg);
    }
};
