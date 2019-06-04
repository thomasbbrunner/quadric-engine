
// #define OPENGL_ES

// Avoid define conflicts
#ifdef OPENGL
#undef OPENGL_ES
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "opengl.h"
#include "light.h"
#include "thing.h"
#include "camera.h"
#include "time.h"
#include "common.h"

Lighting lighting;

Tiktok &tiktok = Tiktok::get_instance();
Camera &camera = Camera::get_instance();
OpenGL &opengl = OpenGL::get_instance();

Thing thing;
Light light;
// Light sun(LIGHT_DIRECTIONAL);

Shader shader1;
Shader shader2("/home/thomas/Nextcloud/Projects/Art/quadric-engine/shaders/study41.vert", "/home/thomas/Nextcloud/Projects/Art/quadric-engine/shaders/study41.frag");
Shader shader3("/home/thomas/Nextcloud/Projects/Art/quadric-engine/shaders/study41.vert");

void loop()
{
    // Update state
    opengl.update();
    camera.update();
    tiktok.update();

    // Update lights
    float dir = 200.0f * (-1.6 + sin(0.8 * tiktok.get() - 0.1));
    light.set_position(glm::vec3(0.0, 0.0, dir));
    camera.Front = light.get_position();
    light.set_color(glm::vec4(-sin(tiktok.get() - 2.0) / 2.0 + 0.5, -sin(tiktok.get()) / 2.0 + 0.5, -sin(tiktok.get() + 2.0) / 2.0 + 0.5, 1.0));
    light.set_brightness(BRIGHTNESS_FLARE);
    light.set_attenuation(0.6);
    lighting.update();

    // Draw things
    thing.set_shader(shader2);
    thing.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    thing.update_shader(&lighting);
    thing.draw(DW_FILL);

    thing.set_shader(shader3);
    thing.set_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    thing.update_shader();
    thing.draw(DW_CONTOUR, 1.0);
}

int main()
{
    // SHADERS
    printf("--Building and compiling shaders--\n");

    // GEOMETRIES
    printf("--Creating geometries--\n");

    // 1. Initialize used geometries

    // Short tunnel (vertically)
    for (int k = -1; k <= 1; k += 2)
    {
        for (int i = -25; i < 25; i++)
        {
            for (int j = -20; j < 170; j++)
            {
                Cube cube(5.0f);

                float rand1 = Math::random(-1.0, 1.0);
                float rand2 = Math::random(-1.0, 1.0);

                float inc = 5.0;

                cube.apply_transformation(glm::rotate(glm::mat4(1.0f), rand1, glm::vec3(0.0f, 0.0f, 1.0f)));
                cube.apply_transformation(glm::translate(glm::mat4(1.0f), glm::vec3(k * 40.0 + rand2 - k * inc * (float)abs(i), -5.0f * i, -5.0f * j)));
                // cube.apply_transformation(glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 2.0, 1.0)));

                // 2. Add geometries to Thing object
                thing.add_geometry(cube);
            }
        }
    }

    // Elongated tunnel (vertically)
    // for (int k = -1; k <= 1; k += 2)
    // {
    //     for (int i = -25; i < 25; i++)
    //     {
    //         for (int j = -10; j < 50; j++)
    //         {
    //             Cube cube(5.0f);

    //             float rand1 = Math::random(-1.0, 1.0);
    //             float rand2 = Math::random(-1.0, 1.0);

    //             float inc = 5.0;

    //             cube.apply_transformation(glm::rotate(glm::mat4(1.0f), rand1, glm::vec3(0.0f, 0.0f, 1.0f)));
    //             cube.apply_transformation(glm::translate(glm::mat4(1.0f), glm::vec3(k * 40.0 + rand2 - k * inc * (float)abs(i), -5.0f * i, -250.0f - 5.0f * j * inc)));
    //             // cube.apply_transformation(glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 2.0, 1.0)));

    //             // 2. Add geometries to Thing object
    //             thing.add_geometry(cube);
    //         }
    //     }
    // }

    // Cubes
    // Cube cube(15.0f);
    // cube.apply_transformation(glm::translate(glm::mat4(1.0f), (glm::vec3(10.0, 0.0, 0.0))));
    // thing.add_geometry(cube);
    // cube.apply_transformation(glm::translate(glm::mat4(1.0f), (glm::vec3(-20.0, 0.0, 0.0))));
    // thing.add_geometry(cube);

    // (2.5 Generate buffers)
    thing.generate_buffers();

    // 3. Set thing's properties
    thing.set_shader(shader2);
    thing.set_model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
    thing.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Lights
    // 1. Add a light source
    lighting.add_source(&light);
    // 2. Set light type
    light.set_type(LIGHT_POSITIONAL);
    // (3. Add geometry to light)
    light.add_geometry(Sphere(0.2f));

    // Camera
    // camera.set_type(CAMERA_STATIC);
    // camera.set_type(CAMERA_ROTATE_AROUND);
    camera.set_type(CAMERA_SPLINE);
    // camera.set_position(glm::vec3(0.0f, 2.0f, -15.0f));

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
