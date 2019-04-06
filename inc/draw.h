#ifndef DRAW_H
#define DRAW_H

#include <geometry.h>

class Draw
{
  public:
    void draw_wireframe()
    {
        update_shader();

        glBindTexture(GL_TEXTURE_BUFFER, texture_vertex_data);

        glBindVertexArray(VAO_mesh);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, numelements_mesh, GL_UNSIGNED_INT, 0);
    }

    void draw_contour()
    {
        update_shader();

        glBindVertexArray(VAO_contour);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_LINES, numelements_contour, GL_UNSIGNED_INT, 0);
    }

    void draw_fill()
    {
        update_shader();

        glBindTexture(GL_TEXTURE_BUFFER, texture_vertex_data);

        glBindVertexArray(VAO_mesh);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, numelements_mesh, GL_UNSIGNED_INT, 0);
    }

    void draw_dots()
    {
        update_shader();

        glBindVertexArray(VAO_dots);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_POINTS, numelements_dots, GL_UNSIGNED_INT, 0);
    }

  private:
    unsigned int VAO_mesh = 0;
    unsigned int VBO_mesh = 0;
    unsigned int EBO_mesh = 0;
    unsigned int numelements_mesh = 0;

    unsigned int VAO_contour = 0;
    unsigned int VBO_contour = 0;
    unsigned int EBO_contour = 0;
    unsigned int numelements_contour = 0;

    unsigned int VAO_dots = 0;
    unsigned int VBO_dots = 0;
    unsigned int EBO_dots = 0;
    unsigned int numelements_dots = 0;

    unsigned int VBO_texture = 0;
    unsigned int texture_vertex_data = 0;
};

class Buffer
{
  public:
    void generate_buffers(Mesh mesh)
    {
        /*** MESH ***/

        numelements_mesh = mesh.get_raw_vertex_indices_size();

        glGenVertexArrays(1, &VAO_mesh);
        glBindVertexArray(VAO_mesh);

        glGenBuffers(1, &VBO_mesh);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_mesh);
        glBufferData(GL_ARRAY_BUFFER,
                     mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes() +
                         mesh.get_raw_vertex_adjacents_size_bytes(0) + mesh.get_raw_vertex_adjacents_size_bytes(1) +
                         mesh.get_raw_vertex_adjacents_sequence_size_bytes(0) + mesh.get_raw_vertex_adjacents_sequence_size_bytes(1) + mesh.get_raw_vertex_adjacents_sequence_size_bytes(2),
                     NULL, GL_STATIC_DRAW);

        // Vertex coordinates
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        mesh.get_raw_vertex_coordinates_size_bytes(), mesh.get_raw_vertex_coordinates());
        // Vertex normals
        glBufferSubData(GL_ARRAY_BUFFER,
                        mesh.get_raw_vertex_coordinates_size_bytes(),
                        mesh.get_raw_vertex_normals_size_bytes(), mesh.get_raw_vertex_normals());
        // Adjacents 0
        glBufferSubData(GL_ARRAY_BUFFER,
                        mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes(),
                        mesh.get_raw_vertex_adjacents_size_bytes(0), mesh.get_raw_vertex_adjacents(0));
        // Adjacents 1
        glBufferSubData(GL_ARRAY_BUFFER,
                        mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes() +
                            mesh.get_raw_vertex_adjacents_size_bytes(0),
                        mesh.get_raw_vertex_adjacents_size_bytes(1), mesh.get_raw_vertex_adjacents(1));
        // Adjacents sequence 0
        glBufferSubData(GL_ARRAY_BUFFER,
                        mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes() +
                            mesh.get_raw_vertex_adjacents_size_bytes(0) + mesh.get_raw_vertex_adjacents_size_bytes(1),
                        mesh.get_raw_vertex_adjacents_sequence_size_bytes(0), mesh.get_raw_vertex_adjacents_sequence(0));
        // Adjacents sequence 1
        glBufferSubData(GL_ARRAY_BUFFER,
                        mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes() +
                            mesh.get_raw_vertex_adjacents_size_bytes(0) + mesh.get_raw_vertex_adjacents_size_bytes(1) +
                            mesh.get_raw_vertex_adjacents_sequence_size_bytes(0),
                        mesh.get_raw_vertex_adjacents_sequence_size_bytes(1), mesh.get_raw_vertex_adjacents_sequence(1));
        // Adjacents sequence 2
        glBufferSubData(GL_ARRAY_BUFFER,
                        mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes() +
                            mesh.get_raw_vertex_adjacents_size_bytes(0) + mesh.get_raw_vertex_adjacents_size_bytes(1) +
                            mesh.get_raw_vertex_adjacents_sequence_size_bytes(0) + mesh.get_raw_vertex_adjacents_sequence_size_bytes(1),
                        mesh.get_raw_vertex_adjacents_sequence_size_bytes(2), mesh.get_raw_vertex_adjacents_sequence(2));

        // Vertex coordinates
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        // Vertex normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)(intptr_t)(mesh.get_raw_vertex_coordinates_size_bytes()));
        glEnableVertexAttribArray(1);

        // Adjacents 0
        glVertexAttribIPointer(2, 3, GL_INT, 3 * sizeof(int),
                               (void *)(intptr_t)(mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes()));
        glEnableVertexAttribArray(2);

        // Adjacents 1
        glVertexAttribIPointer(3, 3, GL_INT, 3 * sizeof(int),
                               (void *)(intptr_t)(mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes() +
                                                  mesh.get_raw_vertex_adjacents_size_bytes(0)));
        glEnableVertexAttribArray(3);

        // Adjacents sequence 0
        glVertexAttribIPointer(4, 4, GL_BYTE, 4 * sizeof(char),
                               (void *)(intptr_t)(mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes() +
                                                  mesh.get_raw_vertex_adjacents_size_bytes(0) + mesh.get_raw_vertex_adjacents_size_bytes(1)));
        glEnableVertexAttribArray(4);

        // Adjacents sequence 1
        glVertexAttribIPointer(5, 4, GL_BYTE, 4 * sizeof(char),
                               (void *)(intptr_t)(mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes() +
                                                  mesh.get_raw_vertex_adjacents_size_bytes(0) + mesh.get_raw_vertex_adjacents_size_bytes(1) +
                                                  mesh.get_raw_vertex_adjacents_sequence_size_bytes(0)));
        glEnableVertexAttribArray(5);

        // Adjacents sequence 2
        glVertexAttribIPointer(6, 4, GL_BYTE, 4 * sizeof(char),
                               (void *)(intptr_t)(mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes() +
                                                  mesh.get_raw_vertex_adjacents_size_bytes(0) + mesh.get_raw_vertex_adjacents_size_bytes(1) +
                                                  mesh.get_raw_vertex_adjacents_sequence_size_bytes(0) + mesh.get_raw_vertex_adjacents_sequence_size_bytes(1)));
        glEnableVertexAttribArray(6);

        glGenBuffers(1, &EBO_mesh);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.get_raw_vertex_indices_size_bytes(), mesh.get_raw_vertex_indices(), GL_STATIC_DRAW);

        glGenBuffers(1, &VBO_texture);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
        glBufferData(GL_ARRAY_BUFFER, mesh.get_raw_vertex_coordinates_size_bytes(), mesh.get_raw_vertex_coordinates(), GL_STATIC_DRAW);

        glGenTextures(1, &texture_vertex_data);
        glBindTexture(GL_TEXTURE_BUFFER, texture_vertex_data);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, VBO_texture);
    }

    void generate_buffers(Contour contour)
    {
        /*** CONTOUR ***/

        numelements_contour = contour.get_raw_vertex_indices_size();

        glGenVertexArrays(1, &VAO_contour);
        glBindVertexArray(VAO_contour);

        glGenBuffers(1, &VBO_contour);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_contour);
        glBufferData(GL_ARRAY_BUFFER, contour.get_raw_vertex_coordinates_size_bytes(), contour.get_raw_vertex_coordinates(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO_contour);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_contour);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, contour.get_raw_vertex_indices_size_bytes(), contour.get_raw_vertex_indices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }

    void generate_buffers(Dots dots)
    {
        /*** DOTS ***/

        numelements_dots = dots.get_raw_vertex_indices_size();

        glGenVertexArrays(1, &VAO_dots);
        glBindVertexArray(VAO_dots);

        if (VBO_mesh == 0)
        {
            glGenBuffers(1, &VBO_dots);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_dots);
            glBufferData(GL_ARRAY_BUFFER, dots.get_raw_vertex_coordinates_size_bytes(), dots.get_raw_vertex_coordinates(), GL_STATIC_DRAW);
        }
        else
        {
            VBO_dots = VBO_mesh;
            glBindBuffer(GL_ARRAY_BUFFER, VBO_dots);
        }

        glGenBuffers(1, &EBO_dots);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_dots);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, dots.get_raw_vertex_indices_size_bytes(), dots.get_raw_vertex_indices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(intptr_t)(dots.get_raw_vertex_coordinates_size_bytes()));
        // glEnableVertexAttribArray(1);
    }

  private:
      // Raw data
    std::vector<float> raw_vertex_coordinates;
    std::vector<float> raw_vertex_normals;
    std::vector<std::vector<int>> raw_vertex_adjacents;
    std::vector<std::vector<char>> raw_vertex_adjacents_sequence;
    std::vector<unsigned int> raw_vertex_indices;

    /*** ACCESS FUNCTIONS ***/

    // Coordinates
    float *get_raw_vertex_coordinates()
    {
        return raw_vertex_coordinates.data();
    }

    unsigned int get_raw_vertex_coordinates_size_bytes()
    {
        return raw_vertex_coordinates.size() * sizeof(float);
    }

    // Normals
    float *get_raw_vertex_normals()
    {
        return raw_vertex_normals.data();
    }

    unsigned int get_raw_vertex_normals_size_bytes()
    {
        return raw_vertex_normals.size() * sizeof(float);
    }

    // Adjacents
    int *get_raw_vertex_adjacents(unsigned int index)
    {
        return raw_vertex_adjacents.at(index).data();
    }

    unsigned int get_raw_vertex_adjacents_size_bytes(unsigned int index)
    {
        return raw_vertex_adjacents.at(index).size() * sizeof(int);
    }

    // Adjacents sequence
    char *get_raw_vertex_adjacents_sequence(unsigned int index)
    {
        return raw_vertex_adjacents_sequence.at(index).data();
    }

    unsigned int get_raw_vertex_adjacents_sequence_size_bytes(unsigned int index)
    {
        return raw_vertex_adjacents_sequence.at(index).size() * sizeof(char);
    }

    // Max adjacents
    unsigned int get_max_number_adjacent_vertex()
    {
        return max_number_adjacent_vertex;
    }

    // Indices
    unsigned int *get_raw_vertex_indices()
    {
        return raw_vertex_indices.data();
    }

    unsigned int get_raw_vertex_indices_size_bytes()
    {
        return raw_vertex_indices.size() * sizeof(unsigned int);
    }

    unsigned int get_raw_vertex_indices_size()
    {
        return raw_vertex_indices.size();
    }

    /*** GENERATE FUNCTIONS ***/

    std::vector<float> generate_raw_vertex_coordinates()
    {
        std::vector<float> vec;

        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through vertices
        {
            for (unsigned int j = 0; j < 3; j++)
            // Iterating through coordinate elements
            {
                vec.push_back(vertices.at(i).coordinates[j]);
            }
        }

        return vec;
    }

    std::vector<float> generate_raw_vertex_normals()
    {
        std::vector<float> vec;

        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through vertices
        {
            for (unsigned int j = 0; j < 3; j++)
            // Iterating through coordinate elements
            {
                vec.push_back(vertices.at(i).normal[j]);
            }
        }

        return vec;
    }

    std::vector<std::vector<int>> generate_raw_vertex_adjacents()
    {
        // Splitting adjacent vertex ids into several vectors
        std::vector<std::vector<int>> vec(2);

        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through vertices
        {
            // fix logic here maybe?
            for (unsigned int j = 0; j < 6; j++)
            // Iterating through coordinate elements
            {
                unsigned int k = j / 3;

                if (j < vertices.at(i).adjacent_vertex_id.size())
                {
                    vec.at(k).push_back(vertices.at(i).adjacent_vertex_id.at(j));
                }
                else
                {
                    vec.at(k).push_back(60000); // TODO change to bigger value
                }
            }
        }

        // Print::array(vec);

        return vec;
    }

    std::vector<std::vector<char>> generate_raw_vertex_adjacents_sequence()
    {
        // Splitting adjacent vertex ids into several vectors
        std::vector<std::vector<char>> vec(3);

        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through vertices
        {
            for (unsigned int j = 0; j < 12; j++)
            // Iterating through coordinate elements
            {
                unsigned int k = j / 4;

                if (j < vertices.at(i).adjacent_vertex_sequence.size())
                {
                    vec.at(k).push_back(vertices.at(i).adjacent_vertex_sequence.at(j));
                }
                else
                {
                    vec.at(k).push_back(255);
                }
            }
        }

        // Print::array(vec);

        return vec;
    }

    std::vector<unsigned int> generate_raw_vertex_indices()
    {
        std::vector<unsigned int> vec;

        for (unsigned int i = 0; i < polygons.size(); i++)
        // Iterating through polygons
        {
            for (unsigned int j = 0; j < polygons.at(i).vertex_id.size(); j++)
            // Iterating through vertex IDs
            {
                vec.push_back(polygons.at(i).vertex_id.at(j));
            }
        }

        return vec;
    }
};

#endif