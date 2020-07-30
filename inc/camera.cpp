
#include "camera.hpp"

#include <Eigen/Core>
#include <unsupported/Eigen/Splines>

void Camera::update(float time)
{

    float camera_speed = speed(time);

    if (type_ == Type::STATIC)
    {
        if (glfwGetKey(opengl.window, GLFW_KEY_W) == GLFW_PRESS)
            position_ += camera_speed * front_;
        if (glfwGetKey(opengl.window, GLFW_KEY_S) == GLFW_PRESS)
            position_ -= camera_speed * front_;
        if (glfwGetKey(opengl.window, GLFW_KEY_A) == GLFW_PRESS)
            position_ -= glm::normalize(glm::cross(front_, up_)) * camera_speed;
        if (glfwGetKey(opengl.window, GLFW_KEY_D) == GLFW_PRESS)
            position_ += glm::normalize(glm::cross(front_, up_)) * camera_speed;
        if (glfwGetKey(opengl.window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
            position_ += camera_speed * up_;
        if (glfwGetKey(opengl.window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
            position_ -= camera_speed * up_;
        if (glfwGetKey(opengl.window, GLFW_KEY_E) == GLFW_PRESS)
        {
            // front_ -= 0.2f * camera_speed * glm::normalize(glm::cross(up_, front_));
            // fov++;
            // printf("fov: %.5f\n", fov);
        }
        if (glfwGetKey(opengl.window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            // fov--;
            // printf("fov: %.5f\n", fov);
            // front_ += 0.2f * camera_speed * glm::normalize(glm::cross(up_, front_));
        }

        view_ = glm::lookAt(position_, position_ + front_, up_);
    }
    else if (type_ == Type::ROTATE)
    {
        view_ = glm::rotate(glm::lookAt(position_, position_ + front_, glm::vec3(0.0, 1.0, 0.0)), 0.5f * time, front_);
    }
    else if (type_ == Type::ROTATE_AROUND)
    {
        float vel = 0.5;
        float radius = 30.0;
        float height = 5.0;
        float camX = glm::sin(vel * time * radius);
        float camZ = glm::cos(vel * time * radius);
        view_ = glm::lookAt(glm::vec3(camX, height, camZ), -glm::vec3(camX, height, camZ) + position_, glm::vec3(0.0, 1.0, 0.0));
    }
    else if (type_ == Type::MOVE_BACKWARDS)
    {
        position_ -= camera_speed * front_;
        view_ = glm::lookAt(position_, position_ + front_, up_);
    }
    // else if (type_ == Type::SPLINE)
    // {}
    else
    {
        quad::fatal_error("Camera type not recognized.", "Camera::update()");
    }

    aspect = glm::scale(glm::mat4(1.0), glm::vec3((float)opengl.get_window_height() / (float)opengl.get_window_width(), 1.0f, 1.0f));
    proj = glm::perspective<float>(glm::radians(fov), 1.0f, 1.0f, 1000.0f);
    // proj = glm::ortho<float>(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 1000.0f);
}

/* this should not be here, this is not a job for the Camera class
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
    float spline_walker = time.get() / 15.0;

    if (spline_walker >= 0.95)
    {
        spline_walker = 0.95;
    }

    Eigen::VectorXd pos = spline(spline_walker);

    position_ = glm::vec3(pos[0], pos[1], pos[2]);

    return glm::lookAt(position_, front_, up_);
}
*/
