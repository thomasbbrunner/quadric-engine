
#pragma once

#include <glad/glad.h>
// GLFW has to be included after glad
// Including it only here solves compiling conflicts
#include <GLFW/glfw3.h>

namespace qe
{
    namespace api
    {
        GLFWwindow *init_window(unsigned int window_width, unsigned int window_height, unsigned int msaa_samples);

        void resize_window(GLFWwindow *window, int width, int height);

        int get_window_height(GLFWwindow *window);

        int get_window_width(GLFWwindow *window);

        void update_window(GLFWwindow *window);

        void terminate();

        bool window_should_close(GLFWwindow *window);

        void check_for_errors();

        void mouse_callback(GLFWwindow *window, double x_coo, double y_coo);

        double get_mouse_x_coo();

        double get_mouse_y_coo();

    } // namespace api
} // namespace qe
