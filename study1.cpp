
// #define VIDEO_OUT
// #define OPENGL_ES

// Avoid define conflicts
#ifdef OPENGL
#undef OPENGL_ES
#endif

#include <common.h>
#include <video.h>
#include <camera.h>
#include <geometry.h>
#include <opengl.h>
#include <shader.h>

/*** TODO ***
 * - Save vertex data into texture data
 * - Add geometry specific for "sun" type lighting
 * - Take neighboring triangles into consideration at normals calculator
 * - Add shadow support
 * - Recover old animations (everything in one big file)
 * - Create tutorial for storing vertex data in texture? Geometry shader for WebGL/OpenGL ES
 */

/*** IDEAS ***
 * - add light source to light the landspace (like a rising sun)
 * - clouds are small planes with wave shader applied to them floating around
 * - play with FOV
 * - explore "two lights underneath plane.mp4"
 * - quadrics / superquadrics
 */

/*** DOCUMENTATION ***
 * 
 * Transformations
 * Adding a translation to the transformation matrix
 *  trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
 * Adding a rotation
 *  trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
 * Adding a scaling
 *  trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));
 */

void tetra_terminate(int signal);
void tetra_update();

#ifdef VIDEO_OUT
Video video(opengl.window_width(), opengl.window_height());
#endif

// Cube geometry(10.0f);
Plane geometry(2, 50, 200);
Sphere light1(0.2f);
Sphere light2(0.2f);

void loop()
{
    // Update state
    tetra_update();

    // Drawing
    // shader_rainbow.set_vec4("color", glm::vec4(-sin(tetra_time.get() - 2.0) / 2.0 + 0.5, sin(tetra_time.get()) / 2.0 + 0.5, sin(tetra_time.get() + 2.0) / 2.0 + 0.5, 1.0));
    light1.set_color(glm::vec4(1.0, 0.0, 0.0, 1.0)); //glm::vec4(-sin(tetra_time.get() - 2.0) / 2.0 + 0.5, -sin(tetra_time.get()) / 2.0 + 0.5, -sin(tetra_time.get() + 2.0) / 2.0 + 0.5, 1.0));
    light1.set_model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, -10.0f)));
    light1.draw_fill();

    light2.set_color(glm::vec4(0.0, 1.0, 0.0, 1.0)); //(glm::vec4(sin(tetra_time.get() - 2.0) / 2.0 + 0.5, sin(tetra_time.get()) / 2.0 + 0.5, sin(tetra_time.get() + 2.0) / 2.0 + 0.5, 1.0));
    light2.set_model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 10.0f)));
    light2.draw_fill();

    geometry.draw_fill();
}

int main()
{
    // Adding SIGINT function
    signal(SIGINT, tetra_terminate);

    // Build and compile shaders
    printf("--Building and compiling shaders--\n");
    Shader shader1;
    Shader shader2("./shaders/study41.vert", "./shaders/study41.frag");

    // Lights
    // Sphere light1(0.1f);
    light1.set_light_source();
    light1.set_shader(shader1);

    // Sphere light2(0.1f);
    light2.set_light_source();
    light2.set_shader(shader1);

    // TODO
    // Lighting Sun;
    // Sun.add_source(0);
    // Sun.set_color(1.0,1.0,1.0,1.0);

    // Geometry
    printf("--Creating geometries--\n");
    // Plane geometry(1, 5, 5);
    geometry.set_shader(shader2);
    geometry.set_model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f)));
    geometry.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

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
    tetra_terminate(0);

    return 0;
}

void tetra_terminate(int signal)
{
    std::cout << "--Terminating--" << std::endl;
    glfwTerminate();
#ifdef VIDEO_OUT
    video.terminate();
#endif
    exit(0);
}

void tetra_update()
{
    opengl.update();
    camera.update(opengl.window);
    tetra_time.update();
#ifdef VIDEO_OUT
    video.record();
#endif

    // Clearing screen
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}