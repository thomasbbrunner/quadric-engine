#ifndef CAMERA_H
#define CAMERA_H

#include <common.h>
#include <opengl.h>

#define CAMERA_STATIC 0x1
#define CAMERA_ROTATE_AROUND 0x2
#define CAMERA_ROTATE 0x4

class Camera
{
  public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f))
    {
        Position = pos;
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        lasttime = -1.0f;
    }

    void update(GLFWwindow *window)
    {
        float cameraSpeed = speed();

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Position += cameraSpeed * Front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Position -= cameraSpeed * Front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Position -= glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Position += glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
            Position += cameraSpeed * Up;
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
            Position -= cameraSpeed * Up;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            Front -= cameraSpeed * glm::normalize(glm::cross(Up, Front));
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            Front += cameraSpeed * glm::normalize(glm::cross(Up, Front));

        aspect = glm::scale(glm::mat4(1.0), glm::vec3((float)opengl.window_height() / (float)opengl.window_width(), 1.0f, 1.0f));

        proj = glm::perspective<float>(glm::radians(45.0f), 1.0f, 1.0f, 1000.0f);
        //proj = glm::ortho<float>(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 1000.0f);
    }

    void set_position(glm::vec3 pos)
    {
        Position = pos;
    }

    void set_type(int type_in)
    {
        type = type_in;
    }

    glm::mat4 get_view()
    {
        if (type == CAMERA_STATIC)
            return view_static();
        if (type == CAMERA_ROTATE)
            return view_rotate();
        if (type == CAMERA_ROTATE_AROUND)
            return view_rotate_around();
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

  private:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;

    glm::mat4 aspect = glm::mat4(1.0);
    glm::mat4 proj = glm::mat4(1.0);

    float lasttime;

    int type = 0;

    glm::mat4 view_static()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 view_rotate_around(glm::vec3 point = glm::vec3(0.0, 0.0, 0.0), float vel = 0.5, float radius = 28.0, float height = 5.0)
    {
        float camX = sin(vel * tetra_time.get()) * radius;
        float camZ = cos(vel * tetra_time.get()) * radius;
        return glm::lookAt(glm::vec3(camX, height, camZ), -glm::vec3(camX, height, camZ) + point, glm::vec3(0.0, 1.0, 0.0));
    }

    glm::mat4 view_rotate()
    {
        return glm::rotate(glm::lookAt(Position, Position + Front, glm::vec3(0.0, 1.0, 0.0)), 0.5f * (float)tetra_time.get(), Front);
    }

    glm::mat4 view_backwards()
    {
        float cameraSpeed = speed();

        Position -= cameraSpeed * Front;

        return glm::lookAt(Position, Position + Front, Up);
    }

    float speed()
    {
        if (lasttime == -1.0f)
        {
            lasttime = tetra_time.get();
        }

        float out = 5.0f * (tetra_time.get() - lasttime);

        lasttime = tetra_time.get();

        return out;
    }
};

Camera camera;

#endif