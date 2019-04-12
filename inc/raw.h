#ifndef RAW_H
#define RAW_H

#include <common.h>

class RawValues
{
  public:
    Model model;
    RawValues(Model model) : model(model)
    {
    }

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
        return model.max_number_adjacent_vertex;
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

    void generate_raw_vertex_coordinates()
    {
        std::vector<float> vec;

        for (unsigned int i = 0; i < model.vertices.size(); i++)
        // Iterating through vertices
        {
            for (unsigned int j = 0; j < 3; j++)
            // Iterating through coordinate elements
            {
                vec.push_back(model.vertices.at(i).coordinates[j]);
            }
        }

        raw_vertex_coordinates = vec;
    }

    void generate_raw_vertex_normals()
    {
        std::vector<float> vec;

        for (unsigned int i = 0; i < model.vertices.size(); i++)
        // Iterating through vertices
        {
            for (unsigned int j = 0; j < 3; j++)
            // Iterating through coordinate elements
            {
                vec.push_back(model.vertices.at(i).normal[j]);
            }
        }

        raw_vertex_normals = vec;
    }

    void generate_raw_vertex_adjacents()
    {
        // Splitting adjacent vertex ids into several vectors
        std::vector<std::vector<int>> vec(2);

        for (unsigned int i = 0; i < model.vertices.size(); i++)
        // Iterating through vertices
        {
            // fix logic here maybe?
            for (unsigned int j = 0; j < 6; j++)
            // Iterating through coordinate elements
            {
                unsigned int k = j / 3;

                if (j < model.vertices.at(i).adjacent_vertex_id.size())
                {
                    vec.at(k).push_back(model.vertices.at(i).adjacent_vertex_id.at(j));
                }
                else
                {
                    vec.at(k).push_back(60000); // TODO change to bigger value
                }
            }
        }

        // Print::array(vec);

        raw_vertex_adjacents = vec;
    }

    void generate_raw_vertex_adjacents_sequence()
    {
        // Splitting adjacent vertex ids into several vectors
        std::vector<std::vector<char>> vec(3);

        for (unsigned int i = 0; i < model.vertices.size(); i++)
        // Iterating through vertices
        {
            for (unsigned int j = 0; j < 12; j++)
            // Iterating through coordinate elements
            {
                unsigned int k = j / 4;

                if (j < model.vertices.at(i).adjacent_vertex_sequence.size())
                {
                    vec.at(k).push_back(model.vertices.at(i).adjacent_vertex_sequence.at(j));
                }
                else
                {
                    vec.at(k).push_back(255);
                }
            }
        }

        // Print::array(vec);

        raw_vertex_adjacents_sequence = vec;
    }

    void generate_raw_vertex_indices()
    {
        std::vector<unsigned int> vec;

        for (unsigned int i = 0; i < model.polygons.size(); i++)
        // Iterating through polygons
        {
            for (unsigned int j = 0; j < model.polygons.at(i).vertex_id.size(); j++)
            // Iterating through vertex IDs
            {
                vec.push_back(model.polygons.at(i).vertex_id.at(j));
            }
        }

        raw_vertex_indices = vec;
    }
};

#endif