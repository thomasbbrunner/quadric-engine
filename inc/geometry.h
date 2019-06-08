#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <stdio.h>

/*** PARENT CLASSES ***/

class Geometry
{
  public:
    std::vector<glm::vec3> vertex_coordinates_mesh;
    std::vector<glm::vec3> vertex_coordinates_contour;
    std::vector<glm::vec3> vertex_coordinates_dots;
    std::vector<unsigned int> vertex_indices_mesh;
    std::vector<unsigned int> vertex_indices_contour;

    void apply_transformation(glm::mat4 transformation)
    {
        for (unsigned int i = 0; i < vertex_coordinates_mesh.size(); i++)
        {
            vertex_coordinates_mesh.at(i) = glm::vec3(transformation * glm::vec4(vertex_coordinates_mesh.at(i), 1.0));
        }
        for (unsigned int i = 0; i < vertex_coordinates_contour.size(); i++)
        {
            vertex_coordinates_contour.at(i) = glm::vec3(transformation * glm::vec4(vertex_coordinates_contour.at(i), 1.0));
        }
        for (unsigned int i = 0; i < vertex_coordinates_dots.size(); i++)
        {
            vertex_coordinates_dots.at(i) = glm::vec3(transformation * glm::vec4(vertex_coordinates_dots.at(i), 1.0));
        }
    }
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

class Mesh : public Model
{
  public:
    Mesh(std::vector<glm::vec3> vertex_data, std::vector<unsigned int> index_data)
    {
        if (vertex_data.size() == 0 || index_data.size() == 0)
        {
            return;
        }

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
            vertices.push_back(Vertex());
            vertices.back().vertex_id = i;
            vertices.back().coordinates = vertex_data.at(i);
        }

        // Creating polygons array
        for (unsigned int i = 0; i < index_data.size(); i += 3)
        {
            polygons.push_back(Polygon());
            polygons.back().polygon_id = polygons.size() - 1;
            polygons.back().vertex_id = std::vector<unsigned int>{index_data.at(i), index_data.at(i + 1), index_data.at(i + 2)};

            // Adding polygon information to vertices array
            vertices.at(polygons.back().vertex_id.at(0)).polygon_id.push_back(polygons.back().polygon_id);
            vertices.at(polygons.back().vertex_id.at(1)).polygon_id.push_back(polygons.back().polygon_id);
            vertices.at(polygons.back().vertex_id.at(2)).polygon_id.push_back(polygons.back().polygon_id);
        }

        // Gathering adjacent vertices
        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through main vertices
        {
            for (unsigned int j = 0; j < vertices.at(i).polygon_id.size(); j++)
            // Iterating through the polygons that contain the main vertex
            {
                // Iterating through vertices inside polygons that contain the main vertex
                // with algorithm for keeping correct polygon positive direction

                if (polygons.at(vertices.at(i).polygon_id.at(j)).vertex_id.at(0) == vertices.at(i).vertex_id)
                // If main vertex is the first in the polygon
                {
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(1));
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(2));
                }

                if (polygons.at(vertices.at(i).polygon_id.at(j)).vertex_id.at(1) == vertices.at(i).vertex_id)
                // If main vertex is the second in the polygon
                {
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(2));
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(0));
                }

                if (polygons.at(vertices.at(i).polygon_id.at(j)).vertex_id.at(2) == vertices.at(i).vertex_id)
                // If main vertex is the third in the polygon
                {
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(0));
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).polygon_id.at(j))
                            .vertex_id.at(1));
                }
            }

            std::vector<unsigned int> temp_adjacent_vertex_id = vertices.at(i).adjacent_vertex_id;

            // Sorting and removing duplicates
            std::sort(vertices.at(i).adjacent_vertex_id.begin(), vertices.at(i).adjacent_vertex_id.end());
            vertices.at(i).adjacent_vertex_id.erase(std::unique(vertices.at(i).adjacent_vertex_id.begin(), vertices.at(i).adjacent_vertex_id.end()), vertices.at(i).adjacent_vertex_id.end());

            // Creating sequence for cross product
            for (unsigned int j = 0; j < temp_adjacent_vertex_id.size(); j++)
            {
                char index = std::find(vertices.at(i).adjacent_vertex_id.begin(), vertices.at(i).adjacent_vertex_id.end(), temp_adjacent_vertex_id.at(j)) - vertices.at(i).adjacent_vertex_id.begin();
                vertices.at(i).adjacent_vertex_sequence.push_back(index);
            }

            // Getting maximun number of adjacent vertices to save space when creating buffer object
            max_number_adjacent_vertex = vertices.at(i).adjacent_vertex_id.size() > max_number_adjacent_vertex ? vertices.at(i).adjacent_vertex_id.size() : max_number_adjacent_vertex;
        }

        // Calculating static normals
        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through main vertices
        {
            for (unsigned int j = 0; j < vertices.at(i).adjacent_vertex_sequence.size() - 1; j += 2)
            // Iterating through adjacent vertices
            {
                if (vertices.at(i).adjacent_vertex_sequence.size() == 12 && j == 11)
                // Check if main vertex is surrounded on all sides by other vertices
                // Corresponds to special case
                {
                    vertices.at(i).normal += glm::cross(
                        vertices.at(vertices.at(i).adjacent_vertex_id.at(vertices.at(i).adjacent_vertex_sequence.at(j))).coordinates - vertices.at(i).coordinates,
                        vertices.at(vertices.at(i).adjacent_vertex_id.at(vertices.at(i).adjacent_vertex_sequence.at(0))).coordinates - vertices.at(i).coordinates);
                }
                else
                {
                    vertices.at(i).normal += glm::cross(
                        vertices.at(vertices.at(i).adjacent_vertex_id.at(vertices.at(i).adjacent_vertex_sequence.at(j))).coordinates - vertices.at(i).coordinates,
                        vertices.at(vertices.at(i).adjacent_vertex_id.at(vertices.at(i).adjacent_vertex_sequence.at(j + 1))).coordinates - vertices.at(i).coordinates);
                }
            }
            vertices.at(i).normal = glm::normalize(vertices.at(i).normal);
        }

        // Printing for debug
        // Print::print_polygons(polygons);
        // Print::print_vertices(vertices);
    }
};

class Contour : public Model
{
  public:
    Contour(std::vector<glm::vec3> vertex_data, std::vector<unsigned int> index_data)
    {
        for (unsigned int i = 0; i < vertex_data.size(); i++)
        {
            vertices.push_back(Vertex());
            vertices.back().coordinates = vertex_data.at(i);
        }

        for (unsigned int i = 0; i < index_data.size(); i += 2)
        {
            polygons.push_back(Polygon());
            polygons.back().polygon_id = i;
            polygons.back().vertex_id.push_back(index_data.at(i));
            polygons.back().vertex_id.push_back(index_data.at(i + 1));
        }

        // Print::array(vertex_data);
        // Print::array(index_data);
    }
};

class Dots : public Model
{
  public:
    Dots(std::vector<glm::vec3> vertex_data)
    {
        for (unsigned int i = 0; i < vertex_data.size(); i++)
        {
            vertices.push_back(Vertex());
            vertices.back().coordinates = vertex_data.at(i);
        }

        for (unsigned int i = 0; i < vertex_data.size(); i++)
        {
            polygons.push_back(Polygon());
            polygons.back().polygon_id = i;
            polygons.back().vertex_id.push_back(i);
        }
    }
};
