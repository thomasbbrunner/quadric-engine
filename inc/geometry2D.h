#ifndef GEOMETRY2D_H
#define GEOMETRY2D_H

#include <common.h>
#include <shader.h>
#include <camera.h>
#include <light.h>
#include <geometry.h>


/*** 2D GEOMETRY ***/

// class Geometry_2D : public Geometry
// {
//   public:
//     // Uses GL_TRIANGLE_FAN instead of GL_TRIANGLES
//     void draw_fill()
//     {
//         update_shader();

//         glBindVertexArray(VAO_mesh);

//         // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//         glDrawElements(GL_TRIANGLE_FAN, numelements_mesh, GL_UNSIGNED_INT, 0);
//     }

//     void draw_wireframe()
//     {
//         update_shader();

//         glBindVertexArray(VAO_mesh);

//         // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//         glDrawElements(GL_TRIANGLE_FAN, numelements_mesh, GL_UNSIGNED_INT, 0);
//     }
// };

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

#endif