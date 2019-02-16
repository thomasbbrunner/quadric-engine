#ifndef LIGHT_H
#define LIGHT_H

#include <common.h>
#include <shader.h>

class Lighting
{
  public:
    Lighting()
    {
    }

    int add_source(int type_in)
    {
        light.push_back(Light(type_in));

        return light.size() - 1;
    }

    void update_shader(Shader shader)
    {
        for (unsigned int i = 0; i < light.size(); i++)
        {
            std::stringstream temp;
            temp << "light[" << i << "]";
            
            shader.set_vec4(temp.str() + ".color", light.at(i).color);
            shader.set_vec3(temp.str() + ".pos", light.at(i).position);
            shader.set_int(temp.str() + ".type", light.at(i).type);
        }
    }

    void set_color(int light_ID, glm::vec4 color_in)
    {
        light.at(light_ID).color = color_in;
    }

    void set_position(int light_ID, glm::vec3 position_in)
    {
        light.at(light_ID).position = position_in;
    }

  private:
    class Light
    {
      public:
        glm::vec3 position = glm::vec3(0.0);
        glm::vec4 color = glm::vec4(0.0);
        int type = -1;
        // Directional light has w = 0
        // Positional light has w = 1

        Light(int type_in) : type(type_in)
        {
        }
    };

    std::vector<Light> light;
};

Lighting lighting;

#endif