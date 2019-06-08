#include "camera.h"

#include <Eigen/Core>
#include <unsupported/Eigen/Splines>

void Camera::update()
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

void Camera::initialize_spline()
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

glm::mat4 Camera::view_spline()
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
