#ifndef GEOMETRY1D_H
#define GEOMETRY1D_H

#include <common.h>
#include <shader.h>
#include <camera.h>
#include <light.h>
#include <geometry.h>


/*** 1D GEOMETRY ***/

// class Geometry_1D : public Geometry
// {
// };

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

#endif