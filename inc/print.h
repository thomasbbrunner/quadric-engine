#ifndef PRINT_H
#define PRINT_H

#include <common.h>

class Print
{
  public:
    static void mat4(glm::mat4 mat)
    {
        for (int i = 0; i < 4; i++)
        {
            printf("\t");
            for (int j = 0; j < 4; j++)
            {
                printf("%4.3f\t", mat[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    static void vec4(glm::vec4 vec)
    {
        std::cout << "\t" << vec.x << "\n"
                  << "\t" << vec.y << "\n"
                  << "\t" << vec.z << "\n"
                  << "\t" << vec.w << std::endl;
    }

    static void vec3(glm::vec3 vec)
    {
        std::cout << "\t" << vec.x << "\n"
                  << "\t" << vec.y << "\n"
                  << "\t" << vec.z << std::endl;
    }

    static void array(float *arr, int dim1, int dim2 = 1)
    {
        for (int i = 0; i < dim1; i++)
        {
            for (int j = 0; j < dim2; j++)
            {
                printf("%.4f ", arr[i * dim2 + j]);
            }
        }
        printf("\n");
    }

    static void array(unsigned int *arr, int dim1, int dim2 = 1)
    {
        for (int i = 0; i < dim1; i++)
        {
            for (int j = 0; j < dim2; j++)
            {
                printf("%u ", arr[i * dim2 + j]);
            }
        }
        printf("\n");
    }

    static void array(int *arr, int dim1, int dim2 = 1)
    {
        for (int i = 0; i < dim1; i++)
        {
            for (int j = 0; j < dim2; j++)
            {
                printf("%u ", arr[i * dim2 + j]);
            }
        }
        printf("\n");
    }

    static void array(glm::vec3 *arr, int dim1, int dim2 = 1)
    {
        for (int i = 0; i < dim1; i++)
        {
            for (int j = 0; j < dim2; j++)
            {
                printf("%.4f ", arr[i * dim2 + j].x);
                printf("%.4f ", arr[i * dim2 + j].y);
                printf("%.4f ", arr[i * dim2 + j].z);
                printf("\n");
            }
            printf("\n");
        }
    }

    static void array(glm::vec3 **arr, int dim1, int dim2 = 1)
    {
        for (int i = 0; i < dim1; i++)
        {
            for (int j = 0; j < dim2; j++)
            {
                printf("%.4f ", arr[i][j].x);
                printf("%.4f ", arr[i][j].y);
                printf("%.4f ", arr[i][j].z);
                printf("\n");
            }
            printf("\n");
        }
    }

    static void array(std::vector<std::vector<glm::vec3>> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            for (unsigned int j = 0; j < arr.at(i).size(); j++)
            {
                printf("%.4f ", arr.at(i).at(j).x);
                printf("%.4f ", arr.at(i).at(j).y);
                printf("%.4f ", arr.at(i).at(j).z);
                printf("\n");
            }
            printf("\n");
        }
    }

    static void array(std::vector<std::vector<int>> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            for (unsigned int j = 0; j < arr.at(i).size(); j++)
            {
                printf("%i ", arr.at(i).at(j));
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    }

    static void array(std::vector<std::vector<char>> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            for (unsigned int j = 0; j < arr.at(i).size(); j++)
            {
                printf("%u ", arr.at(i).at(j));
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    }

    static void array(std::vector<glm::vec3> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            printf("%.4f ", arr.at(i).x);
            printf("%.4f ", arr.at(i).y);
            printf("%.4f ", arr.at(i).z);
            printf("\n");
        }
        printf("\n");
    }

    static void array(std::vector<unsigned int> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            printf("%u ", arr.at(i));
            printf("\n");
        }
        printf("\n");
    }

    static void array(std::vector<float> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            printf("%.4f ", arr.at(i));
            printf("\n");
        }
        printf("\n");
    }

    // static void print_vertices(std::vector<Vertex> vertices)
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

    // static void print_polygons(std::vector<Polygon> polygons);
    // {
    //     printf("Polygon data:\n");
    //     for (unsigned int i = 0; i < polygons.size(); i++)
    //     {
    //         printf("%u\t%u\t%u\t%u\n",
    //                polygons.at(i).polygon_id,
    //                polygons.at(i).vertex_id.at(0), polygons.at(i).vertex_id.at(1), polygons.at(i).vertex_id.at(2));
    //     }
    // }
};

#endif