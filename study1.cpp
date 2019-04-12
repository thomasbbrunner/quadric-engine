
// #define VIDEO_OUT
// #define OPENGL_ES

// Avoid define conflicts
#ifdef OPENGL
#undef OPENGL_ES
#endif

#include <common.h>

// Lighting not working:
// -disabled in geometry.h
// Camera uses fix window sizes (otherwise opengl includes camera which includes opengl...)

// Change tabs to spaces

#ifdef VIDEO_OUT
Video video(opengl.window_width(), opengl.window_height());
#endif

Thing thing;

void loop()
{
    // Update state
    opengl.update();

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

    thing.draw(DW_CONTOUR);
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
    Cube cube(10.0f);
    // Icosahedron ico(10.0f);
    // Plane geometry(50, 50, 1);
    // Circle circle(2.0);
    // Cube light1(0.2f);
    // Cube light2(0.2f);
    // Plane geometry(1, 5, 5);

    // 2. Add geometries to Thing object
    thing.add_geometry(circle);
    thing.add_geometry(cube);

    // (2.5 Generate buffers)
    thing.generate_buffers();

    // 3. Set thing's properties
    thing.set_shader(shader1);
    thing.set_model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
    thing.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // // Lights
    // light1.set_light_source();
    // light1.set_shader(shader1);

    // light2.set_light_source();
    // light2.set_shader(shader1);

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
