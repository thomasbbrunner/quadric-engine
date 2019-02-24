#ifndef GEOMETRY3D_H
#define GEOMETRY3D_H

#include <common.h>
#include <shader.h>
#include <camera.h>
#include <light.h>
#include <geometry.h>

/*** 3D GEOMETRY ***/

class Cube : public Geometry
{
  public:
    Cube(float size = 0.5f)
    {
        std::vector<glm::vec3> vertex_coordinates_mesh{
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

        std::vector<unsigned int> vertex_indices_mesh{
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

        Mesh mesh(vertex_coordinates_mesh, vertex_indices_mesh);

        float vertices_contour[] = {
            size / 2, size / 2, size / 2,
            -size / 2, size / 2, size / 2,
            -size / 2, -size / 2, size / 2,
            size / 2, -size / 2, size / 2,
            size / 2, size / 2, -size / 2,
            -size / 2, size / 2, -size / 2,
            -size / 2, -size / 2, -size / 2,
            size / 2, -size / 2, -size / 2};

        unsigned int indices_contour[] = {
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

        numelements_contour = sizeof(indices_contour) / sizeof(indices_contour[0]);

        // Contour
        glGenVertexArrays(1, &VAO_contour);

        glBindVertexArray(VAO_contour);

        glGenBuffers(1, &VBO_contour);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_contour);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_contour), vertices_contour, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO_contour);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_contour);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_contour), indices_contour, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        /*** Mesh ***/
        numelements_mesh = 24 * 3; //sizeof(indices_fill) / sizeof(indices_fill[0]);

        glGenVertexArrays(1, &VAO_mesh);
        glBindVertexArray(VAO_mesh);

        glGenBuffers(1, &VBO_mesh);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_mesh);
        glBufferData(GL_ARRAY_BUFFER, mesh.get_raw_vertex_data_size(), mesh.get_raw_vertex_data(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO_mesh);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.get_raw_vertex_indices_size(), mesh.get_raw_vertex_indices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
};

class Icosahedron
{
  public:
    unsigned int VBO;
    unsigned int EBO;
    unsigned int VAO;

    unsigned int numelements;

    glm::mat4 model;
    glm::vec4 color;

    // Constructor
    Icosahedron(float d = 1.0f, glm::vec3 coords = glm::vec3(0.0f, 0.0f, 0.0f))
    {
        model = glm::mat4(1.0f);

        float t = (1.0f + std::sqrt(5.0f)) / 2.0f;
        float a = 1.0f / (std::sqrt(1.0f + t * t));

        float vertices[12 * 3] =
            {a * t, a, 0,
             -a * t, a, 0,
             a * t, -a, 0,
             -a * t, -a, 0,
             a, 0, a * t,
             a, 0, -a * t,
             -a, 0, a * t,
             -a, 0, -a * t,
             0, a * t, a,
             0, -a * t, a,
             0, a * t, -a,
             0, -a * t, -a};

        for (int i = 0; i < 12 * 3; i++)
        {
            vertices[i] *= d;
        }

        for (int i = 0; i < 12 * 3; i += 3)
        {
            vertices[i] += coords.x;
            vertices[i + 1] += coords.y;
            vertices[i + 2] += coords.z;
        }

        unsigned int indices[20 * 3] =
            {0, 8, 4,
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

        numelements = sizeof(indices) / sizeof(indices[0]);
        // numelements = 16*3;

        // Vertex array object
        glGenVertexArrays(1, &VAO);
        // InitialisationchangeValue
        // 1. bind Vertex Array Object
        glBindVertexArray(VAO);
        // 2. copy our vertices array in a vertex buffer for OpenGL to use
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 3. copy our index array in a element buffer for OpenGL to use
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // 4. then set the vertex attributes pointers
        // Specifying how OpenGL should interpret the vertex values
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }

    void draw_wireframe()
    {
        // Bind buffer
        glBindVertexArray(VAO);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wirerame mode
        glDrawElements(GL_TRIANGLES, numelements, GL_UNSIGNED_INT, 0);
    }

    void draw_fill()
    {
        // Bind buffer
        glBindVertexArray(VAO);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill mode
        glDrawElements(GL_TRIANGLES, numelements, GL_UNSIGNED_INT, 0);
    }

    void setmodel(glm::mat4 modeltemp)
    {
        model = modeltemp;
    }
};

class Sphere : public Geometry
{
  public:
    Sphere(float d = 1.0f, glm::vec3 coords = glm::vec3(0.0f, 0.0f, 0.0f))
    {
        float t = (1.0f + std::sqrt(5.0f)) / 2.0f;
        float a = 1.0f / (std::sqrt(1.0f + t * t));

        float vertices0[12][3] =
            {{a * t, a, 0},
             {-a * t, a, 0},
             {a * t, -a, 0},
             {-a * t, -a, 0},
             {a, 0, a * t},
             {a, 0, -a * t},
             {-a, 0, a * t},
             {-a, 0, -a * t},
             {0, a * t, a},
             {0, -a * t, a},
             {0, a * t, -a},
             {0, -a * t, -a}};

        unsigned int indices0[20 * 3] =
            {0, 8, 4,
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
            float norm = std::sqrt(std::pow(vertices0[i][0], 2) + std::pow(vertices0[i][1], 2) + std::pow(vertices0[i][2], 2));

            for (int j = 0; j < 3; j++)
            {
                vertices0[i][j] /= norm;
            }
        }

        // Creating new vertices
        float vertices2[20 * 3][3] = {{0.0f}};
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

        for (int i = 0; i < 42; i++)
        {
            vertices4[i][0] += coords.x;
            vertices4[i][1] += coords.y;
            vertices4[i][2] += coords.z;
        }

        numelements_mesh = sizeof(indices1) / sizeof(indices1[0][0]);
        numelements_contour = numelements_mesh;

        // Mesh
        glGenVertexArrays(1, &VAO_mesh);

        glBindVertexArray(VAO_mesh);

        glGenBuffers(1, &VBO_mesh);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_mesh);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4), vertices4, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO_mesh);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // contour
        VAO_contour = VAO_mesh;
    }
};

class Plane : public Geometry
{
  public:
    unsigned int VAO_dots = 0;
    unsigned int EBO_dots = 0;
    unsigned int VBO_texture = 0;
    unsigned int numelements_dots = 0;

    Plane(int dotres, int dx, int dz)
    {
        /*** MESH ***/

        // Generating vertex coordinates arranged in 2D format
        std::vector<std::vector<glm::vec3>> coords = generate_vertices(dotres, dx, dz);

        // Print::array(coords);

        // Transforming vertex coordinates to 1D format
        std::vector<glm::vec3> vertex_coordinates;
        for (unsigned int i = 0; i < coords.size(); i++)
        {
            for (unsigned int j = 0; j < coords.at(0).size(); j++)
            {
                // Vertex coordinates
                vertex_coordinates.push_back(coords.at(i).at(j));
            }
        }

        // Generating indices for mesh
        std::vector<unsigned int> vertex_indices_mesh;
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

        Mesh mesh(vertex_coordinates, vertex_indices_mesh);

        numelements_mesh = vertex_indices_mesh.size();

        /*** DOTS ***/

        // Generating indices for dots
        std::vector<unsigned int> indices_dots(vertex_coordinates.size());

        for (unsigned int i = 0; i < indices_dots.size(); i++)
        {
            indices_dots.at(i) = i;
        }

        numelements_dots = indices_dots.size();

        /*** MESH ***/
        glGenVertexArrays(1, &VAO_mesh);
        glBindVertexArray(VAO_mesh);

        glGenBuffers(1, &VBO_mesh);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_mesh);
        glBufferData(GL_ARRAY_BUFFER, mesh.get_raw_vertex_data_size(), mesh.get_raw_vertex_data(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO_mesh);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.get_raw_vertex_indices_size(), mesh.get_raw_vertex_indices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture
        glGenBuffers(1, &VBO_texture);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
        glBufferData(GL_ARRAY_BUFFER, mesh.get_raw_vertex_coordinates_size(), mesh.get_raw_vertex_coordinates(), GL_STATIC_DRAW);
        glGenTextures(1, &texture_vertex_data);
        glBindTexture(GL_TEXTURE_BUFFER, texture_vertex_data);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, VBO_texture);

        /*** DOTS ***/
        glGenVertexArrays(1, &VAO_dots);
        glBindVertexArray(VAO_dots);

        // glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_mesh);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.at(0)) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO_dots);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_dots);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_dots.at(0)) * indices_dots.size(), indices_dots.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }

    void draw_dots()
    {
        glBindVertexArray(VAO_dots);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_POINTS, numelements_dots, GL_UNSIGNED_INT, 0);
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