
#include "api.hpp"

#include "error.hpp"
#include "print.hpp"

#ifdef OPENGL_ES // Including files for OpenGL ES

#include <GLES3/gl3.h>
#include <emscripten/emscripten.h>
#include <GLFW/glfw3.h>
/* main loop for OpenGL ES
#ifdef OPENGL_ES
    emscripten_set_main_loop(loop, 0, opengl.running());
#else
*/

#else // Including files for OpenGL

#endif

namespace qe
{
    namespace api
    {
        double mouse_x_coo = 0;
        double mouse_y_coo = 0;

        double get_mouse_x_coo()
        {
            return mouse_x_coo;
        }

        double get_mouse_y_coo()
        {
            return mouse_y_coo;
        }

        GLFWwindow *init_window(unsigned int window_width, unsigned int window_height, unsigned int msaa_samples)
        {
            quad::print::info("Initialising quadric-engine");

            // Initialising GLFW
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            if (msaa_samples)
            {
                glfwWindowHint(GLFW_SAMPLES, msaa_samples);
            }

            // Creating window
            GLFWwindow *window = glfwCreateWindow(window_width, window_height, "OpenGL", NULL, NULL); // Fullscreen: glfwGetPrimaryMonitor()
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
            glfwSetFramebufferSizeCallback(window, resize_window);

            // Setting window size and location
            resize_window(window, window_width, window_height);

            // Enabling depth test
            glEnable(GL_DEPTH_TEST);

            // Enable MSAA
            if (msaa_samples)
            {
                glEnable(GL_MULTISAMPLE);
                glEnable(GL_LINE_SMOOTH);
            }

            // Debuging messages
            // glEnable(GL_DEBUG_OUTPUT);
            // glDebugMessageCallback();

            // Mouse support
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, mouse_callback);
            // glfwRawMouseMotionSupported() // Needs GLFW 3.3

            // Printing OpenGL information
            std::string info_msg{
                std::string("Device information:") +
                "\n  OpenGL version: " + reinterpret_cast<const char *>(glGetString(GL_VERSION)) +
                "\n  GLSL version:   " + reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)) +
                "\n  Vendor:         " + reinterpret_cast<const char *>(glGetString(GL_VENDOR)) +
                "\n  Renderer:       " + reinterpret_cast<const char *>(glGetString(GL_RENDERER))};
            quad::print::info(info_msg);

            return window;
        }

        void resize_window(GLFWwindow *window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }

        int get_window_height(GLFWwindow *window)
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            return height;
        }

        int get_window_width(GLFWwindow *window)
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            return width;
        }

        void update_window(GLFWwindow *window)
        {
            // Check if esc was pressed to quit
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            check_for_errors();

            // Check for events and swap buffer
            glfwSwapBuffers(window);
            glfwPollEvents();

            // Clearing screen
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void terminate()
        {
            quad::print::info("Terminating API");
            glfwTerminate();
        }

        bool window_should_close(GLFWwindow *window)
        {
            return glfwWindowShouldClose(window);
        }

        void check_for_errors()
        {
            if (GLenum error = glGetError() != GL_NO_ERROR)
            {
                quad::print::error("OpenGL error " + std::to_string(error));
            }
        }

        void mouse_callback(GLFWwindow *window, double x_coo, double y_coo)
        {
            mouse_x_coo = x_coo;
            mouse_y_coo = y_coo;
        }

    } // namespace api
} // namespace qe
