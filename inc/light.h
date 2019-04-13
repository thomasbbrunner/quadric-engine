#ifndef LIGHT_H
#define LIGHT_H

#include "geometry.h"
#include "geometry3D.h"
#include "draw.h"
#include "thing.h"

#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POSITIONAL 1

class Light
{
  public:
    void add_geometry(Geometry geometry = Icosahedron(0.2f))
    {
        thing.add_geometry(geometry);
        thing.generate_buffers();
        thing.set_shader(shader);
    }

    void draw()
    {
        thing.update_shader();
        thing.draw(DW_FILL);
    }

    void set_position(glm::vec3 pos)
    {
        position = pos;
        thing.set_model(glm::translate(glm::mat4(1.0f), position));
    }

    void set_color(glm::vec4 col)
    {
        color = col;
        thing.set_color(color);
    }

    void set_type(unsigned int type_in)
    {
        type = type_in;
    }

    glm::vec3 get_position()
    {
        return position;
    }

    glm::vec4 get_color()
    {
        return color;
    }

    int get_type()
    {
        return type;
    }

  private:
    glm::vec3 position = glm::vec3(0.0);
    glm::vec4 color = glm::vec4(1.0);
    int type = LIGHT_POSITIONAL;
    // Directional light has w = 0
    // Positional light has w = 1
    Shader shader;
    Thing thing;
};

class Lighting
{
  public:
    void add_source(Light *light)
    {
        lights.push_back(light);
    }

    void update_shader(Shader shader)
    // Updates other component's shaders to add lighting effects
    {
        for (unsigned int i = 0; i < lights.size(); i++)
        {
            std::stringstream temp;
            temp << "light[" << i << "]";

            shader.set_vec4(temp.str() + ".color", lights.at(i)->get_color());
            shader.set_vec3(temp.str() + ".pos", lights.at(i)->get_position());
            shader.set_int(temp.str() + ".type", lights.at(i)->get_type());
        }
    }

    void update()
    {
        for (unsigned int i = 0; i < lights.size(); i++)
        {
            lights.at(i)->draw();
        }
    }

  private:
    std::vector<Light *> lights;
};

#endif