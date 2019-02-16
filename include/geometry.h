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

    unsigned int vertexTexture = 0;

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

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, numelements_mesh, GL_UNSIGNED_INT, 0);
    }

    void draw_contour()
    {
        update_shader();

        glBindVertexArray(VAO_contour);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_LINES, numelements_contour, GL_UNSIGNED_INT, 0);
    }

    void draw_fill()
    {
        update_shader();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, vertexTexture);

        glBindVertexArray(VAO_mesh);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

class Geometry_2D : public Geometry
{
  public:
    // Uses GL_TRIANGLE_FAN instead of GL_TRIANGLES
    void draw_fill()
    {
        update_shader();

        glBindVertexArray(VAO_mesh);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLE_FAN, numelements_mesh, GL_UNSIGNED_INT, 0);
    }

    void draw_wireframe()
    {
        update_shader();

        glBindVertexArray(VAO_mesh);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLE_FAN, numelements_mesh, GL_UNSIGNED_INT, 0);
    }
};

// class Face
// {
//   public:
//     unsigned int ID;

//     Face(int)
//     {
//         glm::vec3 vec;
//     }
// };

/*** 1D GEOMETRY ***/

// class Point
// {
//   public:
//     unsigned int VBO = 0;
//     unsigned int EBO = 0;
//     unsigned int VAO = 0;

//     unsigned int numelements = 0;

//     glm::mat4 model = glm::mat4(1.0f);
//     glm::vec4 color = glm::vec4(1.0f);

//     Point(glm::vec3 **coords, int dotres, int dx, int dz)
//     {
//         int numdotsx = dotres * dx;
//         int numdotsz = dotres * dz;

//         float vertices[3 * numdotsx * numdotsz] = {0.0f};

//         int m = 0;
//         for (int i = 0; i < numdotsx; i++)
//         {
//             for (int j = 0; j < numdotsz; j++)
//             {
//                 vertices[m] = coords[i][j].x;
//                 vertices[m + 1] = coords[i][j].y;
//                 vertices[m + 2] = coords[i][j].z;
//                 m += 3;
//             }
//         }

//         unsigned int indices[numdotsx * numdotsz] = {0};
//         for (int i = 0; i < numdotsx * numdotsz; i++)
//         {
//             indices[i] = i;
//         }

//         numelements = sizeof(indices) / sizeof(indices[0]);

//         glGenVertexArrays(1, &VAO);

//         glBindVertexArray(VAO);

//         glGenBuffers(1, &VBO);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//         glGenBuffers(1, &EBO);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
//         glEnableVertexAttribArray(0);
//     }

//     void draw_fill()
//     {
//         glBindVertexArray(VAO);

//         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//         glDrawElements(GL_POINTS, numelements, GL_UNSIGNED_INT, 0);
//     }

//     void setmodel(glm::mat4 modeltemp)
//     {
//         model = modeltemp;
//     }
// };

// /*** 2D GEOMETRY ***/

// class Pentagon : public Geometry_2D
// {
//   public:
//     Pentagon(float d = 0.5f)
//     {
//         float h = d * sqrt(3) / 2;
//         float vertices[] = {
//             d, 0.0f, 0.0f,
//             d / 2, h, 0.0f,
//             -d / 2, h, 0.0f,
//             -d, 0.0f, 0.0f,
//             -d / 2, -h, 0.0f,
//             d / 2, -h, 0.0f};

//         unsigned int indices_contour[] = {
//             // note that we start from 0!
//             0, 1,
//             1, 2,
//             2, 3,
//             3, 4,
//             4, 5,
//             5, 0};

//         numelements_contour = sizeof(indices_contour) / sizeof(indices_contour[0]);
//         numelements_mesh = 12;

//         // Contour
//         glGenVertexArrays(1, &VAO_contour);

//         glBindVertexArray(VAO_contour);

//         glGenBuffers(1, &VBO_contour);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO_contour);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//         glGenBuffers(1, &EBO_contour);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_contour);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_contour), indices_contour, GL_STATIC_DRAW);

//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
//         glEnableVertexAttribArray(0);

//         // Mesh
//         VAO_mesh = VAO_contour;
//     }
// };

// class Tetra : public Geometry_2D
// {
//   public:
//     Tetra(float size = 1.0f, glm::vec3 coords = glm::vec3(0.0f, 0.0f, 0.0f))
//     {
//         float h = 0.55f;
//         float d = 1.0f;
//         float dT = 0.4f;
//         float vertices[] =
//             {d, -h, 0,
//              d + dT, h, 0,
//              -d, h, 0,
//              -d - dT, -h, 0};

//         for (unsigned int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i++)
//         {
//             vertices[i] *= size;
//         }

//         for (unsigned int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 3)
//         {
//             vertices[i] += coords.x;
//             vertices[i + 1] += coords.y;
//             vertices[i + 2] += coords.z;
//         }

//         unsigned int indices_contour[] =
//             {0, 1,
//              1, 2,
//              2, 3,
//              3, 0};

//         numelements_contour = sizeof(indices_contour) / sizeof(indices_contour[0]);
//         numelements_mesh = 6;

//         // Contour
//         glGenVertexArrays(1, &VAO_contour);

//         glBindVertexArray(VAO_contour);

//         glGenBuffers(1, &VBO_contour);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO_contour);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//         glGenBuffers(1, &EBO_contour);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_contour);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_contour), indices_contour, GL_STATIC_DRAW);

//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
//         glEnableVertexAttribArray(0);

//         // Mesh
//         VAO_mesh = VAO_contour;
//     }
// };

// class Circle : public Geometry_2D
// {
//   public:
//     Circle(float d = 5.0f, int numlin = 20)
//     {
//         float vertices[numlin * 3] = {0};
//         unsigned int indices[numlin * 2] = {0};

//         int helper = 0;

//         for (unsigned int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i++)
//         {
//             if (i % 3 == 0) // x coordinates
//             {
//                 vertices[i] = d * std::cos(helper * 2 * M_PI / numlin);
//             }
//             else if (i % 3 == 1) // y coordinates
//             {
//                 vertices[i] = d * std::sin(helper * 2 * M_PI / numlin);
//             }
//             else if (i % 3 == 2) // z coordinates
//             {
//                 vertices[i] = -1.0f;
//                 helper++;
//             }
//             else
//             {
//                 printf("Error: unexpected condition when building circle\n");
//                 exit(0);
//             }
//         }

//         for (unsigned int i = 0; i < sizeof(indices) / sizeof(indices[0]) / 2; i++)
//         {
//             indices[2 * i] = i;
//             if (2 * i + 1 == sizeof(indices) / sizeof(indices[0]) - 1)
//                 indices[2 * i + 1] = 0;
//             else
//                 indices[2 * i + 1] = i + 1;
//         }

//         numelements_contour = sizeof(indices) / sizeof(indices[0]);
//         numelements_mesh = sizeof(indices) / sizeof(indices[0]);

//         // Contour
//         glGenVertexArrays(1, &VAO_contour);

//         glBindVertexArray(VAO_contour);

//         glGenBuffers(1, &VBO_contour);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO_contour);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//         glGenBuffers(1, &EBO_contour);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_contour);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
//         glEnableVertexAttribArray(0);

//         // Mesh
//         VAO_mesh = VAO_contour;
//     }
// };

/*** 3D GEOMETRY ***/

class Cube : public Geometry
{
  public:
    Cube(float d = 0.5f)
    {
        // Saving vertices
        // float vertices_fill[] = {
        //     -d / 2, -d / 2, -d / 2, 0.0f, 0.0f, -1.0f,
        //     d / 2, -d / 2, -d / 2, 0.0f, 0.0f, -1.0f,
        //     d / 2, d / 2, -d / 2, 0.0f, 0.0f, -1.0f,
        //     -d / 2, d / 2, -d / 2, 0.0f, 0.0f, -1.0f,

        //     -d / 2, -d / 2, d / 2, 0.0f, 0.0f, 1.0f,
        //     d / 2, -d / 2, d / 2, 0.0f, 0.0f, 1.0f,
        //     d / 2, d / 2, d / 2, 0.0f, 0.0f, 1.0f,
        //     -d / 2, d / 2, d / 2, 0.0f, 0.0f, 1.0f,

        //     -d / 2, d / 2, d / 2, -1.0f, 0.0f, 0.0f,
        //     -d / 2, d / 2, -d / 2, -1.0f, 0.0f, 0.0f,
        //     -d / 2, -d / 2, -d / 2, -1.0f, 0.0f, 0.0f,
        //     -d / 2, -d / 2, d / 2, -1.0f, 0.0f, 0.0f,

        //     d / 2, d / 2, d / 2, 1.0f, 0.0f, 0.0f,
        //     d / 2, d / 2, -d / 2, 1.0f, 0.0f, 0.0f,
        //     d / 2, -d / 2, -d / 2, 1.0f, 0.0f, 0.0f,
        //     d / 2, -d / 2, d / 2, 1.0f, 0.0f, 0.0f,

        //     -d / 2, -d / 2, -d / 2, 0.0f, -1.0f, 0.0f,
        //     d / 2, -d / 2, -d / 2, 0.0f, -1.0f, 0.0f,
        //     d / 2, -d / 2, d / 2, 0.0f, -1.0f, 0.0f,
        //     -d / 2, -d / 2, d / 2, 0.0f, -1.0f, 0.0f,

        //     -d / 2, d / 2, -d / 2, 0.0f, 1.0f, 0.0f,
        //     d / 2, d / 2, -d / 2, 0.0f, 1.0f, 0.0f,
        //     d / 2, d / 2, d / 2, 0.0f, 1.0f, 0.0f,
        //     -d / 2, d / 2, d / 2, 0.0f, 1.0f, 0.0f};

        //     unsigned int indices_fill[] = {
        // 0, 1, 2,
        // 0, 2, 3,
        // 4, 5, 6,
        // 4, 6, 7,
        // 8, 9, 10,
        // 8, 10, 11,
        // 12, 13, 14,
        // 12, 14, 15,
        // 16, 17, 18,
        // 16, 18, 19,
        // 20, 21, 22,
        // 20, 22, 23};

        float vertices_fill[] = {
            -d / 2, -d / 2, -d / 2, 0.0f, 0.0f, -1.0f,
            d / 2, d / 2, -d / 2, 0.0f, 0.0f, -1.0f,
            d / 2, -d / 2, -d / 2, 0.0f, 0.0f, -1.0f,
            d / 2, d / 2, -d / 2, 0.0f, 0.0f, -1.0f,
            -d / 2, -d / 2, -d / 2, 0.0f, 0.0f, -1.0f,
            -d / 2, d / 2, -d / 2, 0.0f, 0.0f, -1.0f,

            -d / 2, -d / 2, d / 2, 0.0f, 0.0f, 1.0f,
            d / 2, -d / 2, d / 2, 0.0f, 0.0f, 1.0f,
            d / 2, d / 2, d / 2, 0.0f, 0.0f, 1.0f,
            d / 2, d / 2, d / 2, 0.0f, 0.0f, 1.0f,
            -d / 2, d / 2, d / 2, 0.0f, 0.0f, 1.0f,
            -d / 2, -d / 2, d / 2, 0.0f, 0.0f, 1.0f,

            -d / 2, d / 2, d / 2, -1.0f, 0.0f, 0.0f,
            -d / 2, d / 2, -d / 2, -1.0f, 0.0f, 0.0f,
            -d / 2, -d / 2, -d / 2, -1.0f, 0.0f, 0.0f,
            -d / 2, -d / 2, -d / 2, -1.0f, 0.0f, 0.0f,
            -d / 2, -d / 2, d / 2, -1.0f, 0.0f, 0.0f,
            -d / 2, d / 2, d / 2, -1.0f, 0.0f, 0.0f,

            d / 2, d / 2, d / 2, 1.0f, 0.0f, 0.0f,
            d / 2, -d / 2, -d / 2, 1.0f, 0.0f, 0.0f,
            d / 2, d / 2, -d / 2, 1.0f, 0.0f, 0.0f,
            d / 2, -d / 2, -d / 2, 1.0f, 0.0f, 0.0f,
            d / 2, d / 2, d / 2, 1.0f, 0.0f, 0.0f,
            d / 2, -d / 2, d / 2, 1.0f, 0.0f, 0.0f,

            -d / 2, -d / 2, -d / 2, 0.0f, -1.0f, 0.0f,
            d / 2, -d / 2, -d / 2, 0.0f, -1.0f, 0.0f,
            d / 2, -d / 2, d / 2, 0.0f, -1.0f, 0.0f,
            d / 2, -d / 2, d / 2, 0.0f, -1.0f, 0.0f,
            -d / 2, -d / 2, d / 2, 0.0f, -1.0f, 0.0f,
            -d / 2, -d / 2, -d / 2, 0.0f, -1.0f, 0.0f,

            -d / 2, d / 2, -d / 2, 0.0f, 1.0f, 0.0f,
            d / 2, d / 2, d / 2, 0.0f, 1.0f, 0.0f,
            d / 2, d / 2, -d / 2, 0.0f, 1.0f, 0.0f,
            d / 2, d / 2, d / 2, 0.0f, 1.0f, 0.0f,
            -d / 2, d / 2, -d / 2, 0.0f, 1.0f, 0.0f,
            -d / 2, d / 2, d / 2, 0.0f, 1.0f, 0.0f};

        unsigned int indices_fill[] = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11,
            12, 13, 14,
            15, 16, 17,
            18, 19, 20,
            21, 22, 23,
            24, 25, 26,
            27, 28, 29,
            30, 31, 32,
            33, 34, 35};

        float vertices_contour[] = {
            d / 2, d / 2, d / 2,
            -d / 2, d / 2, d / 2,
            -d / 2, -d / 2, d / 2,
            d / 2, -d / 2, d / 2,
            d / 2, d / 2, -d / 2,
            -d / 2, d / 2, -d / 2,
            -d / 2, -d / 2, -d / 2,
            d / 2, -d / 2, -d / 2};

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
        numelements_mesh = sizeof(indices_fill) / sizeof(indices_fill[0]);

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

        // Mesh
        glGenVertexArrays(1, &VAO_mesh);

        glBindVertexArray(VAO_mesh);

        glGenBuffers(1, &VBO_mesh);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_mesh);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_fill), vertices_fill, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO_mesh);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_fill), indices_fill, GL_STATIC_DRAW);

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
    unsigned int numelements_dots = 0;

    Plane(int dotres, int dx, int dz)
    {
        std::vector<std::vector<glm::vec3>> coords = create(dotres, dx, dz);

        // Print::array(coords);

        unsigned int coords_size = coords.size() * coords.at(0).size();

        std::vector<glm::vec3> vertices(coords_size);

        for (unsigned int i = 0; i < coords.size(); i++)
        {
            for (unsigned int j = 0; j < coords.at(0).size(); j++)
            {
                vertices.at(i * coords.at(0).size() + j) = coords.at(i).at(j);
            }
        }

        // Print::array(vertices);

        for (unsigned int i = 0; i < coords_size; i++)
        {
            vertices.push_back(glm::vec3(0.0, 1.0, 0.0));
        }

        // Print::array(vertices);

        std::vector<unsigned int> indices_mesh(3 * (coords.at(0).size() - 1) * 2 * (coords.size() - 1));

        int m = 0;
        for (unsigned int i = 0; i < coords.size() - 1; i++)
        {
            for (unsigned int j = 0; j < coords.at(0).size() - 1; j++)
            {
                indices_mesh.at(m) = j + i * coords.at(0).size();
                indices_mesh.at(m + 1) = j + (i + 1) * coords.at(0).size() + 1;
                indices_mesh.at(m + 2) = j + (i + 1) * coords.at(0).size();
                indices_mesh.at(m + 3) = j + i * coords.at(0).size();
                indices_mesh.at(m + 4) = j + i * coords.at(0).size() + 1;
                indices_mesh.at(m + 5) = j + (i + 1) * coords.at(0).size() + 1;
                m += 6;
            }
        }

        std::vector<unsigned int> indices_dots(vertices.size());

        for (unsigned int i = 0; i < indices_dots.size(); i++)
        {
            indices_dots.at(i) = i;
        }

        numelements_mesh = indices_mesh.size();

        numelements_dots = indices_dots.size();

        // Mesh
        glGenVertexArrays(1, &VAO_mesh);
        glBindVertexArray(VAO_mesh);

        glGenBuffers(1, &VBO_mesh);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_mesh);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.at(0)) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO_mesh);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_mesh.at(0)) * indices_mesh.size(), indices_mesh.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(sizeof(vertices.at(0)) * coords_size));
        glEnableVertexAttribArray(1);

        // Texture
        glGenTextures(1, &vertexTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, vertexTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 3* dx *dz, 1, 0, GL_RGB, GL_FLOAT, (void *)vertices.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, VBO_mesh);
        // std::vector<float> pixels(30);
        // float pixels[100] = {0.0};
        // glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, pixels.data());

        // Print::array(vertices.data(), 10);
        // Print::array(pixels);
        // printf("out\n");

        // Dots
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

        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_POINTS, numelements_dots, GL_UNSIGNED_INT, 0);
    }

  private:
    static std::vector<std::vector<glm::vec3>> create(int dotres, int dx, int dz)
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