#ifndef GEOMETRY2D_H
#define GEOMETRY2D_H

#include <geometry.h>
#include <print.h>

/*** 2D GEOMETRY ***/

class Geometry_2D : public Geometry
{
  public:
    // Uses GL_TRIANGLE_FAN instead of GL_TRIANGLES
    void draw_fill()
    {
        update_shader();

        glBindTexture(GL_TEXTURE_BUFFER, texture_vertex_data);

        glBindVertexArray(VAO_contour);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLE_FAN, numelements_contour, GL_UNSIGNED_INT, 0);
    }

    void draw_wireframe()
    {
        update_shader();

        glBindTexture(GL_TEXTURE_BUFFER, texture_vertex_data);

        glBindVertexArray(VAO_contour);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLE_FAN, numelements_contour, GL_UNSIGNED_INT, 0);
    }
};

class Pentagon : public Geometry_2D
{
  public:
    Pentagon(float d = 1.0f)
    {
        float h = d * sqrt(3) / 2;
        std::vector<glm::vec3> vertex_coordinates{
            glm::vec3(d, 0.0f, 0.0f),
            glm::vec3(d / 2, h, 0.0f),
            glm::vec3(-d / 2, h, 0.0f),
            glm::vec3(-d, 0.0f, 0.0f),
            glm::vec3(-d / 2, -h, 0.0f),
            glm::vec3(d / 2, -h, 0.0f)};

        std::vector<unsigned int> vertex_indices{
            0, 1,
            1, 2,
            2, 3,
            3, 4,
            4, 5,
            5, 0};

        /*** MESH/CONTOUR ***/

        Contour contour(vertex_coordinates, vertex_indices);

        generate_buffers(contour);

        /*** DOTS ***/

        Dots dots(vertex_coordinates);

        generate_buffers(dots);
    }
};

class Tetra : public Geometry_2D
{
  public:
    Tetra(float size = 1.0f)
    {
        float h = 0.55f;
        float d = 1.0f;
        float dT = 0.4f;

        std::vector<glm::vec3> vertex_coordinates{
            size * glm::vec3(d, -h, 0),
            size * glm::vec3(d + dT, h, 0),
            size * glm::vec3(-d, h, 0),
            size * glm::vec3(-d - dT, -h, 0)};

        std::vector<unsigned int> vertex_indices{
            0, 1,
            1, 2,
            2, 3,
            3, 0};

        /*** MESH/CONTOUR ***/

        Contour contour(vertex_coordinates, vertex_indices);

        generate_buffers(contour);

        /*** DOTS ***/

        Dots dots(vertex_coordinates);

        generate_buffers(dots);
    }
};

class Circle : public Geometry_2D
{
  public:
    Circle(float d = 5.0f, unsigned int numlin = 100)
    {
        std::vector<glm::vec3> vertex_coordinates;
        std::vector<unsigned int> vertex_indices;

        for (unsigned int i = 0; i < numlin; i++)
        {
            vertex_coordinates.push_back(glm::vec3(
                d * std::cos(i * 2 * M_PI / numlin),
                d * std::sin(i * 2 * M_PI / numlin),
                0.0));

            vertex_indices.push_back(i);
            if (i == numlin - 1)    
                vertex_indices.push_back(0);
            else
                vertex_indices.push_back(i + 1);
        }

        /*** MESH/CONTOUR ***/

        Contour contour(vertex_coordinates, vertex_indices);

        generate_buffers(contour);

        /*** DOTS ***/

        Dots dots(vertex_coordinates);

        generate_buffers(dots);
    }
};

#endif