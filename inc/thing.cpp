
#include "thing.h"

#include <algorithm>
#include <glm/glm.hpp>

#include "geometry.h"
#include "api.h"
#include "camera.h"
#include "light.h"
#include "shader.h"

void Thing::add_geometry(Geometry geometry)
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

void Thing::generate_buffers()
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

void Thing::update_shader(Lighting *lighting)
{
    shader.set_mat4("model", model);
    shader.set_vec4("color", color);
    shader.set_mat4("view", camera.get_view());
    shader.set_mat4("aspect", camera.get_aspect());
    shader.set_mat4("proj", camera.get_proj());

    shader.set_time(tiktok.get());

    if (lighting != NULL)
    {
        // Send lighting info to thing's shader
        lighting->update_shader(shader);
    }
}