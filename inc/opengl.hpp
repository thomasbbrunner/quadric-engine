
#pragma once

#include "api.hpp"
#include "error.hpp"
#include "print.hpp"

#include <signal.h>

// Toggle MSAA TODO: add as argument to constructor
constexpr bool MSAA = true;
constexpr int MSAA_SAMPLES = 4;

// Toggle video capture
constexpr bool VIDEO_OUT = false;

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
        int window_height = 1080;
        int window_width = 1080;

        quad::print::info("Initialising quadric-engine");

        // Adding SIGINT handler
        // signal(SIGINT, signal_handler);

        // Initialising GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        if (MSAA)
        {
            glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES);
        }

        // Creating window
        window = glfwCreateWindow(window_width, window_height, "OpenGL", NULL, NULL); // Fullscreen: glfwGetPrimaryMonitor()
        if (window == NULL)
        {
            throw quad::fatal_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window);

        // Initialising GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw quad::fatal_error("Failed to initialize GLAD");
        }

        // For window resizing
        glfwSetFramebufferSizeCallback(window, this->framebuffer_resize_callback);

        // Setting window size and location
        set_window_dimensions(window_width, window_height);

        // Enabling depth test
        glEnable(GL_DEPTH_TEST);

        // Enable MSAA
        if (MSAA)
        {
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_LINE_SMOOTH);
        }

        // Debuging messages
        // glEnable(GL_DEBUG_OUTPUT);
        // glDebugMessageCallback();

        // Printing OpenGL information
        std::string info_msg{
            std::string("Device information:") +
            "\n  OpenGL version: " + reinterpret_cast<const char *>(glGetString(GL_VERSION)) +
            "\n  GLSL version:   " + reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)) +
            "\n  Vendor:         " + reinterpret_cast<const char *>(glGetString(GL_VENDOR)) +
            "\n  Renderer:       " + reinterpret_cast<const char *>(glGetString(GL_RENDERER))};
        quad::print::info(info_msg);

        // Recorder
        if constexpr (VIDEO_OUT)
        {
            start_recorder();
        }
    }

public:
    GLFWwindow *window;

    static void framebuffer_resize_callback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void set_window_dimensions(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    int get_window_height()
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return height;
    }

    int get_window_width()
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

        if constexpr (VIDEO_OUT)
        {
            record();
        }

        // Clearing screen
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void terminate()
    {
        quad::print::info("Terminating");
        glfwTerminate();
        if constexpr (VIDEO_OUT)
        {
            // pclose(ffmpeg);
        }

        exit(0);
    }

    void mouse_input_callback(GLFWwindow *window, double xpos, double ypos)
    {
    }

    void check_error()
    {
        if (GLenum error = glGetError() != GL_NO_ERROR)
        {
            quad::print::error("OpenGL error " + std::to_string(error));
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
                     "-r 30 -f rawvideo -pixel_format rgba -video_size %dx%d -i pipe:0 "                // input file configs
                     "-preset slow -threads 8 -video_size %dx%d -vf vflip -y -crf 18 ./vid/output.mp4", // output file configs
                get_window_width(), get_window_height(), get_window_width(), get_window_height());

        // Add "-c:v libx264 -profile:v main -vf format=yuv420p" for mobile compatibility?

        // Source: http://blog.mmacklin.com/2013/06/11/real-time-video-capture-with-ffmpeg/

        // open pipe to ffmpeg's stdin in binary write mode
        if ((ffmpeg = popen(cmd, "w")) == NULL)
        {
            perror("popen error");
            // opengl.terminate(0);
        }

        if ((buffer = new int[get_window_width() * get_window_height()]) == NULL)
        {
            quad::fatal_error("allocation error");
        }
    }

    void record()
    {
        glReadPixels(0, 0, get_window_width(), get_window_height(), GL_RGBA, GL_UNSIGNED_BYTE, buffer);

        fwrite(buffer, sizeof(int) * get_window_width() * get_window_height(), 1, ffmpeg);
    }
};
