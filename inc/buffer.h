#ifndef BUFFER_H
#define BUFFER_H

#include <common.h>

class ModelBuffer
{
  public:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int TBO = 0;
    unsigned int VBO_texture = 0;
    unsigned int numelements = 0;
};

class Buffers
{
  public:
    ModelBuffer mesh;
    ModelBuffer contour;
    ModelBuffer dots;
};

class BufferGeneration
{
  public:
    static ModelBuffer generate_from_mesh(Model model)
    {
        ModelBuffer buffer;

        // Generating raw values (C arrays for buffers)
        RawValues raw(model);
        raw.generate_raw_vertex_adjacents();
        raw.generate_raw_vertex_adjacents_sequence();
        raw.generate_raw_vertex_coordinates();
        raw.generate_raw_vertex_indices();
        raw.generate_raw_vertex_normals();

        buffer.numelements = raw.get_raw_vertex_indices_size();

        glGenVertexArrays(1, &buffer.VAO);
        glBindVertexArray(buffer.VAO);

        glGenBuffers(1, &buffer.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes() +
                         raw.get_raw_vertex_adjacents_size_bytes(0) + raw.get_raw_vertex_adjacents_size_bytes(1) +
                         raw.get_raw_vertex_adjacents_sequence_size_bytes(0) + raw.get_raw_vertex_adjacents_sequence_size_bytes(1) + raw.get_raw_vertex_adjacents_sequence_size_bytes(2),
                     NULL, GL_STATIC_DRAW);

        // Vertex coordinates
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        raw.get_raw_vertex_coordinates_size_bytes(), raw.get_raw_vertex_coordinates());
        // Vertex normals
        glBufferSubData(GL_ARRAY_BUFFER,
                        raw.get_raw_vertex_coordinates_size_bytes(),
                        raw.get_raw_vertex_normals_size_bytes(), raw.get_raw_vertex_normals());
        // Adjacents 0
        glBufferSubData(GL_ARRAY_BUFFER,
                        raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes(),
                        raw.get_raw_vertex_adjacents_size_bytes(0), raw.get_raw_vertex_adjacents(0));
        // Adjacents 1
        glBufferSubData(GL_ARRAY_BUFFER,
                        raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes() +
                            raw.get_raw_vertex_adjacents_size_bytes(0),
                        raw.get_raw_vertex_adjacents_size_bytes(1), raw.get_raw_vertex_adjacents(1));
        // Adjacents sequence 0
        glBufferSubData(GL_ARRAY_BUFFER,
                        raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes() +
                            raw.get_raw_vertex_adjacents_size_bytes(0) + raw.get_raw_vertex_adjacents_size_bytes(1),
                        raw.get_raw_vertex_adjacents_sequence_size_bytes(0), raw.get_raw_vertex_adjacents_sequence(0));
        // Adjacents sequence 1
        glBufferSubData(GL_ARRAY_BUFFER,
                        raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes() +
                            raw.get_raw_vertex_adjacents_size_bytes(0) + raw.get_raw_vertex_adjacents_size_bytes(1) +
                            raw.get_raw_vertex_adjacents_sequence_size_bytes(0),
                        raw.get_raw_vertex_adjacents_sequence_size_bytes(1), raw.get_raw_vertex_adjacents_sequence(1));
        // Adjacents sequence 2
        glBufferSubData(GL_ARRAY_BUFFER,
                        raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes() +
                            raw.get_raw_vertex_adjacents_size_bytes(0) + raw.get_raw_vertex_adjacents_size_bytes(1) +
                            raw.get_raw_vertex_adjacents_sequence_size_bytes(0) + raw.get_raw_vertex_adjacents_sequence_size_bytes(1),
                        raw.get_raw_vertex_adjacents_sequence_size_bytes(2), raw.get_raw_vertex_adjacents_sequence(2));

        // Vertex coordinates
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        // Vertex normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)(intptr_t)(raw.get_raw_vertex_coordinates_size_bytes()));
        glEnableVertexAttribArray(1);

        // Adjacents 0
        glVertexAttribIPointer(2, 3, GL_INT, 3 * sizeof(int),
                               (void *)(intptr_t)(raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes()));
        glEnableVertexAttribArray(2);

        // Adjacents 1
        glVertexAttribIPointer(3, 3, GL_INT, 3 * sizeof(int),
                               (void *)(intptr_t)(raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes() +
                                                  raw.get_raw_vertex_adjacents_size_bytes(0)));
        glEnableVertexAttribArray(3);

        // Adjacents sequence 0
        glVertexAttribIPointer(4, 4, GL_BYTE, 4 * sizeof(char),
                               (void *)(intptr_t)(raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes() +
                                                  raw.get_raw_vertex_adjacents_size_bytes(0) + raw.get_raw_vertex_adjacents_size_bytes(1)));
        glEnableVertexAttribArray(4);

        // Adjacents sequence 1
        glVertexAttribIPointer(5, 4, GL_BYTE, 4 * sizeof(char),
                               (void *)(intptr_t)(raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes() +
                                                  raw.get_raw_vertex_adjacents_size_bytes(0) + raw.get_raw_vertex_adjacents_size_bytes(1) +
                                                  raw.get_raw_vertex_adjacents_sequence_size_bytes(0)));
        glEnableVertexAttribArray(5);

        // Adjacents sequence 2
        glVertexAttribIPointer(6, 4, GL_BYTE, 4 * sizeof(char),
                               (void *)(intptr_t)(raw.get_raw_vertex_coordinates_size_bytes() + raw.get_raw_vertex_normals_size_bytes() +
                                                  raw.get_raw_vertex_adjacents_size_bytes(0) + raw.get_raw_vertex_adjacents_size_bytes(1) +
                                                  raw.get_raw_vertex_adjacents_sequence_size_bytes(0) + raw.get_raw_vertex_adjacents_sequence_size_bytes(1)));
        glEnableVertexAttribArray(6);

        glGenBuffers(1, &buffer.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, raw.get_raw_vertex_indices_size_bytes(), raw.get_raw_vertex_indices(), GL_STATIC_DRAW);

        glGenBuffers(1, &buffer.VBO_texture);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO_texture);
        glBufferData(GL_ARRAY_BUFFER, raw.get_raw_vertex_coordinates_size_bytes(), raw.get_raw_vertex_coordinates(), GL_STATIC_DRAW);

        glGenTextures(1, &buffer.TBO);
        glBindTexture(GL_TEXTURE_BUFFER, buffer.TBO);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, buffer.VBO_texture);

        return buffer;
    }

    static ModelBuffer generate_from_contour(Model model)
    {
        ModelBuffer buffer;

        // Generating raw values (C arrays for buffers)
        RawValues raw(model);
        raw.generate_raw_vertex_coordinates();
        raw.generate_raw_vertex_indices();

        buffer.numelements = raw.get_raw_vertex_indices_size();

        glGenVertexArrays(1, &buffer.VAO);
        glBindVertexArray(buffer.VAO);

        glGenBuffers(1, &buffer.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
        glBufferData(GL_ARRAY_BUFFER, raw.get_raw_vertex_coordinates_size_bytes(), raw.get_raw_vertex_coordinates(), GL_STATIC_DRAW);

        glGenBuffers(1, &buffer.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, raw.get_raw_vertex_indices_size_bytes(), raw.get_raw_vertex_indices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        return buffer;
    }

    static ModelBuffer generate_from_dots(Model model)
    {
        ModelBuffer buffer;

        // Generating raw values (C arrays for buffers)
        RawValues raw(model);
        raw.generate_raw_vertex_coordinates();
        raw.generate_raw_vertex_indices();

        buffer.numelements = raw.get_raw_vertex_indices_size();

        glGenVertexArrays(1, &buffer.VAO);
        glBindVertexArray(buffer.VAO);

        glGenBuffers(1, &buffer.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
        glBufferData(GL_ARRAY_BUFFER, raw.get_raw_vertex_coordinates_size_bytes(), raw.get_raw_vertex_coordinates(), GL_STATIC_DRAW);

        glGenBuffers(1, &buffer.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, raw.get_raw_vertex_indices_size_bytes(), raw.get_raw_vertex_indices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(intptr_t)(raw.get_raw_vertex_coordinates_size_bytes()));
        // glEnableVertexAttribArray(1);

        return buffer;
    }
};

#endif