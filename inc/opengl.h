#ifndef OPENGL_H
#define OPENGL_H

#include <common.h>

// Toggle MSAA
#define MSAA

#define MSAA_SAMPLES 2
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

class OpenGL
{
  public:
    GLFWwindow *window;

    OpenGL()
    {
        int window_height = WINDOW_HEIGHT;
        int window_width = WINDOW_WIDTH;

        printf("--Initialising--\n");

        // Adding SIGINT function
        signal(SIGINT, terminate);

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
#endif

        // Mouse
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glfwSetCursorPosCallback(window, mouse_callback);

        // Debuging messages
        // glEnable(GL_DEBUG_OUTPUT);
        // glDebugMessageCallback();

        // Printing OpenGL version
        printf("%s\n", glGetString(GL_VERSION));
    }

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

        camera.update(this->window);
        tiktok.update();
#ifdef VIDEO_OUT
        video.record();
#endif

        // Clearing screen
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    static void terminate(int signal = 0)
    {
        std::cout << "--Terminating--" << std::endl;
        glfwTerminate();
#ifdef VIDEO_OUT
        video.terminate();
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

    int running()
    {
        return !glfwWindowShouldClose(window);
    }
};

OpenGL opengl;

#endif