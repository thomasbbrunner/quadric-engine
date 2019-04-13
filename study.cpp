
// #define VIDEO_OUT
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

#ifdef VIDEO_OUT
Video video(opengl.window_width(), opengl.window_height());
#endif

OpenGL opengl;
Lighting lighting;

Tiktok &tiktok = Tiktok::get_instance();
Camera &camera = Camera::get_instance();

Thing thing;
Light light;
// Light sun(LIGHT_DIRECTIONAL);

void loop()
{
    // Update state
    opengl.update();
    camera.update(opengl);
    tiktok.update();

    // Update lights
    float dir = 50.0f + 50.0f * sin(tiktok.get());
    light.set_position(glm::vec3(-dir, -dir, -dir));
    light.set_color(glm::vec4(-sin(tiktok.get() - 2.0) / 2.0 + 0.5, -sin(tiktok.get()) / 2.0 + 0.5, -sin(tiktok.get() + 2.0) / 2.0 + 0.5, 1.0));
    lighting.update();

    // Drawing
    // shader_rainbow.set_vec4("color", glm::vec4(-sin(tetra_time.get() - 2.0) / 2.0 + 0.5, sin(tetra_time.get()) / 2.0 + 0.5, sin(tetra_time.get() + 2.0) / 2.0 + 0.5, 1.0));
    // light1.set_color(glm::vec4(1.0, 0.0, 0.0, 1.0));
    // light1.set_color(glm::vec4(-sin(tetra_time.get() - 2.0) / 2.0 + 0.5, -sin(tetra_time.get()) / 2.0 + 0.5, -sin(tetra_time.get() + 2.0) / 2.0 + 0.5, 1.0));
    // light1.set_model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, -10.0f)));
    // light1.draw_fill();

    // // light2.set_color(glm::vec4(0.0, 1.0, 0.0, 1.0));
    // light2.set_color(glm::vec4(sin(tetra_time.get() - 2.0) / 2.0 + 0.5, sin(tetra_time.get()) / 2.0 + 0.5, sin(tetra_time.get() + 2.0) / 2.0 + 0.5, 1.0));
    // light2.set_model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 10.0f)));
    // light2.draw_fill();

    // Draw things
    thing.update_shader(&lighting);
    thing.draw(DW_FILL);
}

int main()
{
    // SHADERS
    printf("--Building and compiling shaders--\n");
    Shader shader1;
    Shader shader2("/home/thomas/Nextcloud/Projects/Art/quadric-engine/shaders/study41.vert", "/home/thomas/Nextcloud/Projects/Art/quadric-engine/shaders/study41.frag");

    // GEOMETRIES
    printf("--Creating geometries--\n");

    // 1. Initialize used geometries
    float rand2 = Math::random(0.0, 1.0);
    float rand3 = Math::random(0.0, 1.0);
    float rand4 = Math::random(0.0, 1.0);
    for (int i = 0; i < 50; i++)
    {
        Cube cube(5.0f);

        float rand1 = Math::random(0.0, 1.0);
        rand2 += 2.0 + Math::random(0.0, 1.0);
        rand3 += 2.0 + Math::random(0.0, 1.0);
        rand4 += 2.0 + Math::random(0.0, 1.0);

        cube.apply_transformation(glm::translate(glm::mat4(1.0f), glm::vec3(-rand2, -rand3, -rand4)));
        cube.apply_transformation(glm::rotate(glm::mat4(1.0f), rand1, glm::vec3(1.0, 0.8, 0.2)));
        // cube.apply_transformation(glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 2.0, 1.0)));

        // 2. Add geometries to Thing object
        thing.add_geometry(cube);
    }
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
    light.add_geometry();

    // Camera
    camera.set_type(CAMERA_STATIC);
    // camera.set_type(CAMERA_ROTATE_AROUND);
    camera.set_position(glm::vec3(0.0f, 2.0f, 15.0f));

// Render loop
#ifdef OPENGL_ES
    emscripten_set_main_loop(loop, 0, opengl.running());
#else
    while (opengl.running())
    {
        loop();
    }
#endif

    // Clean up
    opengl.terminate();

    return 0;
}
