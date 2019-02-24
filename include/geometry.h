#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <common.h>
#include <shader.h>
#include <camera.h>
#include <light.h>

/*** PARENT CLASSES ***/

class Geometry
{
  public:
    unsigned int VBO_mesh = 0;
    unsigned int EBO_mesh = 0;
    unsigned int VAO_mesh = 0;

    unsigned int VBO_contour = 0;
    unsigned int EBO_contour = 0;
    unsigned int VAO_contour = 0;

    unsigned int texture_vertex_data = 0;

    unsigned int numelements_mesh = 0;
    unsigned int numelements_contour = 0;

    glm::mat4 model = glm::mat4(0.0f);
    glm::vec4 color = glm::vec4(0.0f);

    Shader shader;

    int light_ID = -1;

    void draw_wireframe()
    {
        update_shader();

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

        // glBindTexture(GL_TEXTURE_BUFFER, texture_vertex_data);

        glBindVertexArray(VAO_mesh);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, numelements_mesh, GL_UNSIGNED_INT, 0);
    }

    void set_model(glm::mat4 model_temp)
    {
        model = model_temp;

        // update light (if applicable)
        if (light_ID != -1)
            lighting.set_position(light_ID, glm::vec3(model_temp[3][0], model_temp[3][1], model_temp[3][2]));
    }

    void set_color(glm::vec4 color_temp)
    {
        color = color_temp;

        // update light (if applicable)
        if (light_ID != -1)
            lighting.set_color(light_ID, color_temp);
    }

    void set_shader(Shader shader_in)
    {
        shader = shader_in;
    }

    void set_light_source()
    {
        light_ID = lighting.add_source(1);
    }

    void update_shader()
    {
        shader.set_mat4("model", model);
        shader.set_vec4("color", color);
        shader.set_mat4("view", camera.get_view());
        shader.set_mat4("aspect", camera.get_aspect());
        shader.set_mat4("proj", camera.get_proj());

        shader.update_time(tetra_time.get());

        // send light info to shader
        if (light_ID == -1)
            lighting.update_shader(shader);

        shader.use();
    }
};

class Vertex
{
  public:
    unsigned int vertex_id;                       // ID of vertex
    glm::vec3 coordinates;                        // Positional vector of this vertex
    std::vector<unsigned int> face_id;            // ID of faces that contain this vertex
    std::vector<unsigned int> adjacent_vertex_id; // ID os vertices that are adjacent to this vertex
    glm::vec3 normal;                             // Directional vector of vertex's normal
};

class Face
{
  public:
    unsigned int face_id;                // ID of face element
    std::vector<unsigned int> vertex_id; // ID of vertices inside this face
};

class Mesh
{
  public:
    Mesh(std::vector<glm::vec3> vertex_data, std::vector<unsigned int> index_data)
    {
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

        // Creating faces array
        for (unsigned int i = 0; i < index_data.size(); i += 3)
        {
            faces.push_back(Face());
            faces.back().face_id = faces.size() - 1;
            faces.back().vertex_id = std::vector<unsigned int>{index_data.at(i), index_data.at(i + 1), index_data.at(i + 2)};

            // Adding face information to vertices array
            vertices.at(faces.back().vertex_id.at(0)).face_id.push_back(faces.back().face_id);
            vertices.at(faces.back().vertex_id.at(1)).face_id.push_back(faces.back().face_id);
            vertices.at(faces.back().vertex_id.at(2)).face_id.push_back(faces.back().face_id);
        }

        // Gathering adjacent vertices
        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through main vertices
        {
            for (unsigned int j = 0; j < vertices.at(i).face_id.size(); j++)
            // Iterating through the faces that contain the main vertex
            {
                // Iterating through vertices inside faces that contain the main vertex
                // with algorithm for keeping correct face positive direction

                if (faces.at(vertices.at(i).face_id.at(j)).vertex_id.at(0) == vertices.at(i).vertex_id)
                // If main vertex if the first in the face
                {
                    vertices.at(i).adjacent_vertex_id.push_back(
                        faces.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(1));
                    vertices.at(i).adjacent_vertex_id.push_back(
                        faces.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(2));
                }

                if (faces.at(vertices.at(i).face_id.at(j)).vertex_id.at(1) == vertices.at(i).vertex_id)
                // If main vertex if the second in the face
                {
                    vertices.at(i).adjacent_vertex_id.push_back(
                        faces.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(2));
                    vertices.at(i).adjacent_vertex_id.push_back(
                        faces.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(0));
                }

                if (faces.at(vertices.at(i).face_id.at(j)).vertex_id.at(2) == vertices.at(i).vertex_id)
                // If main vertex if the third in the face
                {
                    vertices.at(i).adjacent_vertex_id.push_back(
                        faces.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(0));
                    vertices.at(i).adjacent_vertex_id.push_back(
                        faces.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(1));
                }
            }
        }

        // Calculating static normals
        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through main vertices
        {
            for (unsigned int j = 0; j < vertices.at(i).adjacent_vertex_id.size() - 1; j += 2)
            // Iterating through adjacent vertices
            {
                if (vertices.at(i).adjacent_vertex_id.size() == 12 && j == 11)
                // Check if main vertex is surrounded by all sides with other vertices
                // Corresponds to special case
                {
                    vertices.at(i).normal += glm::cross(
                        vertices.at(vertices.at(i).adjacent_vertex_id.at(j)).coordinates - vertices.at(i).coordinates,
                        vertices.at(vertices.at(i).adjacent_vertex_id.at(0)).coordinates - vertices.at(i).coordinates);
                }
                else
                {
                    vertices.at(i).normal += glm::cross(
                        vertices.at(vertices.at(i).adjacent_vertex_id.at(j)).coordinates - vertices.at(i).coordinates,
                        vertices.at(vertices.at(i).adjacent_vertex_id.at(j + 1)).coordinates - vertices.at(i).coordinates);
                }
            }
            vertices.at(i).normal = glm::normalize(vertices.at(i).normal);
        }

        // Generating raw data
        raw_vertex_data = generate_raw_vertex_data();
        raw_vertex_coordinates = generate_raw_vertex_coordinates();
        raw_vertex_indices = generate_raw_vertex_indices();

        // print_face_data();
        // print_vertex_data();
    }

    /*** Functions for exporting data to buffer format ***/

    float *get_raw_vertex_data()
    // Gets interleaved vertex coordinates and normals
    {
        return raw_vertex_data.data();
    }

    unsigned int get_raw_vertex_data_size()
    // Gets size of interleaved vertex coordinates and normals
    {
        return raw_vertex_data.size() * sizeof(float);
    }

    float *get_raw_vertex_coordinates()
    // Gets vertex coordinates
    {
        return raw_vertex_coordinates.data();
    }

    unsigned int get_raw_vertex_coordinates_size()
    // Gets size of vertex coordinates
    {
        return raw_vertex_coordinates.size() * sizeof(float);
    }

    unsigned int *get_raw_vertex_indices()
    // Gets vertex indices
    {
        return raw_vertex_indices.data();
    }

    unsigned int get_raw_vertex_indices_size()
    // Gets size of vertex indices
    {
        return raw_vertex_indices.size() * sizeof(unsigned int);
    }

  private:
    // Structured data
    std::vector<Face> faces;      // Vector with all face data from mesh
    std::vector<Vertex> vertices; // Vector with all vertex data from mesh

    // Raw data
    std::vector<float> raw_vertex_data;
    std::vector<float> raw_vertex_coordinates;
    std::vector<unsigned int> raw_vertex_indices;

    std::vector<float> generate_raw_vertex_data()
    {
        std::vector<float> vertex_data;

        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through vertices
        {
            for (unsigned int j = 0; j < 3; j++)
            // Iterating through coordinate elements
            {
                vertex_data.push_back(vertices.at(i).coordinates[j]);
            }
            for (unsigned int j = 0; j < 3; j++)
            // Iterating through coordinate elements
            {
                vertex_data.push_back(vertices.at(i).normal[j]);
            }
        }

        return vertex_data;
    }

    std::vector<float> generate_raw_vertex_coordinates()
    {
        std::vector<float> vertex_coordinates;

        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through vertices
        {
            for (unsigned int j = 0; j < 3; j++)
            // Iterating through coordinate elements
            {
                vertex_coordinates.push_back(vertices.at(i).coordinates[j]);
            }
        }

        return vertex_coordinates;
    }

    std::vector<unsigned int> generate_raw_vertex_indices()
    {
        std::vector<unsigned int> vertex_indices;

        for (unsigned int i = 0; i < faces.size(); i++)
        // Iterating through faces
        {
            for (unsigned int j = 0; j < faces.at(i).vertex_id.size(); j++)
            // Iterating through vertex IDs
            {
                vertex_indices.push_back(faces.at(i).vertex_id.at(j));
            }
        }

        return vertex_indices;
    }

    void print_vertex_data()
    {
        printf("Vertex data:\n");
        for (unsigned int i = 0; i < vertices.size(); i++)
        {
            printf("%u", vertices.at(i).vertex_id);

            printf("\tCoordinates:\t%.3f\t%.3f\t%.3f\n",
                   vertices.at(i).coordinates.x, vertices.at(i).coordinates.y, vertices.at(i).coordinates.z);

            printf("\tNormal:\t\t%.3f\t%.3f\t%.3f\n",
                   vertices.at(i).normal.x, vertices.at(i).normal.y, vertices.at(i).normal.z);

            printf("\tFaces ID:\t");
            for (unsigned int j = 0; j < vertices.at(i).face_id.size(); j++)
            {
                printf("%u\t", vertices.at(i).face_id.at(j));
            }
            printf("\n\tAdjc vert ID:\t");
            for (unsigned int j = 0; j < vertices.at(i).adjacent_vertex_id.size(); j++)
            {
                printf("%u\t", vertices.at(i).adjacent_vertex_id.at(j));
            }
            printf("\n");
        }
    }

    void print_face_data()
    {
        printf("Face data:\n");
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            printf("%u\t%u\t%u\t%u\n",
                   faces.at(i).face_id,
                   faces.at(i).vertex_id.at(0), faces.at(i).vertex_id.at(1), faces.at(i).vertex_id.at(2));
        }
    }
};

#endif