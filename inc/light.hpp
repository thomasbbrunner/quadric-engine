
#pragma once

#include "geometry.hpp"
#include "geometry3D.hpp"
#include "draw.hpp"
#include "scene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <sstream>

class Light
{
public:
    enum class Type : int
    {
        DIRECTIONAL,
        POSITIONAL,
    };

    enum class Effect : int
    {
        NORMAL,
        MANUAL,
        FLICKER_FAST,
        FLICKER_SPORADIC,
        FLARE,
    };

    void add_geometry(Geometry geometry = Sphere())
    {
        has_geometry = true;

        scene.add_geometry(geometry);
        scene.generate_buffers();
        scene.set_shader(shader);
    }

    void draw()
    {
        if (has_geometry)
        {
            scene.update_shader();
            scene.draw(Drawer::Type::FILL);
        }
    }

    void set_position(glm::vec3 pos)
    {
        position = pos;
        scene.set_model(glm::translate(glm::mat4(1.0f), position));
    }

    void set_color(glm::vec4 col)
    {
        color = col;
        scene.set_color(brightness * color);
    }

    void set_type(enum Type type)
    {
        type_ = type;
    }

    glm::vec3 get_position()
    {
        return position;
    }

    glm::vec4 get_color()
    {
        return color;
    }

    enum Type get_type()
    {
        return type_;
    }

    void set_brightness(enum Effect effect, float brightness = 0.8)
    {
        if (effect == Effect::MANUAL)
        {
            if (glfwGetKey(opengl.window, GLFW_KEY_P) == GLFW_PRESS)
                ambient_strength += 0.1;
            if (glfwGetKey(opengl.window, GLFW_KEY_L) == GLFW_PRESS)
                ambient_strength -= 0.1;
            if (glfwGetKey(opengl.window, GLFW_KEY_O) == GLFW_PRESS)
                specular_strength += 0.1;
            if (glfwGetKey(opengl.window, GLFW_KEY_K) == GLFW_PRESS)
                specular_strength -= 0.1;
            if (glfwGetKey(opengl.window, GLFW_KEY_I) == GLFW_PRESS)
                diffuse_strength += 0.1;
            if (glfwGetKey(opengl.window, GLFW_KEY_J) == GLFW_PRESS)
                diffuse_strength -= 0.1;

            ambient_strength = glm::clamp(ambient_strength, 0.0f, 0.8f);
            diffuse_strength = glm::clamp(diffuse_strength, 0.0f, 0.8f);
            specular_strength = glm::clamp(specular_strength, 0.0f, 0.8f);

            printf("ambient: %.2f specular: %.2f diffuse: %.2f\n", ambient_strength, specular_strength, diffuse_strength);

            return;
        }

        if (effect == Effect::NORMAL)
        {
            this->brightness = glm::clamp(brightness, 0.0f, 0.8f);
        }
        else if (effect == Effect::FLICKER_FAST)
        {
            this->brightness = glm::clamp(quad::math::random(0.0, 0.8), 0.0f, 0.8f);
        }
        else if (effect == Effect::FLICKER_SPORADIC)
        {
            this->brightness = glm::clamp(quad::math::random(0.0, 20.0), 0.0f, 0.8f);
        }
        else if (effect == Effect::FLARE)
        {
            this->brightness = 1.0 + quad::math::random(-1.0, 1.0) / 10.0;
            this->brightness = glm::clamp(this->brightness, 0.2f, 2.0f);
        }

        ambient_strength = this->brightness;
        diffuse_strength = this->brightness;
        specular_strength = this->brightness;
    }

    void set_attenuation(float attenuation = 0.6)
    {
        // Input(value)    Output (cutoff_distance)
        //  0.0       ->   500.0
        //  1.0       ->    0.0

        attenuation = glm::clamp(attenuation, 0.0f, 1.0f);

        float cutoff_distance = 500.0 - attenuation * 500.0;

        this->attenuation_linear = quad::math::linear_interpolation(
            cutoff_distance, &attenuation_distance_data, &attenuation_linear_data);

        this->attenuation_quadratic = quad::math::linear_interpolation(
            cutoff_distance, &attenuation_distance_data, &attenuation_quadratic_data);
    }

    // Attenuation
    float attenuation_quadratic = 0.002;
    float attenuation_linear = 0.02;
    const std::vector<float> attenuation_distance_data{
        0, 7, 13, 20, 32, 50, 65, 100, 160, 200, 325, 600, 3250};
    const std::vector<float> attenuation_linear_data{
        1.0, 0.7, 0.35, 0.22, 0.14, 0.09, 0.07, 0.045, 0.027, 0.022, 0.014, 0.007, 0.0014};
    const std::vector<float> attenuation_quadratic_data{
        10.0, 1.8, 0.44, 0.2, 0.07, 0.032, 0.017, 0.0075, 0.0028, 0.0019, 0.0007, 0.0002, 0.00007};

    // Brightness
    float brightness = 1.0;
    float ambient_strength = 0.5;
    float diffuse_strength = 0.5;
    float specular_strength = 0.5;
    int specular_shininess = 64; // powers of two (2,4,8,16...256)

private:
    // Light attributes
    glm::vec3 position = glm::vec3(0.0);
    glm::vec4 color = glm::vec4(1.0);
    enum Type type_ = Type::POSITIONAL;
    // Directional light has w = 0
    // Positional light has w = 1

    Shader shader{"lighting.vert", "lighting.frag"};
    Scene scene;
    bool has_geometry = false;
    OpenGL &opengl = OpenGL::get_instance();
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
            // TODO: implement this in a dedicated class
            // (class for interfacing with shaders -> maybe Shader or another class)
            shader.set_vec4(temp.str() + ".color", lights.at(i)->get_color());
            shader.set_vec3(temp.str() + ".pos", lights.at(i)->get_position());
            shader.set_int(temp.str() + ".type", static_cast<int>(lights.at(i)->get_type()));

            shader.set_float("attenuation_constant", 1.0);
            shader.set_float("attenuation_linear", lights.at(i)->attenuation_linear);
            shader.set_float("attenuation_quadratic", lights.at(i)->attenuation_quadratic);

            shader.set_float("ambient_strength", lights.at(i)->ambient_strength);
            shader.set_float("diffuse_strength", lights.at(i)->diffuse_strength);
            shader.set_float("specular_strength", lights.at(i)->specular_strength);
            shader.set_int("specular_shininess", lights.at(i)->specular_shininess);
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
