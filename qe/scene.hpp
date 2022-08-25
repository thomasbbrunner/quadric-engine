
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
    Scene(Camera &camera) : camera_(camera)
    {
    }

    void add_geometry(Geometry geometry);

    void generate_buffers();

    // TODO remove this absurdity of a NULL pointer
    // and remove default value for time 
    void update_shader(Lighting *lighting = NULL, float time = 0.0);

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
    Buffers buffers;
    Camera &camera_;

    Drawer drawer;

    glm::mat4 model = glm::mat4(1.0f);
    glm::vec4 color = glm::vec4(1.0f);

    Shader shader{"std.vert", "std.frag"};
};
