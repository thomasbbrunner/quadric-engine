
#pragma once

#include "buffer.hpp"
#include "camera.hpp"
#include "draw.hpp"
#include "geometry.hpp"
#include "math.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>

class Lighting;

class Scene
{
public:
    void add_geometry(Geometry geometry);

    void generate_buffers();

    void update_shader(Lighting *lighting = NULL);

    void draw(enum Drawer::Type type, float line_thickness = 1.0)
    {
        shader.use();

        drawer.draw(&buffers, type, line_thickness);
    }

    /*** VARIABLE ACCESS ***/

    void set_shader(Shader shader_in)
    {
        shader = shader_in;
    }

    void set_model(glm::mat4 model_temp)
    {
        model = model_temp;
    }

    void set_color(glm::vec4 color_temp)
    {
        color = color_temp;
    }

    Geometry geometry;

private:
    Tiktok &tiktok = Tiktok::get_instance();
    Camera &camera = Camera::get_instance();

    Buffers buffers;

    Drawer drawer;

    glm::mat4 model = glm::mat4(1.0f);
    glm::vec4 color = glm::vec4(1.0f);

    Shader shader{"std.vert", "std.frag"};
};
