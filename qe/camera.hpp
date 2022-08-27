
#pragma once

#include "api.hpp"

#include <Eigen/Core>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unsupported/Eigen/Splines>

class Camera
{
public:
    Camera()
    {
    }

    enum class Type
    {
        MOUSE,
        STATIC,
        ROTATE,
        ROTATE_AROUND,
        MOVE_BACKWARDS,
        SPLINE,
    };

    void update(float time, GLFWwindow *window, double mouse_x_coo, double mouse_y_coo);

    void set_position(glm::vec3 pos)
    {
        position_ = pos;
    }

    void set_front(glm::vec3 front)
    {
        front_ = front;
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
    float yaw = 0.0f;
    float pitch = 0.0f;
    float fov = 45.0f;

    glm::mat4 aspect = glm::mat4(1.0);
    glm::mat4 proj = glm::mat4(1.0);

    double mouse_last_x = 0.0;
    double mouse_last_y = 0.0;

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
};
