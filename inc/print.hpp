
#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace quad
{
namespace print
{

void info(std::string message);

void debug(std::string message);

void warning(std::string message);

void error(std::string message,
           std::string function_name = "",
           std::string details = "");

// Arrays ------------------------------------------------------------------

void mat(glm::mat4 mat);

void vec(glm::vec4 vec);

void vec(glm::vec3 vec);

void array(float *arr, int dim1, int dim2 = 1);

void array(unsigned int *arr, int dim1, int dim2 = 1);

void array(int *arr, int dim1, int dim2 = 1);

void array(glm::vec3 *arr, int dim1, int dim2 = 1);

void array(glm::vec3 **arr, int dim1, int dim2 = 1);

// Vectors 1D

void array(std::vector<glm::vec3> arr);

void array(std::vector<unsigned int> arr);

void array(std::vector<unsigned int> arr, unsigned int force_column);

void array(std::vector<float> arr);

// Vectors 2D

void array(std::vector<std::vector<glm::vec3>> arr);

void array(std::vector<std::vector<int>> arr);

void array(std::vector<std::vector<char>> arr);

// Vectors 3D

//   void print_vertices(std::vector<Vertex> vertices)
// {
//     printf("Vertex data:\n");
//     for (unsigned int i = 0; i < vertices.size(); i++)
//     {
//         printf("%u", vertices.at(i).vertex_id);

//         printf("\tCoordinates:\t%.3f\t%.3f\t%.3f\n",
//                vertices.at(i).coordinates.x, vertices.at(i).coordinates.y, vertices.at(i).coordinates.z);

//         printf("\tNormal:\t\t%.3f\t%.3f\t%.3f\n",
//                vertices.at(i).normal.x, vertices.at(i).normal.y, vertices.at(i).normal.z);

//         printf("\tPolygon ID:\t");
//         for (unsigned int j = 0; j < vertices.at(i).polygon_id.size(); j++)
//         {
//             printf("%u\t", vertices.at(i).polygon_id.at(j));
//         }
//         printf("\n\tAdjc vert ID:\t");
//         for (unsigned int j = 0; j < vertices.at(i).adjacent_vertex_id.size(); j++)
//         {
//             printf("%u\t", vertices.at(i).adjacent_vertex_id.at(j));
//         }
//         printf("\n\tAdjc vert seq:\t");
//         for (unsigned int j = 0; j < vertices.at(i).adjacent_vertex_sequence.size(); j++)
//         {
//             printf("%u\t", vertices.at(i).adjacent_vertex_sequence.at(j));
//         }
//         printf("\n");
//     }
// }

//   void print_polygons(std::vector<Polygon> polygons);
// {
//     printf("Polygon data:\n");
//     for (unsigned int i = 0; i < polygons.size(); i++)
//     {
//         printf("%u\t%u\t%u\t%u\n",
//                polygons.at(i).polygon_id,
//                polygons.at(i).vertex_id.at(0), polygons.at(i).vertex_id.at(1), polygons.at(i).vertex_id.at(2));
//     }
// }
} // namespace print
} // namespace quad
