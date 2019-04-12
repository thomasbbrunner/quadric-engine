#ifndef THING_H
#define THING_H

#include <common.h>

class Thing
{
  public:
    void add_geometry(Geometry geometry)
    {
        this->geometry.vertex_coordinates_mesh.insert(
            this->geometry.vertex_coordinates_mesh.end(),
            geometry.vertex_coordinates_mesh.begin(),
            geometry.vertex_coordinates_mesh.end());

        // Print::array(this->geometry.vertex_coordinates_mesh);

        this->geometry.vertex_coordinates_contour.insert(
            this->geometry.vertex_coordinates_contour.end(),
            geometry.vertex_coordinates_contour.begin(),
            geometry.vertex_coordinates_contour.end());

        this->geometry.vertex_coordinates_dots.insert(
            this->geometry.vertex_coordinates_dots.end(),
            geometry.vertex_coordinates_dots.begin(),
            geometry.vertex_coordinates_dots.end());

        // Adapting indices to new vertex IDs
        unsigned int max_index;
        if (this->geometry.vertex_indices_mesh.size() != 0)
        {
            max_index = 1 + *std::max_element(this->geometry.vertex_indices_mesh.begin(), this->geometry.vertex_indices_mesh.end());
        }
        else
        {
            max_index = 0;
        }

        Math::add_to_each(&geometry.vertex_indices_mesh, max_index);

        this->geometry.vertex_indices_mesh.insert(
            this->geometry.vertex_indices_mesh.end(),
            geometry.vertex_indices_mesh.begin(),
            geometry.vertex_indices_mesh.end());

        // Adapting indices to new vertex IDs
        if (this->geometry.vertex_indices_contour.size() != 0)
        {
            max_index = 1 + *std::max_element(this->geometry.vertex_indices_contour.begin(), this->geometry.vertex_indices_contour.end());
        }
        else
        {
            max_index = 0;
        }

        Math::add_to_each(&geometry.vertex_indices_contour, max_index);

        this->geometry.vertex_indices_contour.insert(
            this->geometry.vertex_indices_contour.end(),
            geometry.vertex_indices_contour.begin(),
            geometry.vertex_indices_contour.end());
    }

    void generate_buffers()
    {
        // Generate models
        Mesh mesh(this->geometry.vertex_coordinates_mesh, this->geometry.vertex_indices_mesh);
        Contour contour(this->geometry.vertex_coordinates_contour, this->geometry.vertex_indices_contour);
        Dots dots(this->geometry.vertex_coordinates_dots);

        // Generate buffers
        buffers.mesh = BufferGeneration::generate_from_mesh(mesh);
        buffers.contour = BufferGeneration::generate_from_contour(contour);
        buffers.dots = BufferGeneration::generate_from_dots(dots);
    }

    void update_shader()
    {
        shader.set_mat4("model", model);
        shader.set_vec4("color", color);
        shader.set_mat4("view", camera.get_view());
        shader.set_mat4("aspect", camera.get_aspect());
        shader.set_mat4("proj", camera.get_proj());

        shader.update_time(tiktok.get());

        // send light info to shader
        // if (light_ID == -1)
        // 	lighting.update_shader(shader);

        shader.use();
    }

    void draw(unsigned int type)
    {
        update_shader();

        drawer.draw(&buffers, type);
    }

    /*** VARIABLE ACCESS ***/

    void set_shader(Shader shader_in)
    {
        shader = shader_in;
    }

    // void set_light_source()
    // {
    // 	light_ID = lighting.add_source(1);
    // }

    void set_model(glm::mat4 model_temp)
    {
        model = model_temp;

        // update light (if applicable)
        // if (light_ID != -1)
        //     lighting.set_position(light_ID, glm::vec3(model_temp[3][0], model_temp[3][1], model_temp[3][2]));
    }

    void set_color(glm::vec4 color_temp)
    {
        color = color_temp;

        // update light (if applicable)
        // if (light_ID != -1)
        // 	lighting.set_color(light_ID, color_temp);
    }

  private:
    Geometry geometry;

    Buffers buffers;

    Drawer drawer;

    glm::mat4 model = glm::mat4(0.0f);
    glm::vec4 color = glm::vec4(0.0f);

    Shader shader;
    int light_ID = -1;
};

#endif