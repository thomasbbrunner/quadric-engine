
#pragma once

#include "opengl.hpp"
#include "api.hpp"

#include <Eigen/Core>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unsupported/Eigen/Splines>

class Camera
{
    // Singleton initialization
public:
    static Camera &get_instance()
    {
        static Camera instance;
        return instance;
    }

    Camera(Camera const &) = delete;
    void operator=(Camera const &) = delete;

private:
    Camera()
    {
        // Mouse
        glfwSetInputMode(opengl.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(opengl.window, mouse_callback);
        // glfwRawMouseMotionSupported() // Needs GLFW 3.3

        mouse_last_x = (float)opengl.get_window_width();
        mouse_last_y = (float)opengl.get_window_height();
    }
    // End of Singleton initialization

public:
    enum class Type
    {
        STATIC,
        ROTATE,
        ROTATE_AROUND,
        MOVE_BACKWARDS,
        SPLINE,
    };

    void update(float time);

    void set_position(glm::vec3 pos)
    {
        position_ = pos;
    }

    void set_type(enum Type type)
    {
        type_ = type;

        if (type_ == Type::SPLINE)
            initialize_spline();
    }

    void set_fov(float fov)
    {
        if (fov <= 0.0f)
            this->fov = 0.0f;
        else if (fov >= 180.0f)
            this->fov = 180.0f;
        else
            this->fov = fov;
    }

    glm::mat4 get_view()
    {
        return view_;
    }

    glm::mat4 get_aspect()
    {
        return aspect;
    }

    glm::mat4 get_proj()
    {
        return proj;
    }

    enum Type type_ = Type::STATIC;
    glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 front_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view_;

private:
    OpenGL &opengl = OpenGL::get_instance();

    float yaw = 0.0f;
    float pitch = 0.0f;
    bool firstMouse = true;
    float fov = 45.0f;

    glm::mat4 aspect = glm::mat4(1.0);
    glm::mat4 proj = glm::mat4(1.0);

    float mouse_last_x;
    float mouse_last_y;

    float lasttime = -1.0f;

    Eigen::Spline<double, 3> spline;

    void initialize_spline();

    glm::mat4 view_spline();

    float speed(float time)
    {
        if (lasttime == -1.0f)
        {
            lasttime = time;
        }

        float out = 50.0f * (time - lasttime);

        lasttime = time;

        return out;
    }

    static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
    {
        Camera &camera = Camera::get_instance();

        if (camera.type_ == Type::STATIC)
        {
            if (camera.firstMouse)
            {
                camera.mouse_last_x = xpos;
                camera.mouse_last_y = ypos;
                camera.firstMouse = false;
            }

            float xoffset = xpos - camera.mouse_last_x;
            float yoffset = camera.mouse_last_y - ypos;
            camera.mouse_last_x = xpos;
            camera.mouse_last_y = ypos;

            float sensitivity = 0.1;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            camera.yaw += xoffset;
            camera.pitch += yoffset;

            if (camera.pitch > 89.0f)
                camera.pitch = 89.0f;
            if (camera.pitch < -89.0f)
                camera.pitch = -89.0f;

            glm::vec3 front;
            front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
            front.y = sin(glm::radians(camera.pitch));
            front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
            camera.front_ = glm::normalize(front);
        }
    }
};
