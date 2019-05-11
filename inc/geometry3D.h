#ifndef GEOMETRY3D_H
#define GEOMETRY3D_H

#include "geometry.h"
#include "common.h"
#include "print.h"

#include <vector>
#include <glm/glm.hpp>

class Cube : public Geometry
{
  public:
    Cube(float size = 1.0f)
    {
        /*** MESH ***/

        vertex_coordinates_mesh = {
            glm::vec3(-size / 2, -size / 2, -size / 2),
            glm::vec3(size / 2, -size / 2, -size / 2),
            glm::vec3(size / 2, size / 2, -size / 2),
            glm::vec3(-size / 2, size / 2, -size / 2),
            glm::vec3(-size / 2, -size / 2, size / 2),
            glm::vec3(size / 2, -size / 2, size / 2),
            glm::vec3(size / 2, size / 2, size / 2),
            glm::vec3(-size / 2, size / 2, size / 2),
            glm::vec3(-size / 2, size / 2, size / 2),
            glm::vec3(-size / 2, size / 2, -size / 2),
            glm::vec3(-size / 2, -size / 2, -size / 2),
            glm::vec3(-size / 2, -size / 2, size / 2),
            glm::vec3(size / 2, size / 2, size / 2),
            glm::vec3(size / 2, size / 2, -size / 2),
            glm::vec3(size / 2, -size / 2, -size / 2),
            glm::vec3(size / 2, -size / 2, size / 2),
            glm::vec3(-size / 2, -size / 2, -size / 2),
            glm::vec3(size / 2, -size / 2, -size / 2),
            glm::vec3(size / 2, -size / 2, size / 2),
            glm::vec3(-size / 2, -size / 2, size / 2),
            glm::vec3(-size / 2, size / 2, -size / 2),
            glm::vec3(size / 2, size / 2, -size / 2),
            glm::vec3(size / 2, size / 2, size / 2),
            glm::vec3(-size / 2, size / 2, size / 2)};

        vertex_indices_mesh = {
            0, 2, 1,
            0, 3, 2,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            12, 14, 13,
            12, 15, 14,
            16, 17, 18,
            16, 18, 19,
            20, 22, 21,
            20, 23, 22};

        /*** CONTOUR ***/

        vertex_coordinates_contour = {
            glm::vec3(size / 2, size / 2, size / 2),
            glm::vec3(-size / 2, size / 2, size / 2),
            glm::vec3(-size / 2, -size / 2, size / 2),
            glm::vec3(size / 2, -size / 2, size / 2),
            glm::vec3(size / 2, size / 2, -size / 2),
            glm::vec3(-size / 2, size / 2, -size / 2),
            glm::vec3(-size / 2, -size / 2, -size / 2),
            glm::vec3(size / 2, -size / 2, -size / 2)};

        vertex_indices_contour = {
            0, 1,
            1, 2,
            2, 3,
            3, 0,
            4, 5,
            5, 6,
            6, 7,
            7, 4,
            0, 4,
            1, 5,
            2, 6,
            3, 7};

        /*** DOTS ***/

        vertex_coordinates_dots = vertex_coordinates_contour;
    }
};

class Polyhedron : public Geometry
// TODO: without normalization of icosahedron you get a cylinder!
{
  public:
    Polyhedron(float d = 1.0f, unsigned int res = 5)
    {
        // First vertex
        vertex_coordinates_mesh.push_back(glm::vec3(0.0, 1.0f, 0.0));

        unsigned int horizontal_poly = 4 + res;
        unsigned int vertical_poly = 4 + 2 * res;
        float horizontal_angle = glm::radians(360.0 / (float)horizontal_poly);
        float vertical_angle = glm::radians(360.0 / (float)vertical_poly);

        // Middle vertices
        for (unsigned int i = 1; i < vertical_poly / 2; i++)
        {
            for (unsigned int j = 0; j < horizontal_poly; j++)
            {
                float height_constant = glm::pi<float>() / 2.0 - i * vertical_angle;

                float x = glm::cos(j * horizontal_angle + (i - 1) * horizontal_angle / 2.0) * glm::cos(height_constant);
                float z = glm::sin(j * horizontal_angle + (i - 1) * horizontal_angle / 2.0) * glm::cos(height_constant);
                float y = glm::sin(height_constant);

                vertex_coordinates_mesh.push_back(glm::vec3(x, y, z));
            }
        }

        // Last vertex
        vertex_coordinates_mesh.push_back(glm::vec3(0.0, -1.0f, 0.0));

        Math::multiply_by_scalar(&vertex_coordinates_mesh, d);

        // Indices top layer
        for (unsigned int i = 1; i <= horizontal_poly; i++)
        {
            vertex_indices_mesh.push_back(0);
            vertex_indices_mesh.push_back(i == horizontal_poly ? 1 : i + 1);
            vertex_indices_mesh.push_back(i);
        }

        // Indices middle layers
        for (unsigned int i = 0; i < vertical_poly / 2 - 2; i++)
        {
            for (unsigned int j = 1; j <= horizontal_poly; j++)
            {
                unsigned int k = j + i * horizontal_poly;

                vertex_indices_mesh.push_back(k);
                vertex_indices_mesh.push_back(j == horizontal_poly ? (i)*horizontal_poly + 1 : k + 1);
                vertex_indices_mesh.push_back(k + horizontal_poly);

                vertex_indices_mesh.push_back(k);
                vertex_indices_mesh.push_back(k + horizontal_poly);
                vertex_indices_mesh.push_back(j == 1 ? (i + 2) * horizontal_poly : k + horizontal_poly - 1);
            }
        }

        // Indices bottom layer
        for (unsigned int i = 1; i <= horizontal_poly; i++)
        {
            vertex_indices_mesh.push_back(horizontal_poly * (vertical_poly / 2 - 1) + 1);
            vertex_indices_mesh.push_back(horizontal_poly * (vertical_poly / 2 - 2) + i);
            vertex_indices_mesh.push_back(i == horizontal_poly ? horizontal_poly * (vertical_poly / 2 - 2) + 1 : horizontal_poly * (vertical_poly / 2 - 2) + i + 1);
        }
    }
};

class Icosahedron : public Geometry
{
  public:
    Icosahedron(float d = 1.0f)
    {
        float t = (1.0f + std::sqrt(5.0f)) / 2.0f;
        float a = 1.0f / (std::sqrt(1.0f + t * t));

        vertex_coordinates_mesh = {
            d * glm::vec3(a * t, a, 0),
            d * glm::vec3(-a * t, a, 0),
            d * glm::vec3(a * t, -a, 0),
            d * glm::vec3(-a * t, -a, 0),
            d * glm::vec3(a, 0, a * t),
            d * glm::vec3(a, 0, -a * t),
            d * glm::vec3(-a, 0, a * t),
            d * glm::vec3(-a, 0, -a * t),
            d * glm::vec3(0, a * t, a),
            d * glm::vec3(0, -a * t, a),
            d * glm::vec3(0, a * t, -a),
            d * glm::vec3(0, -a * t, -a)};

        vertex_indices_mesh = {
            0, 8, 4,
            0, 5, 10,
            2, 4, 9,
            2, 11, 5,
            1, 6, 8,
            1, 10, 7,
            3, 9, 6,
            3, 7, 11,
            0, 10, 8,
            1, 8, 10,
            2, 9, 11,
            3, 9, 11,
            4, 2, 0,
            5, 0, 2,
            6, 1, 3,
            7, 3, 1,
            8, 6, 4,
            9, 4, 6,
            10, 5, 7,
            11, 7, 5};
    }
};

class Sphere : public Polyhedron
{
  public:
    Sphere(float d = 1.0f, unsigned int res = 10) : Polyhedron(d, res)
    {
    }
};

class Sphere_half_brute_force : public Geometry
{
  public:
    Sphere_half_brute_force(float d = 1.0f)
    {
        unsigned int res = 1;
        unsigned int vertical_poly = 6 * std::pow(2, res);
        float vertical_angle = glm::radians(360.0 / (float)vertical_poly);
        std::vector<unsigned int> horizontal_poly_vec;

        // First Layer
        {
            // Vertex
            vertex_coordinates_mesh.push_back(glm::vec3(0.0, 1.0f, 0.0));
            // Indices
            unsigned int horizontal_poly = 5;
            horizontal_poly_vec.push_back(1);
            for (unsigned int i = 1; i <= horizontal_poly; i++)
            {
                vertex_indices_mesh.push_back(0);
                vertex_indices_mesh.push_back(i == horizontal_poly ? 1 : i + 1);
                vertex_indices_mesh.push_back(i);
            }
        }

        // Middle layers
        for (unsigned int i = 1; i < vertical_poly / 2; i++)
        {
            unsigned int horizontal_poly;
            bool stage_1 = i < vertical_poly / 6;
            bool stage_2 = i >= vertical_poly / 6 && i <= vertical_poly / 6 * 2;
            bool stage_3 = i > vertical_poly / 6 * 2;
            // debug
            if ((stage_1 ? 1 : 0) + (stage_2 ? 1 : 0) + (stage_3 ? 1 : 0) > 1)
            {
                printf("Error in the logic\n");
                exit(0);
            }

            if (stage_1)
                horizontal_poly = i * 5;
            else if (stage_2)
                horizontal_poly = vertical_poly / 6 * 5;
            else if (stage_3)
                horizontal_poly = (vertical_poly / 2 - i) * 5;
            else
            {
                printf("Error stage unrecognized\n");
                exit(0);
            }

            horizontal_poly_vec.push_back(horizontal_poly);
            float horizontal_angle = glm::radians(360.0 / (float)horizontal_poly);
            printf("stage: %i%i%i horz poly: %u\n", stage_1, stage_2, stage_3, horizontal_poly);

            for (unsigned int j = 0; j < horizontal_poly; j++)
            {
                // Vertices
                float height_constant = glm::pi<float>() / 2.0 - i * vertical_angle;

                float x = glm::cos(j * horizontal_angle + (i - 1) * horizontal_angle / 2.0) * glm::cos(height_constant);
                float z = glm::sin(j * horizontal_angle + (i - 1) * horizontal_angle / 2.0) * glm::cos(height_constant);
                float y = glm::sin(height_constant);

                vertex_coordinates_mesh.push_back(glm::vec3(x, y, z));
            }
        }

        // Indexes
        for (unsigned int i = 0; i < vertical_poly / 2 - 1; i++)
        {
            unsigned int max_k = 0;

            unsigned int horizontal_poly = horizontal_poly_vec.at(i + 1);
            for (unsigned int j = 1; j <= horizontal_poly; j++)
            {
                unsigned int k = j + i * horizontal_poly;

                vertex_indices_mesh.push_back(k);
                vertex_indices_mesh.push_back(j == horizontal_poly ? (i)*horizontal_poly + 1 : k + 1);
                vertex_indices_mesh.push_back(k + horizontal_poly);

                vertex_indices_mesh.push_back(k);
                vertex_indices_mesh.push_back(k + horizontal_poly);
                vertex_indices_mesh.push_back(j == 1 ? (i + 2) * horizontal_poly : k + horizontal_poly - 1);

                if (k > max_k)
                    max_k = k;
            }
        }

        // Bottom layer
        {
            // Vertex
            vertex_coordinates_mesh.push_back(glm::vec3(0.0, -1.0f, 0.0));
            // Indices
            unsigned int horizontal_poly = 5;
            horizontal_poly_vec.push_back(1);
            for (unsigned int i = 1; i <= horizontal_poly; i++)
            {
                vertex_indices_mesh.push_back(horizontal_poly * (vertical_poly / 2 - 1) + 1);
                vertex_indices_mesh.push_back(horizontal_poly * (vertical_poly / 2 - 2) + i);
                vertex_indices_mesh.push_back(i == horizontal_poly ? horizontal_poly * (vertical_poly / 2 - 2) + 1 : horizontal_poly * (vertical_poly / 2 - 2) + i + 1);
            }
        }

        float dist = glm::length(
            vertex_coordinates_mesh.at(0) - vertex_coordinates_mesh.at(1));

        printf("dist: %.5f\n", dist);

        vertex_indices_mesh.clear();

        for (unsigned int i = 0; i < vertex_coordinates_mesh.size(); i++)
        {
            for (unsigned int j = 0; j < vertex_coordinates_mesh.size(); j++)
            {
                for (unsigned int k = 0; k < vertex_coordinates_mesh.size(); k++)
                {
                    if (i == j || i == k || k == j)
                    {
                        continue;
                    }

                    float dist_ij = glm::length(vertex_coordinates_mesh.at(i) - vertex_coordinates_mesh.at(j));
                    float dist_jk = glm::length(vertex_coordinates_mesh.at(j) - vertex_coordinates_mesh.at(k));
                    float dist_ki = glm::length(vertex_coordinates_mesh.at(k) - vertex_coordinates_mesh.at(i));

                    if (std::abs(dist_ij - dist_ki) > 0.001 ||
                        std::abs(dist_ij - dist_jk) > 0.001 ||
                        std::abs(dist_jk - dist_ki) > 0.001)
                    {
                        // continue;
                    }
                    printf("dist: %.5f\n", dist_ij);

                    // if (dist_ij <= dist && dist_jk <= dist && dist_ki <= dist)
                    {
                        glm::vec3 vec_midpoint = (vertex_coordinates_mesh.at(i) +
                                                  vertex_coordinates_mesh.at(j) +
                                                  vertex_coordinates_mesh.at(k));

                        glm::vec3 vec_cross = glm::cross(vertex_coordinates_mesh.at(k) - vertex_coordinates_mesh.at(i),
                                                         vertex_coordinates_mesh.at(j) - vertex_coordinates_mesh.at(k));

                        float vec_angle = glm::acos(glm::dot(glm::normalize(vec_cross), glm::normalize(vec_midpoint)));

                        if (std::abs(vec_angle - 0.0) > 1e-5)
                        {
                            continue;
                        }

                        vec_cross = glm::cross(vertex_coordinates_mesh.at(j) - vertex_coordinates_mesh.at(k),
                                               vertex_coordinates_mesh.at(i) - vertex_coordinates_mesh.at(j));

                        vec_angle = glm::acos(glm::dot(glm::normalize(vec_cross), glm::normalize(vec_midpoint)));

                        if (std::abs(vec_angle - 0.0) > 1e-5)
                        {
                            // continue;
                        }

                        vec_cross = glm::cross(vertex_coordinates_mesh.at(i) - vertex_coordinates_mesh.at(j),
                                               vertex_coordinates_mesh.at(k) - vertex_coordinates_mesh.at(i));

                        vec_angle = glm::acos(glm::dot(glm::normalize(vec_cross), glm::normalize(vec_midpoint)));

                        if (std::abs(vec_angle - 0.0) > 1e-5)
                        {
                            // continue;
                        }

                        vertex_indices_mesh.push_back(i);
                        vertex_indices_mesh.push_back(j);
                        vertex_indices_mesh.push_back(k);
                    }
                }
            }
        }

        // Math::normalize(&vertex_coordinates_mesh);
        printf("%lu\n", vertex_indices_mesh.size());
        Print::array(vertex_indices_mesh, 3);

        Math::multiply_by_scalar(&vertex_coordinates_mesh, 10.0f);

        vertex_coordinates_dots = vertex_coordinates_mesh;
        vertex_coordinates_mesh.clear();
    }
};

class Sphere_brute_force : public Geometry
{
  public:
    Sphere_brute_force(float d = 1.0f)
    {
        float t = (1.0f + std::sqrt(5.0f)) / 2.0f;
        float a = 1.0f / (std::sqrt(1.0f + t * t));

        vertex_coordinates_mesh = {
            glm::vec3(a * t, a, 0),
            glm::vec3(-a * t, a, 0),
            glm::vec3(a * t, -a, 0),
            glm::vec3(-a * t, -a, 0),
            glm::vec3(a, 0, a * t),
            glm::vec3(a, 0, -a * t),
            glm::vec3(-a, 0, a * t),
            glm::vec3(-a, 0, -a * t),
            glm::vec3(0, a * t, a),
            glm::vec3(0, -a * t, a),
            glm::vec3(0, a * t, -a),
            glm::vec3(0, -a * t, -a)};

        vertex_indices_mesh = {
            0, 8, 4,
            0, 5, 10,
            2, 4, 9,
            2, 11, 5,
            1, 6, 8,
            1, 10, 7,
            3, 9, 6,
            3, 7, 11,
            0, 10, 8,
            1, 8, 10,
            2, 9, 11,
            3, 9, 11,
            4, 2, 0,
            5, 0, 2,
            6, 1, 3,
            7, 3, 1,
            8, 6, 4,
            9, 4, 6,
            10, 5, 7,
            11, 7, 5};

        std::vector<glm::vec3> vertex_coordinates_mesh_backup = vertex_coordinates_mesh;
        std::vector<unsigned int> vertex_indices_mesh_backup = vertex_indices_mesh;

        for (unsigned int reps = 0; reps < 1; reps++)
        {
            // Normalizing vertices
            Math::normalize(&vertex_coordinates_mesh);

            // Creating new vertices
            std::vector<glm::vec3> vertices_new;
            glm::mat3 trans(0.5f);

            for (unsigned int i = 0; i < vertex_indices_mesh.size(); i += 3)
            {
                vertices_new.push_back(trans *
                                       (vertex_coordinates_mesh.at(vertex_indices_mesh.at(i)) + vertex_coordinates_mesh.at(vertex_indices_mesh.at(i + 1))));

                vertices_new.push_back(trans *
                                       (vertex_coordinates_mesh.at(vertex_indices_mesh.at(i + 1)) + vertex_coordinates_mesh.at(vertex_indices_mesh.at(i + 2))));

                vertices_new.push_back(trans *
                                       (vertex_coordinates_mesh.at(vertex_indices_mesh.at(i + 2)) + vertex_coordinates_mesh.at(vertex_indices_mesh.at(i))));
            }

            // Removing repeated vertices
            for (unsigned int i = 0; i < vertices_new.size(); i++)
            {
                for (unsigned int j = 0; j < vertices_new.size(); j++)
                {
                    if (i >= vertices_new.size())
                        continue;
                    if (i == j)
                        continue;

                    if (vertices_new.at(i) == vertices_new.at(j))
                    {
                        vertices_new.erase(vertices_new.begin() + j);
                    }
                }
            }

            // Joining new and old vertices
            vertex_coordinates_mesh.insert(vertex_coordinates_mesh.end(), vertices_new.begin(), vertices_new.end());

            // Normalizing vertices
            Math::normalize(&vertex_coordinates_mesh);

            vertex_coordinates_dots = vertex_coordinates_mesh;

            // Create indices
            float dist = glm::length(
                vertex_coordinates_mesh.at(vertex_indices_mesh.at(0)) -
                vertex_coordinates_mesh.at(vertex_indices_mesh.at(1)));

            vertex_indices_mesh.clear();

            for (unsigned int i = 0; i < vertex_coordinates_mesh.size(); i++)
            {
                for (unsigned int j = 0; j < vertex_coordinates_mesh.size(); j++)
                {
                    for (unsigned int k = 0; k < vertex_coordinates_mesh.size(); k++)
                    {
                        if (i == j || i == k || k == j)
                        {
                            continue;
                        }

                        float dist_ij = glm::length(vertex_coordinates_mesh.at(i) - vertex_coordinates_mesh.at(j));
                        float dist_jk = glm::length(vertex_coordinates_mesh.at(j) - vertex_coordinates_mesh.at(k));
                        float dist_ki = glm::length(vertex_coordinates_mesh.at(k) - vertex_coordinates_mesh.at(i));

                        if (std::abs(dist_ij - dist_ki) > 0.0001 ||
                            std::abs(dist_ij - dist_jk) > 0.0001 ||
                            std::abs(dist_jk - dist_ki) > 0.0001)
                        {
                            // continue;
                        }

                        if (dist_ij < dist && dist_jk < dist && dist_ki < dist)
                        {
                            glm::vec3 vec_midpoint = (vertex_coordinates_mesh.at(i) +
                                                      vertex_coordinates_mesh.at(j) +
                                                      vertex_coordinates_mesh.at(k));

                            glm::vec3 vec_cross = glm::cross(vertex_coordinates_mesh.at(k) - vertex_coordinates_mesh.at(i),
                                                             vertex_coordinates_mesh.at(j) - vertex_coordinates_mesh.at(k));

                            float vec_angle = glm::acos(glm::dot(glm::normalize(vec_cross), glm::normalize(vec_midpoint)));

                            printf("%.5f\n", vec_angle);
                            if (std::abs(vec_angle - 0.0) > 1e-5)
                            {
                                // continue;
                                // vertices are wrong -> there are not only equilateral triangles!
                            }

                            vec_cross = glm::cross(vertex_coordinates_mesh.at(j) - vertex_coordinates_mesh.at(k),
                                                   vertex_coordinates_mesh.at(i) - vertex_coordinates_mesh.at(j));

                            vec_angle = glm::acos(glm::dot(glm::normalize(vec_cross), glm::normalize(vec_midpoint)));

                            if (std::abs(vec_angle - 0.0) > 1e-5)
                            {
                                // continue;
                            }

                            vec_cross = glm::cross(vertex_coordinates_mesh.at(i) - vertex_coordinates_mesh.at(j),
                                                   vertex_coordinates_mesh.at(k) - vertex_coordinates_mesh.at(i));

                            vec_angle = glm::acos(glm::dot(glm::normalize(vec_cross), glm::normalize(vec_midpoint)));

                            if (std::abs(vec_angle - 0.0) > 1e-5)
                            {
                                // continue;
                            }

                            vertex_indices_mesh.push_back(i);
                            vertex_indices_mesh.push_back(j);
                            vertex_indices_mesh.push_back(k);
                        }
                    }
                }
            }

            printf("Size: %lu\n", vertex_indices_mesh.size());
            Print::array(vertex_indices_mesh, 3);

            // Remove repeated
            // for (unsigned int n = 0; n < 2; n++)
            // {
            //     for (unsigned int i = 0; i < vertex_indices_mesh.size(); i += 3)
            //     {
            //         for (unsigned int j = 0; j < vertex_indices_mesh.size(); j += 3)
            //         {
            //             // printf("i: %u j: %u size: %lu\n", i, j, vertex_indices_mesh.size());

            //             if (vertex_indices_mesh.at(i) == vertex_indices_mesh.at(j) &&
            //                 vertex_indices_mesh.at(i + 1) == vertex_indices_mesh.at(j + 1) &&
            //                 vertex_indices_mesh.at(i + 2) == vertex_indices_mesh.at(j + 2))
            //                 continue;
            //             // if (i >= vertex_indices_mesh.size() - 2 || j >= vertex_indices_mesh.size() - 2)
            //             // continue;

            //             if (vertex_indices_mesh.at(i) == vertex_indices_mesh.at(j) ||
            //                 vertex_indices_mesh.at(i) == vertex_indices_mesh.at(j + 1) ||
            //                 vertex_indices_mesh.at(i) == vertex_indices_mesh.at(j + 2))
            //             {
            //                 if (vertex_indices_mesh.at(i + 1) == vertex_indices_mesh.at(j) ||
            //                     vertex_indices_mesh.at(i + 1) == vertex_indices_mesh.at(j + 1) ||
            //                     vertex_indices_mesh.at(i + 1) == vertex_indices_mesh.at(j + 2))
            //                 {
            //                     if (vertex_indices_mesh.at(i + 2) == vertex_indices_mesh.at(j) ||
            //                         vertex_indices_mesh.at(i + 2) == vertex_indices_mesh.at(j + 1) ||
            //                         vertex_indices_mesh.at(i + 2) == vertex_indices_mesh.at(j + 2))
            //                     {
            //                         vertex_indices_mesh.erase(vertex_indices_mesh.begin() + j, vertex_indices_mesh.begin() + j + 3);
            //                     }
            //                 }
            //             }
            //         }
            //     }
            // }

            printf("Size: %lu\n", vertex_indices_mesh.size());
            Print::array(vertex_indices_mesh, 3);
        }

        // vertex_coordinates_mesh = vertex_coordinates_mesh_backup;
        // vertex_indices_mesh = vertex_indices_mesh_backup;

        // Normalizing vertices
        Math::normalize(&vertex_coordinates_mesh);

        // Multiplying by size
        glm::mat3 trans(d);
        for (unsigned int i = 0; i < vertex_coordinates_mesh.size(); i++)
        {
            vertex_coordinates_mesh.at(i) = trans * vertex_coordinates_mesh.at(i);
        }
        for (unsigned int i = 0; i < vertex_coordinates_dots.size(); i++)
        {
            vertex_coordinates_dots.at(i) = trans * vertex_coordinates_dots.at(i);
        }
    }
};

class Plane : public Geometry
{
  public:
    Plane(int dx, int dz, int dotres = 1)
    {
        /*** MESH ***/

        // Generating vertex coordinates arranged in 2D format
        std::vector<std::vector<glm::vec3>> coords = generate_vertices(dotres, dx, dz);

        // Transforming vertex coordinates to 1D format
        for (unsigned int i = 0; i < coords.size(); i++)
        {
            for (unsigned int j = 0; j < coords.at(0).size(); j++)
            {
                // Vertex coordinates
                vertex_coordinates_mesh.push_back(coords.at(i).at(j));
            }
        }

        // Generating indices for mesh
        for (unsigned int i = 0; i < coords.size() - 1; i++)
        {
            for (unsigned int j = 0; j < coords.at(0).size() - 1; j++)
            {
                vertex_indices_mesh.push_back(j + i * coords.at(0).size());
                vertex_indices_mesh.push_back(j + (i + 1) * coords.at(0).size() + 1);
                vertex_indices_mesh.push_back(j + (i + 1) * coords.at(0).size());
                vertex_indices_mesh.push_back(j + i * coords.at(0).size());
                vertex_indices_mesh.push_back(j + i * coords.at(0).size() + 1);
                vertex_indices_mesh.push_back(j + (i + 1) * coords.at(0).size() + 1);
            }
        }

        /*** DOTS ***/

        vertex_coordinates_dots = vertex_coordinates_mesh;
    }

  private:
    static std::vector<std::vector<glm::vec3>> generate_vertices(int dotres, int dx, int dz)
    {
        int numdotsx = (int)(dotres * dx + 1.0);
        int numdotsz = (int)(dotres * dz + 1.0);

        std::vector<std::vector<glm::vec3>> coords;
        coords.resize(numdotsx, std::vector<glm::vec3>(numdotsz, glm::vec3(0.0)));

        for (int i = 0; i < numdotsx; i++)
        {
            for (int j = 0; j < numdotsz; j++)
            {
                coords.at(i).at(j) = glm::vec3(-dx / 2.0f + i * dx / (float)(numdotsx - 1), 0.0f, -dz / 2.0f + j * dz / (float)(numdotsz - 1));
            }
        }

        return coords;
    }
};

#endif