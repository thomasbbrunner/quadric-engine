#ifndef GEOMETRY3D_H
#define GEOMETRY3D_H

#include "geometry.h"

#include <vector>
#include <glm/glm.hpp>

/*** 3D GEOMETRY ***/

class Cube : public Geometry
{
  public:
    Cube(float size = 0.5f, int dotres = 1)
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
/*
class Sphere : public Geometry
{
  public:
    Sphere(float d = 1.0f)
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

        // Normalizing vertices
        for (int i = 0; i < 12; i++)
        {
            vertex_coordinates_mesh.at(i) = glm::normalize(vertex_coordinates_mesh.at(i));
        }

        // Creating new vertices
        for (int i = 0; i < 20 * 3; i += 3)
        {
            vertices2[i][0] = (vertices0[indices0[i]][0] + vertices0[indices0[i + 1]][0]) / 2;
            vertices2[i][1] = (vertices0[indices0[i]][1] + vertices0[indices0[i + 1]][1]) / 2;
            vertices2[i][2] = (vertices0[indices0[i]][2] + vertices0[indices0[i + 1]][2]) / 2;

            vertices2[i + 1][0] = (vertices0[indices0[i + 1]][0] + vertices0[indices0[i + 2]][0]) / 2;
            vertices2[i + 1][1] = (vertices0[indices0[i + 1]][1] + vertices0[indices0[i + 2]][1]) / 2;
            vertices2[i + 1][2] = (vertices0[indices0[i + 1]][2] + vertices0[indices0[i + 2]][2]) / 2;

            vertices2[i + 2][0] = (vertices0[indices0[i + 2]][0] + vertices0[indices0[i]][0]) / 2;
            vertices2[i + 2][1] = (vertices0[indices0[i + 2]][1] + vertices0[indices0[i]][1]) / 2;
            vertices2[i + 2][2] = (vertices0[indices0[i + 2]][2] + vertices0[indices0[i]][2]) / 2;
        }

        float vertices3[30][3] = {{0.0f}};

        int k = 0;
        int repeated = -1;

        // Removing repeated vertices
        for (int i = 0; i < 20 * 3; i++)
        {
            for (int j = 0; j < 30; j++)
            {
                if ((std::abs(vertices2[i][0] - vertices3[j][0]) < 0.01f) && (std::abs(vertices2[i][1] - vertices3[j][1]) < 0.01f) && (std::abs(vertices2[i][2] - vertices3[j][2]) < 0.01f))
                {
                    // found repeated
                    repeated = 1;
                    break;
                }
                if (j == 30 - 1)
                {
                    // found not repeated
                    repeated = 0;
                }
            }
            if (repeated == 0)
            {
                vertices3[k][0] = vertices2[i][0];
                vertices3[k][1] = vertices2[i][1];
                vertices3[k][2] = vertices2[i][2];

                k++;
            }
        }

        // Joining new and old vertices
        float vertices4[42][3] = {{0.0f}};

        for (int i = 0; i < 42; i++)
        {
            if (i < 12)
            {
                vertices4[i][0] = vertices0[i][0];
                vertices4[i][1] = vertices0[i][1];
                vertices4[i][2] = vertices0[i][2];
            }
            else
            {
                vertices4[i][0] = vertices3[i - 12][0];
                vertices4[i][1] = vertices3[i - 12][1];
                vertices4[i][2] = vertices3[i - 12][2];
            }
        }

        // Create indices
        unsigned int indices1[20 * 4][3] = {{0}};

        float dist = std::abs(std::sqrt(std::pow(vertices0[0][0] - vertices0[8][0], 2) + std::pow(vertices0[0][1] - vertices0[8][1], 2) + std::pow(vertices0[0][2] - vertices0[8][2], 2)));

        int l = 0;
        for (unsigned int i = 0; i < 42; i++)
        {
            for (unsigned int j = 0; j < 42; j++)
            {
                for (unsigned int k = 0; k < 42; k++)
                {
                    if (i != j && i != k && k != j)
                    {
                        float dist1 = std::abs(std::sqrt(std::pow(vertices4[i][0] - vertices4[j][0], 2) + std::pow(vertices4[i][1] - vertices4[j][1], 2) + std::pow(vertices4[i][2] - vertices4[j][2], 2)));
                        float dist2 = std::abs(std::sqrt(std::pow(vertices4[i][0] - vertices4[k][0], 2) + std::pow(vertices4[i][1] - vertices4[k][1], 2) + std::pow(vertices4[i][2] - vertices4[k][2], 2)));
                        float dist3 = std::abs(std::sqrt(std::pow(vertices4[k][0] - vertices4[j][0], 2) + std::pow(vertices4[k][1] - vertices4[j][1], 2) + std::pow(vertices4[k][2] - vertices4[j][2], 2)));

                        if (std::abs(dist1 - dist2) < 0.001 && std::abs(dist1 - dist3) < 0.001 && std::abs(dist3 - dist2) < 0.001)
                        {
                            if (dist1 < dist)
                            {
                                int repeated = -1;
                                for (int m = 0; m < 20 * 4; m++)
                                {
                                    if (i == indices1[m][0])
                                    {
                                        if (j == indices1[m][1] || j == indices1[m][2])
                                        {
                                            if (k == indices1[m][1] || k == indices1[m][2])
                                            {
                                                repeated = 1;
                                                break;
                                            }
                                        }
                                    }
                                    if (j == indices1[m][0])
                                    {
                                        if (i == indices1[m][1] || i == indices1[m][2])
                                        {
                                            if (k == indices1[m][1] || k == indices1[m][2])
                                            {
                                                repeated = 1;
                                                break;
                                            }
                                        }
                                    }
                                    if (k == indices1[m][0])
                                    {
                                        if (j == indices1[m][1] || j == indices1[m][2])
                                        {
                                            if (i == indices1[m][1] || i == indices1[m][2])
                                            {
                                                repeated = 1;
                                                break;
                                            }
                                        }
                                    }
                                    if (m == 20 * 4 - 1)
                                    {
                                        repeated = 0;
                                    }
                                }
                                if (repeated == 0)
                                {
                                    indices1[l][0] = i;
                                    indices1[l][1] = j;
                                    indices1[l][2] = k;

                                    l++;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Normalizing vertices
        for (int i = 0; i < 42; i++)
        {
            float norm = std::sqrt(std::pow(vertices4[i][0], 2) + std::pow(vertices4[i][1], 2) + std::pow(vertices4[i][2], 2));

            for (int j = 0; j < 3; j++)
            {
                vertices4[i][j] /= norm;
            }
        }

        for (int i = 0; i < 42; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                vertices4[i][j] *= d;
            }
        }
    }
};
*/
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