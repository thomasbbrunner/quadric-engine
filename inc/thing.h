#ifndef THING_H
#define THING_H

#include <common.h>

class Thing
{
  public:
    void add_geometry(Geometry geometry)
    {
        // Create model and append to thing's
        mesh.append(geometry.vertex_coordinates_mesh, geometry.vertex_indices_mesh);

        contour.append(geometry.vertex_coordinates_contour, geometry.vertex_indices_contour);

        dots.append(geometry.vertex_coordinates_dots);
    }

    void draw(unsigned int type)
    {
        update_shader();

        drawer.draw(&buffers, type);
    }

    void generate_buffers()
    {
        buffers.mesh = BufferGeneration::generate_from_mesh(mesh.model);
        buffers.contour = BufferGeneration::generate_from_contour(contour.model);
        buffers.dots = BufferGeneration::generate_from_dots(dots.model);
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
    Mesh mesh;
    Contour contour;
    Dots dots;

    Buffers buffers;

    Drawer drawer;

    glm::mat4 model = glm::mat4(0.0f);
    glm::vec4 color = glm::vec4(0.0f);

    Shader shader;
    int light_ID = -1;
};

#endif