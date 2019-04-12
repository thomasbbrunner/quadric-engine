#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <common.h>

/*** PARENT CLASSES ***/

class Geometry
{
  public:
    std::vector<glm::vec3> vertex_coordinates_mesh;
    std::vector<glm::vec3> vertex_coordinates_contour;
    std::vector<glm::vec3> vertex_coordinates_dots;
    std::vector<unsigned int> vertex_indices_mesh;
	std::vector<unsigned int> vertex_indices_contour;
};

class Vertex
{
  public:
    unsigned int vertex_id;                       // ID of vertex
    glm::vec3 coordinates;                        // Positional vector of this vertex
    std::vector<unsigned int> polygon_id;         // ID of polygons that contain this vertex
    std::vector<unsigned int> adjacent_vertex_id; // Unique ID of vertices that are adjacent to this vertex
    std::vector<char> adjacent_vertex_sequence;   // Sequence of adjacent vertices for cross product operation
    glm::vec3 normal;                             // Directional vector of vertex's normal
};

class Polygon
{
  public:
    unsigned int polygon_id;             // ID of polygon element
    std::vector<unsigned int> vertex_id; // ID of vertices inside this polygon
};

class Model
{
  public:
    // Structured data
    std::vector<Polygon> polygons;               // Vector with all polygon data from mesh
    std::vector<Vertex> vertices;                // Vector with all vertex data from mesh
    unsigned int max_number_adjacent_vertex = 0; // Maximum number of adjacent vertices found in a mesh
};

class Mesh
{
  public:
    Model model;

    void append(std::vector<glm::vec3> vertex_data, std::vector<unsigned int> index_data)
    {
        Model model_created = create(vertex_data, index_data);

        // Append to model
        model.polygons.insert(model.polygons.end(), model_created.polygons.begin(), model_created.polygons.end());
        model.vertices.insert(model.vertices.end(), model_created.vertices.begin(), model_created.vertices.end());
    }

    Model create(std::vector<glm::vec3> vertex_data, std::vector<unsigned int> index_data)
    {
        Model model_create;

        // Testing for input plausibility
        // Check if vertex_data is big enough for the maximun index value
        if ((unsigned int)*std::max_element(index_data.begin(), index_data.end()) != vertex_data.size() - 1)
        {
            printf("ERROR at class Mesh constructor: check input array consistency\n");
            exit(0);
        }

        // Creating vertices array
        for (unsigned int i = 0; i < vertex_data.size(); i++)
        {
            // Vertices array
            model_create.vertices.push_back(Vertex());
            model_create.vertices.back().vertex_id = i;
            model_create.vertices.back().coordinates = vertex_data.at(i);
        }

        // Creating polygons array
        for (unsigned int i = 0; i < index_data.size(); i += 3)
        {
            model_create.polygons.push_back(Polygon());
            model_create.polygons.back().polygon_id = model_create.polygons.size() - 1;
            model_create.polygons.back().vertex_id = std::vector<unsigned int>{index_data.at(i), index_data.at(i + 1), index_data.at(i + 2)};

            // Adding polygon information to vertices array
            model_create.vertices.at(model_create.polygons.back().vertex_id.at(0)).polygon_id.push_back(model_create.polygons.back().polygon_id);
            model_create.vertices.at(model_create.polygons.back().vertex_id.at(1)).polygon_id.push_back(model_create.polygons.back().polygon_id);
            model_create.vertices.at(model_create.polygons.back().vertex_id.at(2)).polygon_id.push_back(model_create.polygons.back().polygon_id);
        }

        // Gathering adjacent vertices
        for (unsigned int i = 0; i < model_create.vertices.size(); i++)
        // Iterating through main vertices
        {
            for (unsigned int j = 0; j < model_create.vertices.at(i).polygon_id.size(); j++)
            // Iterating through the polygons that contain the main vertex
            {
                // Iterating through vertices inside polygons that contain the main vertex
                // with algorithm for keeping correct polygon positive direction

                if (model_create.polygons.at(model_create.vertices.at(i).polygon_id.at(j)).vertex_id.at(0) == model_create.vertices.at(i).vertex_id)
                // If main vertex is the first in the polygon
                {
                    model_create.vertices.at(i).adjacent_vertex_id.push_back(
                        model_create.polygons.at(model_create.vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(1));
                    model_create.vertices.at(i).adjacent_vertex_id.push_back(
                        model_create.polygons.at(model_create.vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(2));
                }

                if (model_create.polygons.at(model_create.vertices.at(i).polygon_id.at(j)).vertex_id.at(1) == model_create.vertices.at(i).vertex_id)
                // If main vertex is the second in the polygon
                {
                    model_create.vertices.at(i).adjacent_vertex_id.push_back(
                        model_create.polygons.at(model_create.vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(2));
                    model_create.vertices.at(i).adjacent_vertex_id.push_back(
                        model_create.polygons.at(model_create.vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(0));
                }

                if (model_create.polygons.at(model_create.vertices.at(i).polygon_id.at(j)).vertex_id.at(2) == model_create.vertices.at(i).vertex_id)
                // If main vertex is the third in the polygon
                {
                    model_create.vertices.at(i).adjacent_vertex_id.push_back(
                        model_create.polygons.at(model_create.vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(0));
                    model_create.vertices.at(i).adjacent_vertex_id.push_back(
                        model_create.polygons.at(model_create.vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(1));
                }
            }

            std::vector<unsigned int> temp_adjacent_vertex_id = model_create.vertices.at(i).adjacent_vertex_id;

            // Sorting and removing duplicates
            std::sort(model_create.vertices.at(i).adjacent_vertex_id.begin(), model_create.vertices.at(i).adjacent_vertex_id.end());
            model_create.vertices.at(i).adjacent_vertex_id.erase(std::unique(model_create.vertices.at(i).adjacent_vertex_id.begin(), model_create.vertices.at(i).adjacent_vertex_id.end()), model_create.vertices.at(i).adjacent_vertex_id.end());

            // Creating sequence for cross product
            for (unsigned int j = 0; j < temp_adjacent_vertex_id.size(); j++)
            {
                char index = std::find(model_create.vertices.at(i).adjacent_vertex_id.begin(), model_create.vertices.at(i).adjacent_vertex_id.end(), temp_adjacent_vertex_id.at(j)) - model_create.vertices.at(i).adjacent_vertex_id.begin();
                model_create.vertices.at(i).adjacent_vertex_sequence.push_back(index);
            }

            // Getting maximun number of adjacent vertices to save space when creating buffer object
            model_create.max_number_adjacent_vertex = model_create.vertices.at(i).adjacent_vertex_id.size() > model_create.max_number_adjacent_vertex ? model_create.vertices.at(i).adjacent_vertex_id.size() : model_create.max_number_adjacent_vertex;
        }

        // Calculating static normals
        for (unsigned int i = 0; i < model_create.vertices.size(); i++)
        // Iterating through main vertices
        {
            for (unsigned int j = 0; j < model_create.vertices.at(i).adjacent_vertex_sequence.size() - 1; j += 2)
            // Iterating through adjacent vertices
            {
                if (model_create.vertices.at(i).adjacent_vertex_sequence.size() == 12 && j == 11)
                // Check if main vertex is surrounded on all sides by other vertices
                // Corresponds to special case
                {
                    model_create.vertices.at(i).normal += glm::cross(
                        model_create.vertices.at(model_create.vertices.at(i).adjacent_vertex_id.at(model_create.vertices.at(i).adjacent_vertex_sequence.at(j))).coordinates - model_create.vertices.at(i).coordinates,
                        model_create.vertices.at(model_create.vertices.at(i).adjacent_vertex_id.at(model_create.vertices.at(i).adjacent_vertex_sequence.at(0))).coordinates - model_create.vertices.at(i).coordinates);
                }
                else
                {
                    model_create.vertices.at(i).normal += glm::cross(
                        model_create.vertices.at(model_create.vertices.at(i).adjacent_vertex_id.at(model_create.vertices.at(i).adjacent_vertex_sequence.at(j))).coordinates - model_create.vertices.at(i).coordinates,
                        model_create.vertices.at(model_create.vertices.at(i).adjacent_vertex_id.at(model_create.vertices.at(i).adjacent_vertex_sequence.at(j + 1))).coordinates - model_create.vertices.at(i).coordinates);
                }
            }
            model_create.vertices.at(i).normal = glm::normalize(model_create.vertices.at(i).normal);
        }

        // Printing for debug
        // Print::print_polygons(model_create.polygons);
        // Print::print_vertices(model_create.vertices);

        return model_create;
    }
};

class Contour
{
  public:
    Model model;

    void append(std::vector<glm::vec3> vertex_data, std::vector<unsigned int> index_data)
    {
        Model model_created = create(vertex_data, index_data);

        // Append to model
        model.polygons.insert(model.polygons.end(), model_created.polygons.begin(), model_created.polygons.end());
        model.vertices.insert(model.vertices.end(), model_created.vertices.begin(), model_created.vertices.end());
    }

    Model create(std::vector<glm::vec3> vertex_data, std::vector<unsigned int> index_data)
    {
        Model model_create;

        for (unsigned int i = 0; i < vertex_data.size(); i++)
        {
            model_create.vertices.push_back(Vertex());
            model_create.vertices.back().coordinates = vertex_data.at(i);
        }

        for (unsigned int i = 0; i < index_data.size(); i += 2)
        {
            model_create.polygons.push_back(Polygon());
            model_create.polygons.back().polygon_id = i;
            model_create.polygons.back().vertex_id.push_back(index_data.at(i));
            model_create.polygons.back().vertex_id.push_back(index_data.at(i + 1));
        }

        // Print::array(vertex_data);
        // Print::array(index_data);

        return model_create;
    }
};

class Dots
{
  public:
    Model model;

    void append(std::vector<glm::vec3> vertex_data)
    {
        Model model_created = create(vertex_data);

        // Append to model
        model.polygons.insert(model.polygons.end(), model_created.polygons.begin(), model_created.polygons.end());
        model.vertices.insert(model.vertices.end(), model_created.vertices.begin(), model_created.vertices.end());
    }

    Model create(std::vector<glm::vec3> vertex_data)
    {
        Model model_create;

        for (unsigned int i = 0; i < vertex_data.size(); i++)
        {
            model_create.vertices.push_back(Vertex());
            model_create.vertices.back().coordinates = vertex_data.at(i);
        }

        for (unsigned int i = 0; i < vertex_data.size(); i++)
        {
            model_create.polygons.push_back(Polygon());
            model_create.polygons.back().polygon_id = i;
            model_create.polygons.back().vertex_id.push_back(i);
        }

        return model_create;
    }
};

#endif