
// #define OPENGL_ES

// Avoid define conflicts
#ifdef OPENGL
#undef OPENGL_ES
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include "camera.hpp"
#include "light.hpp"
#include "opengl.hpp"
#include "thing.hpp"
#include "tiktok.hpp"

Lighting lighting;

Tiktok &tiktok = Tiktok::get_instance();
Camera &camera = Camera::get_instance();
OpenGL &opengl = OpenGL::get_instance();

Thing thing;
Light light;
// Light sun(LIGHT_DIRECTIONAL);

Shader shader2("study41.vert", "study41.frag");
Shader shader3("study41.vert", "std.frag");

void loop()
{
    // Update state
    opengl.update();
    camera.update();
    tiktok.update();

    // camera.set_position(glm::vec3(0.0f, 0.0f, 80 * (cos(tiktok.get()) +
    // 0.75)));

    // Update lights
    float dir = 0.0;
    light.set_position(glm::vec3(0.0, 10.0, dir));
    light.set_color(glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
    light.set_brightness(Light::Effect::FLARE);
    // light.set_attenuation(0.6);
    light.draw();
    lighting.update();

    // Draw things
    thing.set_shader(shader2);
    thing.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    thing.update_shader(&lighting);
    thing.draw(Drawer::Type::FILL, 2);
}

int main()
{
    // SHADERS
    quad::print::info("Building and compiling shaders");

    // GEOMETRIES
    quad::print::info("Creating geometries");

    // 1. Initialize used geometries
    // DotCube dotcube(25, 25, 25, 3);
    Plane plane(100, 100, 1);
    Sphere sphere(10.0f);

    // 2. Add geometries to Thing object
    thing.add_geometry(plane);
    thing.add_geometry(sphere);

    // (2.5 Generate buffers)
    thing.generate_buffers();

    // 3. Set thing's properties
    thing.set_shader(shader2);
    thing.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Lights
    // 1. Add a light source
    lighting.add_source(&light);
    // 2. Set light type
    light.set_type(Light::Type::POSITIONAL);
    // (3. Add geometry to light)
    light.add_geometry(Sphere(0.2f));

    // Camera
    camera.set_type(camera.Type::STATIC);
    // camera.set_type(CAMERA_ROTATE_AROUND);
    // camera.set_type(CAMERA_SPLINE);
    camera.set_position(glm::vec3(0.0f, 2.0f, -15.0f));

    // Render loop
    quad::print::info("Starting rendering");
#ifdef OPENGL_ES
    emscripten_set_main_loop(loop, 0, opengl.running());
#else
    while (!opengl.should_close())
    {
        loop();
    }
#endif

    // Clean up
    opengl.terminate();

    return 0;
}
