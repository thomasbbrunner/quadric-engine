
// #define OPENGL_ES

// Avoid define conflicts
#ifdef OPENGL
#undef OPENGL_ES
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include "camera.hpp"
#include "common.hpp"
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

Shader shader1;
Shader shader2("study41.vert", "study41.frag");
Shader shader3("study41.vert");

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
    light.set_color(glm::vec4(0.0f, 0.2f, 1.0f, 1.0f));
    light.set_brightness(BRIGHTNESS_FLARE);
    // light.set_attenuation(0.6);
    lighting.update();

    // Draw things
    thing.set_shader(shader1);
    thing.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    thing.update_shader(&lighting);
    thing.draw(DW_DOTS, 2);
}

int main()
{
    // SHADERS
    printf("--Building and compiling shaders--\n");

    // GEOMETRIES
    printf("--Creating geometries--\n");

    // 1. Initialize used geometries
    // DotCube dotcube(25, 25, 25, 3);
    Plane plane(50, 50, 2);

    // 2. Add geometries to Thing object
    thing.add_geometry(plane);

    // (2.5 Generate buffers)
    thing.generate_buffers();

    // 3. Set thing's properties
    thing.set_shader(shader2);
    thing.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Lights
    // 1. Add a light source
    // lighting.add_source(&light);
    // 2. Set light type
    light.set_type(LIGHT_POSITIONAL);
    // (3. Add geometry to light)
    light.add_geometry(Sphere(0.2f));

    // Camera
    camera.set_type(CAMERA_STATIC);
    // camera.set_type(CAMERA_ROTATE_AROUND);
    // camera.set_type(CAMERA_SPLINE);
    camera.set_position(glm::vec3(0.0f, 2.0f, -15.0f));

    // Render loop
    printf("--Starting rendering--\n");
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
