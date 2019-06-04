#ifndef CAMERA_H
#define CAMERA_H

#include "opengl.h"
#include "time.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Eigen/Core>
#include <Eigen/Splines>

// Camera types
#define CAMERA_STATIC 0
#define CAMERA_ROTATE_AROUND 1
#define CAMERA_ROTATE 2
#define CAMERA_MOVE_BACKWARDS 3
#define CAMERA_SPLINE 4

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
        // glfwSetInputMode(opengl.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glfwSetCursorPosCallback(opengl.window, mouse_callback);
        // glfwRawMouseMotionSupported() // Needs GLFW 3.3

        mouse_last_x = (float)opengl.window_width();
        mouse_last_y = (float)opengl.window_height();
    }
    // End of Singleton initialization

public:
    void update()
    {
        if (type == CAMERA_STATIC)
        {
            float cameraSpeed = speed();

            if (glfwGetKey(opengl.window, GLFW_KEY_W) == GLFW_PRESS)
                Position += cameraSpeed * Front;
            if (glfwGetKey(opengl.window, GLFW_KEY_S) == GLFW_PRESS)
                Position -= cameraSpeed * Front;
            if (glfwGetKey(opengl.window, GLFW_KEY_A) == GLFW_PRESS)
                Position -= glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
            if (glfwGetKey(opengl.window, GLFW_KEY_D) == GLFW_PRESS)
                Position += glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
            if (glfwGetKey(opengl.window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
                Position += cameraSpeed * Up;
            if (glfwGetKey(opengl.window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
                Position -= cameraSpeed * Up;
            if (glfwGetKey(opengl.window, GLFW_KEY_E) == GLFW_PRESS)
            {
                // Front -= 0.2f * cameraSpeed * glm::normalize(glm::cross(Up, Front));
                // fov++;
                // printf("fov: %.5f\n", fov);
            }
            if (glfwGetKey(opengl.window, GLFW_KEY_Q) == GLFW_PRESS)
            {
                // fov--;
                // printf("fov: %.5f\n", fov);
                // Front += 0.2f * cameraSpeed * glm::normalize(glm::cross(Up, Front));
            }
        }

        aspect = glm::scale(glm::mat4(1.0), glm::vec3((float)opengl.window_height() / (float)opengl.window_width(), 1.0f, 1.0f));
        proj = glm::perspective<float>(glm::radians(fov), 1.0f, 1.0f, 1000.0f);
        // proj = glm::ortho<float>(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 1000.0f);
    }

    void set_position(glm::vec3 pos)
    {
        Position = pos;
    }

    void set_type(int type)
    {
        this->type = type;

        if (type == CAMERA_SPLINE)
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
        if (type == CAMERA_STATIC)
            return view_static();
        else if (type == CAMERA_ROTATE)
            return view_rotate();
        else if (type == CAMERA_ROTATE_AROUND)
            return view_rotate_around();
        else if (type == CAMERA_MOVE_BACKWARDS)
            return view_backwards();
        else if (type == CAMERA_SPLINE)
            return view_spline();
        else
            return view_static();
    }

    glm::mat4 get_aspect()
    {
        return aspect;
    }

    glm::mat4 get_proj()
    {
        return proj;
    }

    int type = 0;
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

private:
    Tiktok &tiktok = Tiktok::get_instance();
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

    glm::mat4 view_static()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 view_rotate_around(float vel = 0.5, float radius = 30.0, float height = 5.0)
    {
        float camX = sin(vel * tiktok.get()) * radius;
        float camZ = cos(vel * tiktok.get()) * radius;
        return glm::lookAt(glm::vec3(camX, height, camZ), -glm::vec3(camX, height, camZ) + Position, glm::vec3(0.0, 1.0, 0.0));
    }

    glm::mat4 view_rotate()
    {
        return glm::rotate(glm::lookAt(Position, Position + Front, glm::vec3(0.0, 1.0, 0.0)), 0.5f * (float)tiktok.get(), Front);
    }

    glm::mat4 view_backwards()
    {
        Position -= speed() * Front;

        return glm::lookAt(Position, Position + Front, Up);
    }

    void initialize_spline()
    {
        Eigen::VectorXd x_pts(12);
        Eigen::VectorXd y_pts(12);
        Eigen::VectorXd z_pts(12);

        x_pts << -10.0, -10.0, -10.0, -10.0, 25.0, 25.0, 25.0, 10.0, 150.0, 200.0, 225.0, 250.0;
        y_pts << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 40.0, 40.0, 40.0, 40.0;
        z_pts << 0.0, -60.0, -120.0, -180.0, -240.0, -300.0, -360.0, -420.0, -480.0, -420.0, -400.0, -400.0;

        Eigen::MatrixXd pts = Eigen::MatrixXd(3, x_pts.size());

        pts.row(0) = x_pts;
        pts.row(1) = y_pts;
        pts.row(2) = z_pts;

        spline = Eigen::SplineFitting<Eigen::Spline<double, 3>>::Interpolate(pts, 3);
    }

    glm::mat4 view_spline()
    {
        float spline_walker = tiktok.get() / 15.0;

        if (spline_walker >= 0.95)
        {
            spline_walker = 0.95;
        }

        Eigen::VectorXd pos = spline(spline_walker);

        Position = glm::vec3(pos[0], pos[1], pos[2]);

        return glm::lookAt(Position, Front, Up);
    }

    float speed()
    {
        if (lasttime == -1.0f)
        {
            lasttime = tiktok.get();
        }

        float out = 50.0f * (tiktok.get() - lasttime);

        lasttime = tiktok.get();

        return out;
    }

    static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
    {
        Camera &camera = Camera::get_instance();

        if (camera.type == CAMERA_STATIC)
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
            camera.Front = glm::normalize(front);
        }
    }
};

#endif