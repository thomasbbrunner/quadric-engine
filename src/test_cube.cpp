
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include "camera.hpp"
#include "light.hpp"
#include "api.hpp"
#include "scene.hpp"
#include "time.hpp"

int main()
{
    // Window
    GLFWwindow *window = qe::api::init_window(1080, 1080, 4);

    // Shaders
    qe::print::info("Building and compiling shaders");
    Shader shader("std_normal.vert", "std.frag");

    // Camera
    Camera camera{};
    camera.set_type(camera.Type::MOUSE);
    // camera.set_type(Camera::Type::ROTATE_AROUND);
    // camera.set_type(CAMERA_SPLINE);
    // camera.set_position(glm::vec3(0.0f, 20.0f, 10.0f));

    // Time
    qe::Time time{qe::Time::Type::TICKS};

    // Scene
    Scene scene{camera};

    // Geometries
    qe::print::info("Creating geometries");

    // initialize geometry
    Cube cube(15.0f);

    // add geometries to scene object
    scene.add_geometry(cube);

    // (2.5 Generate buffers)
    scene.generate_buffers();

    // 3. Set scene's properties
    scene.set_shader(shader);
    scene.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Lights
    Light light1;
    Light light2;
    Lighting lighting;
    // Light sun(LIGHT_DIRECTIONAL);
    // 1. Add a light source
    lighting.add_source(&light1);
    lighting.add_source(&light2);
    // 2. Set light type
    light1.set_type(Light::Type::POSITIONAL);
    light2.set_type(Light::Type::POSITIONAL);
    // (3. Add geometry to light)
    light1.add_geometry(Sphere(0.2f));
    light2.add_geometry(Sphere(0.2f));

    // Render loop
    qe::print::info("Starting rendering");

    while (!qe::api::window_should_close(window))
    {
        // Update state
        qe::api::update_window(window);
        time.update();
        camera.update(time.get(), window, qe::api::get_mouse_x_coo(), qe::api::get_mouse_y_coo());

        // camera.set_position(glm::vec3(0.0f, 0.0f, 80 * (cos(time.get()) + 0.75)));

        // Update lights
        light1.set_position(glm::vec3(0.0, 10.0, 20.0f));
        light1.set_color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        light1.set_brightness(Light::Effect::FLARE);
        light1.draw();

        light2.set_position(glm::vec3(0.0, 10.0, -20.0f));
        light2.set_color(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        light2.set_brightness(Light::Effect::NORMAL);
        // light.set_attenuation(0.6);
        light2.draw();
        lighting.update();

        // Draw scene
        scene.set_shader(shader);
        scene.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        scene.update_shader(&lighting, time.get());
        scene.draw(Drawer::Type::FILL, 2);
    }

    // Clean up
    qe::api::terminate();

    return 0;
}
