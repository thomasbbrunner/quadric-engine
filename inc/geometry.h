#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <common.h>
#include <shader.h>
#include <camera.h>
#include <light.h>

/*** PARENT CLASSES ***/

class Vertex
{
  public:
    unsigned int vertex_id;                       // ID of vertex
    glm::vec3 coordinates;                        // Positional vector of this vertex
    std::vector<unsigned int> face_id;            // ID of polygons that contain this vertex
    std::vector<unsigned int> adjacent_vertex_id; // ID os vertices that are adjacent to this vertex
    glm::vec3 normal;                             // Directional vector of vertex's normal
};

class Polygon
{
  public:
    unsigned int face_id;                // ID of polygon element
    std::vector<unsigned int> vertex_id; // ID of vertices inside this polygon
};

class Model
{
  public:
    /*** Functions for exporting data to buffer format ***/

    float *get_raw_vertex_coordinates()
    {
        return raw_vertex_coordinates.data();
    }

    unsigned int get_raw_vertex_coordinates_size_bytes()
    {
        return raw_vertex_coordinates.size() * sizeof(float);
    }

    float *get_raw_vertex_normals()
    {
        return raw_vertex_normals.data();
    }

    unsigned int get_raw_vertex_normals_size_bytes()
    {
        return raw_vertex_normals.size() * sizeof(float);
    }

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

  protected:
    // Structured data
    std::vector<Polygon> polygons; // Vector with all polygon data from mesh
    std::vector<Vertex> vertices;  // Vector with all vertex data from mesh

    // Raw data
    std::vector<float> raw_vertex_coordinates;
    std::vector<float> raw_vertex_normals;
    std::vector<unsigned int> raw_vertex_adjacents;
    std::vector<unsigned int> raw_vertex_indices;

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

    std::vector<unsigned int> generate_raw_vertex_adjacents()
    {
        std::vector<unsigned int> vec;

        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through vertices
        {
            for (unsigned int j = 0; j < 3; j++)
            // Iterating through coordinate elements
            {
                // vec.push_back(vertices.at(i).);
            }
        }

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

            printf("\tPolygon ID:\t");
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
        printf("Polygon data:\n");
        for (unsigned int i = 0; i < polygons.size(); i++)
        {
            printf("%u\t%u\t%u\t%u\n",
                   polygons.at(i).face_id,
                   polygons.at(i).vertex_id.at(0), polygons.at(i).vertex_id.at(1), polygons.at(i).vertex_id.at(2));
        }
    }
};

class Mesh : public Model
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

        // Creating polygons array
        for (unsigned int i = 0; i < index_data.size(); i += 3)
        {
            polygons.push_back(Polygon());
            polygons.back().face_id = polygons.size() - 1;
            polygons.back().vertex_id = std::vector<unsigned int>{index_data.at(i), index_data.at(i + 1), index_data.at(i + 2)};

            // Adding polygon information to vertices array
            vertices.at(polygons.back().vertex_id.at(0)).face_id.push_back(polygons.back().face_id);
            vertices.at(polygons.back().vertex_id.at(1)).face_id.push_back(polygons.back().face_id);
            vertices.at(polygons.back().vertex_id.at(2)).face_id.push_back(polygons.back().face_id);
        }

        // Gathering adjacent vertices
        for (unsigned int i = 0; i < vertices.size(); i++)
        // Iterating through main vertices
        {
            for (unsigned int j = 0; j < vertices.at(i).face_id.size(); j++)
            // Iterating through the polygons that contain the main vertex
            {
                // Iterating through vertices inside polygons that contain the main vertex
                // with algorithm for keeping correct polygon positive direction

                if (polygons.at(vertices.at(i).face_id.at(j)).vertex_id.at(0) == vertices.at(i).vertex_id)
                // If main vertex is the first in the polygon
                {
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(1));
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(2));
                }

                if (polygons.at(vertices.at(i).face_id.at(j)).vertex_id.at(1) == vertices.at(i).vertex_id)
                // If main vertex is the second in the polygon
                {
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(2));
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(0));
                }

                if (polygons.at(vertices.at(i).face_id.at(j)).vertex_id.at(2) == vertices.at(i).vertex_id)
                // If main vertex is the third in the polygon
                {
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).face_id.at(j))
                            .vertex_id.at(0));
                    vertices.at(i).adjacent_vertex_id.push_back(
                        polygons.at(vertices.at(i).face_id.at(j))
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
                // Check if main vertex is surrounded on all sides by other vertices
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
        raw_vertex_coordinates = generate_raw_vertex_coordinates();
        raw_vertex_normals = generate_raw_vertex_normals();
        raw_vertex_adjacents = generate_raw_vertex_adjacents();
        raw_vertex_indices = generate_raw_vertex_indices();

        // print_face_data();
        // print_vertex_data();
    }
};

class Contour : public Model
{
  public:
    Contour(std::vector<glm::vec3> vertex_data, std::vector<unsigned int> index_data)
    {
        for (unsigned int i = 0; i < vertex_data.size(); i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                raw_vertex_coordinates.push_back(vertex_data.at(i)[j]);
            }
        }

        raw_vertex_indices = index_data;
    }
};

class Dots : public Model
{
  public:
    Dots(std::vector<glm::vec3> vertex_data)
    {
        for (unsigned int i = 0; i < vertex_data.size(); i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                raw_vertex_coordinates.push_back(vertex_data.at(i)[j]);
            }

            raw_vertex_indices.push_back(i);
        }
    }
};

class Geometry
{
  public:
    /*** BUFFER GENERATION ***/

    void generate_buffers(Mesh mesh)
    {
        /*** MESH ***/

        numelements_mesh = mesh.get_raw_vertex_indices_size();

        glGenVertexArrays(1, &VAO_mesh);
        glBindVertexArray(VAO_mesh);

        glGenBuffers(1, &VBO_mesh);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_mesh);
        glBufferData(GL_ARRAY_BUFFER, mesh.get_raw_vertex_coordinates_size_bytes() + mesh.get_raw_vertex_normals_size_bytes(), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.get_raw_vertex_coordinates_size_bytes(), mesh.get_raw_vertex_coordinates());
        glBufferSubData(GL_ARRAY_BUFFER, mesh.get_raw_vertex_coordinates_size_bytes(), mesh.get_raw_vertex_normals_size_bytes(), mesh.get_raw_vertex_normals());

        glGenBuffers(1, &EBO_mesh);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.get_raw_vertex_indices_size_bytes(), mesh.get_raw_vertex_indices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(intptr_t)(mesh.get_raw_vertex_coordinates_size_bytes()));
        glEnableVertexAttribArray(1);

        // texture (move from here?)

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
            printf("ERROR: VBO_mesh was not initialized\n");
            exit(0);
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

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(intptr_t)(dots.get_raw_vertex_coordinates_size_bytes()));
        glEnableVertexAttribArray(1);
    }

    /*** DRAW ***/

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

    /*** VARIABLE ACCESS ***/

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

    glm::mat4 model = glm::mat4(0.0f);
    glm::vec4 color = glm::vec4(0.0f);

    Shader shader;

    int light_ID = -1;
};

#endif