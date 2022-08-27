
#include "api.hpp"
#include "camera.hpp"
#include "geometry3D.hpp"
#include "math.hpp"
#include "light.hpp"
#include "scene.hpp"
#include "time.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include <string>
#include <vector>

int main()
{
    // Window
    GLFWwindow *window = qe::api::init_window(1900, 1080, 2);

    // Shaders
    qe::print::info("Building and compiling shaders");
    Shader shader("std.vert", "std.frag");
    Shader shader_normal("std_normal.vert", "std.frag");
    Shader shader_no_lighting("std.vert", "std_no_lighting.frag");
    Shader shader_wave("effects_wave.vert", "std.frag");
    Shader shader_perlin("effects_perlin.vert", "std.frag");
    Shader shader_tube("effects_tube.vert", "std.frag");

    // Camera
    Camera camera{};
    camera.set_type(camera.Type::STATIC);
    camera.set_position(glm::vec3(0.0f, 0.0f, 0.0f));

    // Time
    qe::Time time{qe::Time::Type::WALL};

    // Lights
    Light ambient;
    ambient.set_type(Light::Type::DIRECTIONAL);
    // for debugging
    // ambient.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ambient.set_color(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    ambient.set_brightness(Light::Effect::FLICKER_SPORADIC, 5);
    Light light1;
    light1.set_type(Light::Type::POSITIONAL);
    light1.set_attenuation(0.8);
    light1.set_brightness(Light::Effect::NORMAL, 0.8);
    Light light2;
    light2.set_type(Light::Type::POSITIONAL);
    light2.set_attenuation(0.8);
    light2.set_brightness(Light::Effect::NORMAL, 0.8);
    Light light3;
    light3.set_type(Light::Type::POSITIONAL);
    light3.set_attenuation(0.8);
    light3.set_brightness(Light::Effect::NORMAL, 0.8);

    Lighting lighting;
    lighting.add_source(&ambient);
    lighting.add_source(&light1);
    lighting.add_source(&light2);
    lighting.add_source(&light3);

    // Geometries
    qe::print::info("Creating geometries");
    EclipsiaII eclipsia;
    eclipsia.apply_transformation(glm::scale(glm::mat4(1.), glm::vec3(30, 30, 30)));
    eclipsia.apply_transformation(glm::rotate(glm::mat4(1.), glm::half_pi<float>(), glm::vec3(1, 0, 0)));
    eclipsia.apply_transformation(glm::translate(glm::mat4(1.), glm::vec3(0, 0, -250)));

    // Scenes
    qe::print::info("Creating scenes");
    std::vector<Scene> scenes;

    // Eclipsia Scene
    Scene scene_eclipsia{camera};
    scene_eclipsia.add_geometry(eclipsia);
    scene_eclipsia.generate_buffers();
    scene_eclipsia.set_shader(shader_normal);
    scene_eclipsia.set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    Scene scene_eclipsia_contour{camera};
    scene_eclipsia_contour.add_geometry(eclipsia);
    scene_eclipsia_contour.generate_buffers();
    scene_eclipsia_contour.set_shader(shader_no_lighting);
    scene_eclipsia_contour.set_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    // Scene 0: sea of cubes
    {
        scenes.push_back(Scene{camera});
        for (int i = -5; i < 5; i++)
        {
            for (int j = 0; j < 80; j++)
            {
                Cube cube(15.0f);
                cube.apply_transformation(glm::translate(glm::mat4(1.), glm::vec3(i * 20, -20, -j * 20)));
                scenes.back().add_geometry(cube);
            }
        }
        scenes.back().generate_buffers();
        scenes.back().set_shader(shader_perlin);
        scenes.back().set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    // Scene 1: tunnel of cubes
    {
        scenes.push_back(Scene{camera});
        for (int k = -1; k <= 1; k += 2)
        {
            for (int i = -10; i < 10; i++)
            {
                for (int j = 0; j < 80; j++)
                {
                    Cube cube(15.0f);

                    float rand1 = qe::math::random(-1.0, 1.0);
                    float rand2 = qe::math::random(-1.0, 1.0);

                    float inc = 15.0;

                    cube.apply_transformation(glm::rotate(glm::mat4(1.0f), rand1, glm::vec3(0.0f, 0.0f, 1.0f)));
                    cube.apply_transformation(glm::translate(glm::mat4(1.0f), glm::vec3(k * 90.0 + rand2 - k * inc * (float)abs(i), 10. + -inc * i, -inc * j)));
                    // cube.apply_transformation(glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 2.0, 1.0)));
                    scenes.back().add_geometry(cube);
                }
            }
        }
        scenes.back().generate_buffers();
        scenes.back().set_shader(shader_perlin);
        scenes.back().set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    // Scene 2: plane
    {
        scenes.push_back(Scene{camera});
        Plane plane(200, 300, 3); // TODO increase this value for production
        plane.apply_transformation(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -8.0f, -150.0f)));
        scenes.back().add_geometry(plane);
        scenes.back().generate_buffers();
        scenes.back().set_shader(shader_perlin);
        scenes.back().set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    // Scenes 3 to 8: increasing sea of cubes
    {
        scenes.push_back(Scene{camera});
        scenes.push_back(Scene{camera});
        scenes.push_back(Scene{camera});
        scenes.push_back(Scene{camera});
        scenes.push_back(Scene{camera});
        scenes.push_back(Scene{camera});
        for (int i = -5; i <= 5; i++)
        {
            for (int j = 0; j < 80; j++)
            {
                Cube cube(15.0f);
                cube.apply_transformation(glm::translate(glm::mat4(1.), glm::vec3(i * 20, -20, -j * 20)));
                if (i == -5 or i == 5)
                {
                    scenes.end()[-6].add_geometry(cube);
                }
                else if (i == -4 or i == 4)
                {
                    scenes.end()[-6].add_geometry(cube);
                    scenes.end()[-5].add_geometry(cube);
                }
                else if (i == -3 or i == 3)
                {
                    scenes.end()[-6].add_geometry(cube);
                    scenes.end()[-5].add_geometry(cube);
                    scenes.end()[-4].add_geometry(cube);
                }
                else if (i == -2 or i == 2)
                {
                    scenes.end()[-6].add_geometry(cube);
                    scenes.end()[-5].add_geometry(cube);
                    scenes.end()[-4].add_geometry(cube);
                    scenes.end()[-3].add_geometry(cube);
                }
                else if (i == -1 or i == 1)
                {
                    scenes.end()[-6].add_geometry(cube);
                    scenes.end()[-5].add_geometry(cube);
                    scenes.end()[-4].add_geometry(cube);
                    scenes.end()[-3].add_geometry(cube);
                    scenes.end()[-2].add_geometry(cube);
                }
                else if (i == 0)
                {
                    scenes.end()[-6].add_geometry(cube);
                    scenes.end()[-5].add_geometry(cube);
                    scenes.end()[-4].add_geometry(cube);
                    scenes.end()[-3].add_geometry(cube);
                    scenes.end()[-2].add_geometry(cube);
                    scenes.end()[-1].add_geometry(cube);
                }
            }
        }
        for (int i = 1; i <= 6; i++)
        {
            scenes.end()[-i].generate_buffers();
            scenes.end()[-i].set_shader(shader_normal);
            scenes.end()[-i].set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }
    // Scene 9: tube
    {
        scenes.push_back(Scene{camera});
        Plane plane(400, 500, 1); // TODO increase this value for production
        plane.apply_transformation(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -250.0f))); // y has no effect
        scenes.back().add_geometry(plane);
        scenes.back().generate_buffers();
        scenes.back().set_shader(shader_tube);
        scenes.back().set_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // has any length
    std::vector<int> scene_sequence{
        0, 1, 2, 3, 9, };
    float scene_duration = 20.0;

    // Render loop
    qe::print::info("Starting rendering");
    int scene = 0;
    float start_time = time.get<float>();
    // specific for scene 3 to 8
    float start_time_3 = 0.;
    int scene_index_3 = 3;
    bool ascending_3 = true;
    bool descending_transition = false;
    bool ascending_transition = true;
    float start_time_transition = 0.;
    float transition_duration = 4.; // don't change?
    bool ultra_exposed = false;
    int camera_pos = 0;
    while (!qe::api::window_should_close(window))
    {
        // Update state
        qe::api::update_window(window);
        float cur_time = time.get<float>();
        time.update();
        if (camera_pos == 0) { 
            camera.set_position(glm::vec3(0, 0, 0)); 
            camera.set_front(glm::vec3(0, 0, -1));
        }
        if (camera_pos == 1) { 
            camera.set_position(glm::vec3(-50, 10, -100)); 
            camera.set_front(glm::vec3(0.4, 0, -1));
        }
        if (camera_pos == 2) { 
            camera.set_position(glm::vec3(50, -5, -130)); 
            camera.set_front(glm::vec3(-0.4, 0, -1));
        }
        // camera.update(window);
        camera.update(cur_time, window, qe::api::get_mouse_x_coo(), qe::api::get_mouse_y_coo());

        // Update lights
        light1.set_position(glm::vec3(10, 15, -210));
        light1.set_color(glm::vec4(10, 0, 0, 1));
        light2.set_position(glm::vec3(20.0 * (glm::cos(0.44*cur_time + 1.) + 1), 15.0*glm::sin(0.1*cur_time + 3), -200.0f * (glm::cos(0.3 * cur_time + 5.) + 1)));
        light2.set_color(glm::vec4(1, 0, 5, 1));
        light3.set_position(glm::vec3(0.0, 30.0 * (glm::sin(0.4 * cur_time) + 1), -200.0f * (glm::cos(0.2 * cur_time) + 1)));
        light3.set_color(glm::vec4(4, 0, 5, 1));
        if (ultra_exposed)
        {
            light1.set_color(glm::vec4(200, 0, 0, 1));
            light1.set_attenuation(0.2);
        }
        else { light1.set_attenuation(0.8); }
        if (ascending_transition or descending_transition)
        {
            float exponent = 0.;
            if (ascending_transition) { exponent = (cur_time - start_time_transition) - transition_duration;}
            else { exponent = -(cur_time - start_time_transition);}
            light1.set_color(light1.get_color() * glm::exp(exponent));
            light2.set_color(light2.get_color() * glm::exp(exponent));
            light3.set_color(light3.get_color() * glm::exp(exponent));
        }
        lighting.update();

        // Draw scenes
        scene_eclipsia.update_shader(&lighting, cur_time);
        scene_eclipsia.draw(Drawer::Type::FILL, 2);
        scene_eclipsia_contour.update_shader(&lighting, cur_time);
        scene_eclipsia_contour.draw(Drawer::Type::CONTOUR, 1);
        // check if ready to switch scene
        if (cur_time > start_time + scene_duration)
        {
            start_time = cur_time;
            scene = qe::math::choice(scene_sequence);

            if (scene == 3) { start_time_3 = cur_time; }

            // stop descending and start ascending transition
            descending_transition = false;
            ascending_transition = true;
            start_time_transition = cur_time;

            // lighting type
            if ((scene == 1 or scene == 9) and qe::math::random(0, 1) < 0.5) { ultra_exposed = true; }
            else { ultra_exposed = false; }

            // camera type
            float sample = qe::math::random(0, 1);
            if (sample < 0.3) { camera_pos = 1; }
            else if (sample < 0.6) { camera_pos = 2; }
            else { camera_pos = 0; }

            qe::print::info("Drawing scene " + std::to_string(scene) + " exposed: " + std::to_string(ultra_exposed) + " camera: " + std::to_string(camera_pos));
        }
        if (cur_time > start_time + scene_duration - transition_duration and not descending_transition) {
            qe::print::info("Starting descending transition");
            descending_transition = true;
            start_time_transition = cur_time;
        }
        if (cur_time > start_time + transition_duration and ascending_transition) {
            qe::print::info("Stopping ascending transition");
            ascending_transition = false;
        }
        if (scene == 0)
        {
            scenes.at(0).update_shader(&lighting, cur_time);
            scenes.at(0).draw(Drawer::Type::FILL, 2);
        }
        else if (scene == 1)
        {
            scenes.at(1).update_shader(&lighting, cur_time);
            scenes.at(1).draw(Drawer::Type::FILL, 2);
        }
        else if (scene == 2)
        {
            scenes.at(2).update_shader(&lighting, cur_time);
            scenes.at(2).draw(Drawer::Type::FILL, 2);
        }
        else if (scene == 3)
        {
            if (cur_time > start_time_3 + 0.1)
            {
                if (ascending_3) {scene_index_3++;}
                else {scene_index_3--;}
                if (scene_index_3 == 3) {ascending_3 = true;}
                if (scene_index_3 == 8) {ascending_3 = false;}
                start_time_3 = cur_time;
            }
            scenes.at(scene_index_3).update_shader(&lighting, cur_time);
            scenes.at(scene_index_3).draw(Drawer::Type::FILL, 2);
        }
        else if (scene == 9)
        {
            scenes.at(9).update_shader(&lighting, cur_time);
            scenes.at(9).draw(Drawer::Type::FILL, 2);
        }
    }

    // Clean up
    qe::api::terminate();

    return 0;
}
